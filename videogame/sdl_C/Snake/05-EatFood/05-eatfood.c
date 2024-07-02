/*This source code copyrighted by Lazy Foo' Productions 2004-2024
and may not be redistributed without written permission.*/

//Using SDL and standard IO

#include <stdio.h>
#include <time.h>
#include <math.h> // Serve per floorf ma che non usiamo più...
#include <sys/time.h>
#include <SDL2/SDL.h>

//Screen dimension constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define N_CELLS_H 10 // Lui lo chiama BOARD_WIDTH
#define N_CELLS_V 10 // Lui lo chiama BOARD_HEIGHT

#define CELL_WIDTH ((SCREEN_WIDTH / N_CELLS_H))
#define CELL_HEIGHT ((SCREEN_HEIGHT / N_CELLS_V))

#define MAX_SNAKE_MOVEMENT 0.15 // tempo massimo oltre il quale aggiorno la posizione del serpente
#define MIN_SNAKE_MOVEMENT 0.06 // tempo minimo oltre il quale aggiorno la posizione del serpente
#define STEP_SNAKE_MOVEMENT 0.03 // valore da sottrare mano mano MAX_SNAKE_MOVEMENTS fino ad arrivare MIN_SNAKE_MOVEMENTS

#define MAX_SNAKE_LENGTH ((N_CELLS_H) * (N_CELLS_V)) // Lunghezza massima del serpente

#define DELAY_FOOD_SPAWN 3 // Numero di secondi tra uno spawn e l'altro può essere anche un float
#define FOODS_COUNT 1 // Numero di cibi che voglio far apparire

// STRUTTURE DATI
typedef struct {
    int x;
    int y;
} Pos;

typedef enum {
    DIR_RIGHT = 0, DIR_UP = 1, DIR_LEFT = 2, DIR_DOWN = 3
} Dir;

typedef struct {
    Pos pos;
    int score; // Punteggio che si ottiene mangiando questo cibo
} Food;

typedef struct {
    Pos body[MAX_SNAKE_LENGTH];
    int length; // lunghezza effettiva del serpente
    Dir dir; // direzione in cui si muove il serpente
} Snake;

typedef struct {
    Snake snake;
    Food food[FOODS_COUNT];
    double game_speed; 
    int quit;
    int global_score;
} Game;

// PROTOTIPI
void scc(int code);
void *scp(void *ptr); 

int random_int_range(int min, int max);
Pos random_board_pos(void);
int pos_is_not_empty(Game *game, Pos p);
Pos random_empty_board_pos(Game *game);
void move_snake(Game *game, Dir new_dir, int manual);

void init_game(Game *game);

Pos *get_snake_head(Snake *snake);
int allow_snake_movement(int manual, Game *game);
Pos peak_next_pos(Snake *snake, Dir new_dir);

void eat_food(Game *game, Food *f);

void init_food(Game *game);
int allow_refresh_food(void);
Food * check_for_food(Game *game);

void update_food(SDL_Renderer * renderer, Game *game);

void update_game_speed(Game *game);

void render_board(SDL_Renderer * renderer);
void render_game(SDL_Renderer * renderer,  Game * game);
void render_snake(SDL_Renderer * renderer,  Game * game);
void render_food(SDL_Renderer * renderer, Game * game);
void remove_food(SDL_Renderer * renderer, Game * game);


// fine PROTOTIPI


// VAR GLOBALI
Game game = {0};

// Main
int main( int argc, char* args[] )
{
	srand(time(0)); // è lo stesso se al posto di 0 metto NULL 
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* const window = scp(SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE));
    SDL_Renderer *const renderer = scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));

    init_game(&game);
    while(!game.quit)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT) {game.quit = 1;}

            if(event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_UP: {
                    move_snake(&game, DIR_UP, 1);
                    break;
                }
                case SDLK_DOWN: {
                    move_snake(&game, DIR_DOWN, 1);
                    break;
                }
                case SDLK_LEFT: {
                    move_snake(&game, DIR_LEFT, 1);
                    break;
                }
                case SDLK_RIGHT: {
                    move_snake(&game, DIR_RIGHT, 1);
                    break;
                }

                default:
                    break;
                }
            }
        }
        // main logic loop
        move_snake(&game, game.snake.dir, 0); // automatic movement
        
        Food * f = check_for_food(&game);
        if (f) { // Se f è diverso da NULL
            eat_food(&game, f);
            update_game_speed(&game);
        }
        update_food(renderer, &game);


        
        // rendering stuff
        render_game(renderer, &game);         
    }
    return 0;
}
// FINE MAIN

// DEFINIZIONI FUNZIONI

// Wrapper per la funzioni di SDL che ritornano un codice di errore
void scc(int code) {
    if (code !=0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        exit(1);
    }
    return;
}
void *scp(void *ptr) {
    if(ptr == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    return ptr;
}
// ** Fine Wrapper **

// ** RANDOM FUNCTIONS
// [min, max)
int random_int_range(int min, int max) { return min + rand() % (max - min);}

Dir random_dir(void) { return (Dir) random_int_range(0, 4);} // tra 0 e 3

Pos random_board_pos(void) {
    Pos p = {0};
    p.x = random_int_range(0, N_CELLS_H);
    p.y = random_int_range(0, N_CELLS_V);
    return p;
}

int pos_is_not_empty(Game *game, Pos p) {
    for(int i = 0; i < FOODS_COUNT; i++) {
        if(game->food[i].pos.x == p.x && game->food[i].pos.y == p.y) return 1;
    }
    // TODO: controllare se la posizione è occupata dal serpente
    // TODO: controllare se la posizione è occupata da un ostacolo
    return 0;
}
Pos random_empty_board_pos(Game *game) {
    Pos p = {0}; // Struct vuota
    // Genero posizioni casuali finchè non trovo una posizione vuota
    do {
        p = random_board_pos();
    } while(pos_is_not_empty(game, p));

    return p;
}
// ** Fine RANDOM FUNCTIONS **


// GAME LOGIC
void init_game(Game *game) {
    game->snake.body[0] = random_board_pos(); // la testa inizia in una posizione casuale
    game->snake.length = 1;
    game->snake.dir = random_dir();
    game->quit = 0;
    
    init_food(game);

    // TODO: inizializzare gli ostacoli

    game->global_score = 0;
    game->game_speed = MAX_SNAKE_MOVEMENT;

}

Pos *get_snake_head(Snake *snake) { return &snake->body[snake->length -1];}

int allow_snake_movement(int manual, Game *game) { // maual vale 0 se automatico, 1 se manuale
    static struct timeval old_t = {0};
    static struct timeval new_t = {0};
    static int init = -1;
    double time_elapsed = -1;

    if(init == -1) {
        init = 1;
        gettimeofday(&old_t, NULL); // secondo parametro è il fuso orario non più utilizzato
        return manual ? 1:0;
    }

    gettimeofday(&new_t, NULL);
    // microsecondi + secondi (in modo da avere un differenza di tempo precisa in secondi)
    time_elapsed = (double) (new_t.tv_usec - old_t.tv_usec) / 1000000 + (double) (new_t.tv_sec - old_t.tv_sec);
    if (!manual && time_elapsed < game->game_speed) { // automatico e non è passato il tempo minimo
        return 0; // non fa niente
    }
    else { // si deve muovere
        old_t = new_t;
        return 1;   
    }
 }



Pos peak_next_pos(Snake *snake, Dir new_dir) {
    Pos new_pos;
    Pos *head_pos = get_snake_head(snake);


    switch (new_dir) {
        case DIR_RIGHT: {
            new_pos.x = (head_pos->x + 1) % N_CELLS_H;
            new_pos.y = head_pos->y;
            break;
        }

        case DIR_LEFT: {
            new_pos.x = head_pos->x == 0 ? N_CELLS_H - 1:head_pos->x - 1;
            new_pos.y = head_pos->y;
            break;
        }

        case DIR_UP: {
            new_pos.x = head_pos->x;
            new_pos.y = head_pos->y == 0 ? N_CELLS_V - 1:head_pos->y - 1;
            break;
        }

        case DIR_DOWN: {
            new_pos.x = head_pos->x;
            new_pos.y = (head_pos->y + 1) % N_CELLS_V;
            break;
        }
    }

    return new_pos;

}

// maual vale 0 se automatico, 1 se manuale
void move_snake(Game *game, Dir new_dir, int manual) {
    if(!allow_snake_movement(manual, game)) return; // snake non si deve muovere

    // snake si deve muovere
    Snake *snake = &game->snake;
    Pos new_pos = peak_next_pos(snake, new_dir);

    // can't move back to snake's own tail
    if (snake->length >=2 && 
        new_pos.x == snake->body[snake->length -2].x && 
        new_pos.y == snake->body[snake->length -2].y) return;

    // Perform movement
    Pos *head_pos = get_snake_head(snake);
    Pos old_pos = *head_pos; // copia di valore non di puntatore
    Pos tmp_pos = old_pos; // NON SERVE l'assegnazione

    // Aggiorno la posizione della testa e la direzione dello snake
    *head_pos = new_pos;
    snake->dir = new_dir;

    // Aggiorno la posizione del resto del corpo dello snake (ogni pezzo prende la posizione del pezzo precedente)
    for(int i = snake->length -2; i >=0; i--) {
        tmp_pos = snake->body[i];
        snake->body[i] = old_pos;
        old_pos = tmp_pos;
    }
}

void eat_food(Game *game, Food *f) {
    Snake *snake = &game->snake;

    game->global_score += f->score;
    f->score = 0;
    
    // grow snake's body
    Pos new_pos = peak_next_pos(snake, snake->dir);
    // Attenzione non invertire l'ordine di queste due istruzioni
    snake->length++; 
    snake->body[snake->length-1] = new_pos;
    
    
}

void init_food(Game *game) {
    for(int i = 0; i < FOODS_COUNT; i++) {
        game->food[i].score = 1;
        game->food[i].pos = random_empty_board_pos(game);
    }
    return;
}

int allow_refresh_food(void) {
    static struct timeval old_t = {0};
    static struct timeval new_t = {0};
    static int init = -1;
    Uint32 time_elapsed = -1;

    if(init == -1) {
        init = 1;
        gettimeofday(&old_t, NULL); // secondo parametro è il fuso orario non più utilizzato
        return 1;
    }

    gettimeofday(&new_t, NULL);
    // microsecondi + secondi (in modo da avere un differenza di tempo precisa in secondi)
    time_elapsed = (double) (new_t.tv_usec - old_t.tv_usec) / 1000000 + (double) (new_t.tv_sec - old_t.tv_sec);
    if (time_elapsed < DELAY_FOOD_SPAWN) {
        return 0;
    }
    else {
        old_t = new_t;
        return 1;   
    }
 }


Food * check_for_food(Game *game) {
    Snake *snake = &game->snake;
    Pos head_pos = *get_snake_head(snake);
    
    for (int i = 0; i < FOODS_COUNT; i++) {
       Food *f = &game->food[i];
        if (f->pos.x == head_pos.x && f->pos.y == head_pos.y && f->score > 0 ) {
            return f;
        }
    }
    return NULL;

}

// ** UPDATE FUNCTIONS
void update_food(SDL_Renderer * renderer, Game *game) {
    if(allow_refresh_food()) {
        remove_food(renderer, game);
        init_food(game);
    }
    return;
}

void update_game_speed(Game *game) {
    double step_update = game->global_score * STEP_SNAKE_MOVEMENT;
    printf("Step update: %f\n", step_update);
    if (MAX_SNAKE_MOVEMENT - step_update < MIN_SNAKE_MOVEMENT) {
        game->game_speed = MIN_SNAKE_MOVEMENT;
    }
    else {
        game->game_speed = MAX_SNAKE_MOVEMENT - step_update;
    }
    printf("Game speed: %f\n", game->game_speed);
}
// ** Fine UPDATE FUNCTIONS

// RENDERING FUNCTION
void render_game(SDL_Renderer * renderer, Game * game) {
    scc(SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0));
    SDL_RenderClear(renderer);
    
    render_board(renderer);
    render_snake(renderer, game);
    render_food(renderer, game); // anche game perché le info sul cibo stanno nella struttura del nostro gioco
    
    SDL_RenderPresent(renderer);
}

void render_board(SDL_Renderer * renderer){
 scc(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0));
 // Disegniamo le righe verticali
    for(int x = 0; x < N_CELLS_H; x++) {
        
        // x1, y1, x2, y2
        SDL_RenderDrawLine(renderer, x*CELL_WIDTH, 0, x*CELL_WIDTH, SCREEN_HEIGHT);
    }
    for(int y = 0; y < N_CELLS_V; y++) {
        // x1, y1, x2, y2
        SDL_RenderDrawLine(renderer, 0, y*CELL_HEIGHT, SCREEN_WIDTH, y*CELL_HEIGHT);
    }
}

void render_snake(SDL_Renderer * renderer, Game * game) {
    scc(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0)); // bianco
    Snake *snake = &game->snake;

    for(int i = snake->length -1 ; i >=0; i--) {
        Pos p = snake->body[i];
        SDL_Rect rect = {
            (int) floorf(p.x * CELL_WIDTH),
            (int) floorf(p.y * CELL_HEIGHT),
            (int) floorf(CELL_WIDTH),
            (int) floorf(CELL_HEIGHT)
        };
        scc(SDL_RenderFillRect(renderer, &rect));
    }
}

void render_food(SDL_Renderer * renderer, Game * game) {
    scc(SDL_SetRenderDrawColor(renderer, 119, 178, 140, 0)); // colore verdognolo
    
    for(int i = 0; i < FOODS_COUNT; i++) {   
        Food f = game->food[i];
        
        if (f.score == 0) continue;   
        
        // credo che non sia necessario fare florrf perché il calcolo da già un intero e non un float
        // quindi basterebbe fare f.pos.x * CELL_WIDTH
        SDL_Rect rect = {
            (int) floorf(f.pos.x * CELL_WIDTH),
            (int) floorf(f.pos.y * CELL_HEIGHT),
            CELL_WIDTH,
            CELL_HEIGHT};

        scc(SDL_RenderFillRect(renderer, &rect));
    }
    return;
}

void remove_food(SDL_Renderer * renderer, Game * game) {
    scc(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0)); // setto al colore di sfondo

    for(int i = 0; i < FOODS_COUNT; i++) {
        Food f = game->food[i];
        
        if(!f.score) continue;

        SDL_Rect rect = {
            (int) floorf(f.pos.x * CELL_WIDTH),
            (int) floorf(f.pos.y * CELL_HEIGHT),
            CELL_WIDTH,
            CELL_HEIGHT};
        
        scc(SDL_RenderFillRect(renderer, &rect));
    }
}