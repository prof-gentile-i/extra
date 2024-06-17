# Creazione dello snake


## Strutture per lo snake
Lo snake avra una direzione di movimento, a tale scopo definiamo la seguente struttura
```C
    typedef enum {
        DIR_RIGHT = 0, DIR_UP = 1, DIR_LEFT = 2, DIR_DOWN = 3
    } Dir;
```
Lo snake avrà una dimensione massima che è numero totale di celle della board
```C
    #define MAX_SNAKE_LENGTH ((N_CELLS_H) * (N_CELLS_V)) // Lunghezza massima del serpente
```
mentre la struttura dati dello snake è la seguente
```c
    typedef struct {
        Pos body[MAX_SNAKE_LENGTH];
        int length; // lunghezza effettiva del serpente
        Dir dir; // direzione in cui si muove il serpente
    } Snake;
```
in cui per convenzione l'ultima posizione occupata dell'array ovverro `body[length-1]`  è la testa dello snake.

Lo snake ogni volta che mangerà un food andrà più veloce, cioè aggiornerà la sua posizione sullo schermo (in base alla sua direzione di movimento) ogni tot secondi partendo da un valore di tempo iniziale (più lento) fino ad arrivare a un valore di tempo finale (più veloce) e passando per vari step di tempo intermedi

```c
    #define MAX_SNAKE_MOVEMENTS 0.15 // tempo massimo oltre il quale aggiorno la posizione del serpente
    #define MIN_SNAKE_MOVEMENTS 0.06 // tempo minimo oltre il quale aggiorno la posizione del serpente
    #define STEP_SNAKE_MOVEMENTS 0.3 // valore da sottrare mano mano MAX_SNAKE_MOVEMENTS fino ad arrivare MIN_SNAKE_MOVEMENTS
```
Tali valori vanno settati anche in base al numero di celle impostate (`N_CELLS_H`, `N_CELLS_V`).

Dobbiamo poi aggiornare la struct `Game` mettendo anche lo snake e una `game_speed` che è la velocità con cui aggiornare lo snake

```c
typedef struct {
    Snake snake; // NEW
    Food food[FOODS_COUNT];
    double game_speed; // NEW
    int quit;
} Game;
```

## Inizializzazione del gioco
Nella inizializzazione del gioco dobbiamo gestire anche l'inizializzazione dello snake e signfica mettere la testa in una posizione qualsiasi della bord (bisogna farlo facendolo prima del food, sai dire il perché??) e dargli una direzione casuale quindi

```c
void init_game(Game *game) {
    game->snake.body[0] = random_board_pos(); // NEW: la testa inizia in una posizione casuale
    game->snake.length = 1; // NEW: 
    game->snake.dir = random_dir(); // NEW
    game->quit = 0;
    init_food(game);

    game->game_speed = MAX_SNAKE_MOVEMENT;
}
```
dove la seguente deve generare un numero casuale intero tra `0` e `3`
```c
Dir random_dir(void) { 
    // ** DA FARE **
} // tra 0 e 3
```

## Rendering dello snake
Ora renderizziamo lo snake aggiungendo nel rendere_game
```c
void render_game(SDL_Renderer * renderer, Game * game) {
    render_board(renderer);
    render_snake(renderer, game); // NEW
    render_food(renderer, game); // anche game perché le info sul cibo stanno nella struttura del nostro gioco
}
```
dove `render_snake` è la seguente che renderizza il serpente a partire dalla testa (ma sarebbe lo stesso se partisse dalla coda)
```c
void render_snake(SDL_Renderer * renderer, Game * game) {
    scc(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0)); // per ora bianco
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
```



Sposto in `render_game` tutto ciò che a che fare con render quindi le seguenti istruzioni commentate con `NEW` sono tolte dal main 

```c
void render_game(SDL_Renderer * renderer, Game * game) {
    scc(SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0)); // NEW
    SDL_RenderClear(renderer); // NEW
    
    render_board(renderer);
    render_snake(renderer, game);
    render_food(renderer, game); // anche game perché le info sul cibo stanno nella struttura del nostro gioco
    
    SDL_RenderPresent(renderer); // NEW 
}
```

## Gestione degli eventi
Poi nella gestione degli eventi del `main` intercetto la pressione dei tasti e in particolare le frecce e in tal caso muoverò lo snake (con la funzione `move_snake` che ora implementeremo)

```c
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

            // ******* INIZIO NEW
            if(event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_UP: {
                    move_snake(&game, DIR_UP, 1); // 1= manual movement
                    break;
                }
                case SDLK_DOWN: {
                    move_snake(&game, DIR_DOWN, 1); // 1= manual movement
                    break;
                }
                case SDLK_LEFT: {
                    move_snake(&game, DIR_LEFT, 1); // 1= manual movement
                    break;
                }
                case SDLK_RIGHT: {
                    move_snake(&game, DIR_RIGHT, 1); // 1= manual movement
                    break;
                }

                default:
                    break;
                }
            } 
            // ************FINE NEW
        }
        // main logic loop
        move_snake(&game, game.snake.dir, 0); // NEW,  0= manual movement
        update_food(renderer, &game);
        
        // rendering stuff
        render_game(renderer, &game);         
    }
    return 0;
}
```

## Movimento dello snake
Il movimento dello snake avviene attraverso la funzione `move_snake`. 

Lo snake si muove quando l'utente clicca su uno dei tasti freccia oppure automaticamente quando passa il tempo `game->game_speed`. Quindi controllare innanziatutto attraverso il metodo `allow_snake_movement` se lo snake può muoversi o no (sarà `0` se non si può muovere)
  * Non si deve muovere quando il movimento è impostato in automatico e quando non è passato il tempo `game->game_speed`
  * Viceversa si muoverà


```c
int allow_snake_movement(int manual, Game *game) { // maual vale 0 se automatico, 1 se manuale
    static struct timeval old_t = {0};
    static struct timeval new_t = {0};
    static int init = -1;
    Uint32 time_elapsed = -1;

    if(init == -1) {
        init = 1;
        gettimeofday(&old_t, NULL); // secondo parametro è il fuso orario non più utilizzato
        return manual ? 1:0;
    }

    gettimeofday(&new_t, NULL);
    // microsecondi + secondi (in modo da avere un differenza di tempo precisa in secondi)
    time_elapsed = (double) (new_t.tv_usec - old_t.tv_usec) / 1000000 + (double) (new_t.tv_sec - old_t.tv_sec);
    if (//** INSERISCI LA CONDIZIONE ** ) { // automatico e non è passato il tempo minimo
        return 0; // non fa niente
    }
    else { // si deve muovere
        old_t = new_t;
        return 1;   
    }
 }

```
Vediamo come avviene il movimento dello snake
  * si considera la nuova posizione della testa dello snake in base alla sua direzione (funzione `peak_next_pos`) e la si memorizza in una variabile `new_pos`
  * si oggiorna la testa dello snake al valore new_pos e le altre parti del corpo dello snake vanno ad occupare la posizioni della parte del corpo che la precede più vicina alla testa
```c
// maual vale 0 se automatico, 1 se manuale
void move_snake(Game *game, Dir new_dir, int manual) {
    if(!allow_snake_movement(manual)) return; // snake non si deve muovere

    // snake si deve muovere
    Snake *snake = &game->snake;
    Pos new_pos = peak_next_pos(snake, new_dir);


    // Perform movement
    Pos *head_pos = get_snake_head(snake);
    Pos old_pos = *head_pos; // copia di valore non di puntatore
    Pos tmp_pos; 

    // ** 
    // AGGIORNARE la posizione della testa e la direzione dello snake
    // **

    // ** AGGIORNARE  la posizione del resto del corpo dello snake (ogni pezzo prende la posizione del pezzo precedente)
    for(int i = snake->length -2; i >=0; i--) {
        // ** SCRIVERE IL CODICE
    }
    // ** 
}
```

Il metodo `peak_next_pos` che calcola la nuova posizione della testa è il seguente, in cui come si vede se la posizione muovendosi verso destra supera il numero di celle allora riparte da sinistra e analogamente per le altre posizioni (da sopra risbuca sotto, da sinistra risbuca a destra). Riportiamo anche il banale metodo `get_snake_head` che restituisce la testa dello snake

```c
Pos *get_snake_head(Snake *snake) { return &snake->body[snake->length -1];}
```

```c
Pos peak_next_pos(Snake *snake, Dir new_dir) {
    Pos new_pos;
    Pos *head_pos = get_snake_head(snake);


    switch (new_dir) {
        case DIR_RIGHT: {
            new_pos.x = // ** SCRIVI L'ESPRESSIONE
            new_pos.y = // ** SCRIVI L'ESPRESSIONE 
            break;
        }

        case DIR_LEFT: {
            new_pos.x = // ** SCRIVI L'ESPRESSIONE
            new_pos.y = // ** SCRIVI L'ESPRESSIONE
            break;
        }

        case DIR_UP: {
            new_pos.x = // ** SCRIVI L'ESPRESSIONE
            new_pos.y = // ** SCRIVI L'ESPRESSIONE
            break;
        }

        case DIR_DOWN: {
            new_pos.x = // ** SCRIVI L'ESPRESSIONE
            new_pos.y = // ** SCRIVI L'ESPRESSIONE
            break;
        }
    }

    return new_pos;

}
```



### PROTOTIPI
```c
Pos *get_snake_head(Snake *snake);
Pos peak_next_pos(Snake *snake, Dir new_dir);
int allow_snake_movement(int manual);
void move_snake(Game *game, Dir new_dir, int manual);
```