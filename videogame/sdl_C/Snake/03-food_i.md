# Gestione del food (cibo)


## Strutture e costanti per il cibo
Definire le costanti:
  * tempo (in secondi) di ritardo tra l'apparizione casuale di cibo e un'altra
  * il numero di cibi da far apparire
```c
    #define DELAY_FOOD_SPAWN 3
    #define FOODS_COUNT 1
```

Ora definiamo alcune strutture
  * `Pos` che rappresenta la posizione sulla board (quindi `x` e `y` non sono pixel ma posizioni che partiranno da `0` e arriveranno rispettivamente a `N_CELLS_H`  e `N_CELLS_V`)
  * `Food` cibo (il singolo blocchetto di cibo)
```c
    typedef struct {
        int x;
        int y;
    } Pos;

    typedef struct {
        Pos pos;
        int score; // Punteggio che si ottiene mangiando questo cibo
    } Food;
```

## Inserimento del cibo nel gioco
Nella strttura `Game` inseriamo l'array di tutti i cibi presenti nel nostro gioco
```c
    typedef struct {
        Food food[FOODS_COUNT]; // NEW
        int quit;
    } Game;
```

Inizializzazione nella funzione `init_game` anche del cibo attraverso la funzione `init_food`
```c
    void init_game(Game *game) {
        game->quit = 0;
        init_food(game); // NEW
    }
```
Dove la funzione `init_food` setta per ogni food lo score a un certo punteggio diciamo `1` e la posizione come posizione randomica libera del piano (quella in cui non ci saranno né il serpente ne gli ostacoli)
```c    
    void init_food(Game *game) {
        for(int i = 0; i < FOODS_COUNT; i++) {
            game->food[i].score = 1;
            game->food[i].pos = random_empty_board_pos(game); // Funzione da definire
        }
        return; 
    }
```
La funzione per il calcolo di una posizione libera del piano è 
```c
    Pos random_empty_board_pos(Game *game) {
        Pos p = {0}; // Struct vuota
        // Genero posizioni casuali finchè non trovo una posizione vuota
        do {
            p = random_board_pos();
        } while(pos_is_not_empty(game, p));

        return p;
    }
```
La funzione di sopra sfrutta la funzione `pos_is_not_empty` che data una posizione `p` restituisce `1` se è occupata (da un altro cibo, dal serpente o dall'ostacolo) e `0` altrimenti
```c
    int pos_is_not_empty(Game *game, Pos p) {
        
        // ** SE p è OCCUPATA DAL CIBO RESTITUIRE 1

        // TODO: controllare se la posizione è occupata dal serpente
        // TODO: controllare se la posizione è occupata da un ostacolo
        return 0;
    }
```
La funzione `random_board_pos` genera causalmente una posizione nella board
```c
    Pos random_board_pos(void) {
        Pos p = {0};
        
        // ** GENERARE UNA POSIZIONE RANDOMICA IN UNA CASELLA DELLA BOARD

        return p;
    }
```

## Rendering del food
Creaimo la funzione `render_food` che visualizza il cibo in punto casuale della board, di seguito il prototipo e la definizione (che per ora disegniamo bianco). Il rettagolo è una struttura `SDL_Rect` che richeide 4 parametri
  * ascissa x del vertice in alto a sinistra del rettangolo (in pixel)
  * ascissa y del vertice in alto a sinistra del rettangolo (in pixel)
  * dimensione (in pixel) orizzontale del rettangolo
  * dimensione (in pixel) verticale del rettangolo

```c
    render_food(renderer, game); // anche game perché le info sul cibo stanno nella struttura del nostro gioco

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
```
Poi chiaramente modifichiamo `render_game` aggiungendo anche la renderizzazione del food
```c
    void render_game(SDL_Renderer * renderer, Game * game) {
        render_board(renderer);
        render_food(renderer, game); // NEW: anche game perché le info sul cibo stanno nella struttura del nostro gioco
    }
```

Gestiamo ora il cambiamento di posizione del Food ogni `DELAY_FOOD_SPAWN` secondi. Tale funzione è `update_food` e funziona in questo modo: se è passato il giusto tempo rimuove il food e lo crea in altra posizione
```c
    void update_food(SDL_Renderer * renderer, Game *game) {
        if(allow_refresh_food()) {
            remove_food(renderer, game);
            init_food(game);
        }
        return;
    }
```
La funzione di `remove_food` funziona colorandolo dello stesso colore dello sfondo (nero). Ovviamente rimuoviamo quelli visualizzati ovvero con `score` diverso da zero.
```c
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
```
Mentre la funzione `allow_refresh_food` sfrutta variaibli statiche che essendo definite in un sottoprogramma **mantengono il valore tra una chiamata e l'altra**. 

Prima di vedere come è fatta vediamo la struttura  `timeval` è definita nella libreria di sistema <sys/time.h>. Ecco come è strutturata:
```c
    struct timeval {
        time_t      tv_sec;     // Numero di secondi
        suseconds_t tv_usec;    // Numero di microsecondi
    };
```
  * `tv_sec`: È un campo di tipo `time_t`, che rappresenta il numero di secondi trascorsi dall'Unix epoch (1° gennaio 1970).
  * `tv_usec`: È un campo di tipo `useconds_t`, che rappresenta il numero di microsecondi trascorsi nell'intervallo tra un secondo e il successivo. Può variare da `0` a `999999`.


_Dare una spiegazione della seguente funzione_
```c
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
```
Ovviamente poi vanno aggiunti tutti i prototipi delle funzioni create.

# Il main
Nel main va aggiunta la funzione `update_food`