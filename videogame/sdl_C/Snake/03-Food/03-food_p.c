/*This source code copyrighted by Lazy Foo' Productions 2004-2024
and may not be redistributed without written permission.*/

//Using SDL and standard IO

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include <SDL2/SDL.h>

//Screen dimension constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define N_CELLS_H 10 // Lui lo chiama BOARD_WIDTH
#define N_CELLS_V 10 // Lui lo chiama BOARD_HEIGHT

#define CELL_WIDTH ((SCREEN_WIDTH / N_CELLS_H))
#define CELL_HEIGHT ((SCREEN_HEIGHT / N_CELLS_V))

#define DELAY_FOOD_SPAWN 3 // Numero di secondi tra uno spawn e l'altro può essere anche un float
#define FOODS_COUNT 1 // Numero di cibi che voglio far apparire

// STRUTTURE DATI
typedef struct {
    int x;
    int y;
} Pos;

typedef struct {
    Pos pos;
    int score; // Punteggio che si ottiene mangiando questo cibo
} Food;


typedef struct {
    Food food[FOODS_COUNT];
    int quit;
} Game;

// PROTOTIPI
void scc(int code);
void *scp(void *ptr); 

int random_int_range(int min, int max);
Pos random_board_pos(void);
int pos_is_not_empty(Game *game, Pos p);
Pos random_empty_board_pos(Game *game);

void init_game(Game *game);
void init_food(Game *game);
int allow_refresh_food(void);

void render_board(SDL_Renderer * renderer);
void render_game(SDL_Renderer * renderer,  Game * game);
void render_food(SDL_Renderer * renderer, Game * game);
void remove_food(SDL_Renderer * renderer, Game * game);
void update_food(SDL_Renderer * renderer, Game *game);


// fine PROTOTIPI


// VAR GLOBALI
Game game = {0};


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
            if(event.type == SDL_QUIT)
            {
                game.quit = 1;
            }
        }
        // ** INSERIASCI IN UNA DELLE SEGUENTI RIGHE LA FUNZIONE update_food
        scc(SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0));
        SDL_RenderClear(renderer);
        render_game(renderer, &game);

        update_food(renderer, &game);
        // ** FINE INSERIMENTO DELLA FUNZIONE update_food

        SDL_RenderPresent(renderer);
    }
    return 0;
}

// DEFINIZIONI FUNZIONI

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

// [min, max)
int random_int_range(int min, int max) { return min + rand() % (max - min);}

Pos random_board_pos(void) {
        Pos p = {0};
        
        // ** GENERARE UNA POSIZIONE RANDOMICA IN UNA CASELLA DELLA BOARD

        return p;
}

int pos_is_not_empty(Game *game, Pos p) {
        
        // ** SE p è OCCUPATA DAL CIBO RESTITUIRE 1

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

// GAME LOGIC
void init_game(Game *game) {
    game->quit = 0;
    init_food(game);
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
        gettimeofday(&old_t, NULL); 
        return 1;
    }

    gettimeofday(&new_t, NULL);

    time_elapsed = (double) (new_t.tv_usec - old_t.tv_usec) / 1000000 + (double) (new_t.tv_sec - old_t.tv_sec);
    if (time_elapsed < DELAY_FOOD_SPAWN) {
        return 0;
    }
    else {
        old_t = new_t;
        return 1;   
    }
 }


void update_food(SDL_Renderer * renderer, Game *game) {
    if(allow_refresh_food()) {
        remove_food(renderer, game);
        init_food(game);
    }
    return;
}

void render_game(SDL_Renderer * renderer, Game * game) {
    render_board(renderer);
    render_food(renderer, game); // anche game perché le info sul cibo stanno nella struttura del nostro gioco
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

void render_food(SDL_Renderer * renderer, Game * game) {
    scc(SDL_SetRenderDrawColor(renderer, 119, 178, 140, 0)); // colore verdognolo
    
    // ** Per ogni FOOD che score diverso da zero disegnare il rettangolo
        for(int i = 0; i < FOODS_COUNT; i++) {   

            // ** Per ogni FOOD che score diverso da zero
            // ** costrire un struttura SDL_Rect 
            
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