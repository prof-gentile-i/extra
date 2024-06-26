# Disegno della griglia



## Definiamo le costanti della griglia
Supponiamo di voler disegnare 10 celle orizzontali e 10 verticali
```c
#define N_CELLS_H 10
#define N_CELLS_V 10
```
In base alla larghezza/altezza della finestra dobbiamo definire le seguenti costanti che ci dicono quanti pixel orizzontali e veriticali sarà una cella
```c
#define CELL_WIDTH // definisci...
#define CELL_HEIGHT //definisci ...
```

## Creazione della struct Game
Per gestire varie impostazioni del gioco utilizzeremo una struttura di nome `Game` che definiamo prima del main e che per ora ha un solo campo
```c
// STRUTTURE DATI
typedef struct {
    int quit;
} Game;
```c
Sempre prima del main creiamo una variaible (globale) di nome game
```c
// VAR GLOBALI
Game game = {0};
```

Inoltre prima del while degli eventi  inizializziamo una funzione di gioco
```c
init_game(&game);
```

la cui definizione per ora è 
```c
void init_game(Game *game) {
    game->quit = 0;
}
```

## Renderizzazione del gioco
Nel main è da mettere la funzione per il rendering del gioco 
```c
void render_game(SDL_Renderer * renderer, Game * game) {
    render_board(renderer);
}
```
che nel nostro caso renderizza solo la griglia di gioco.

La funzione `render_board` visualizza in bianco la griglia di gioco (ricordiamo che lo sfondo della finestra è nero quindi la griglia si vede)
```c
void render_board(SDL_Renderer * renderer){
    scc(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0));
    // Ricordiamo che in alto a sinistra è il punto di coordinate (x, y) = (0,0)
    // poi la x cresce verso destra e la y verso il basso
    
     // *** Usa la funzione SDL_RenderDrawLine per disegnare le linee verticali e orizzontali
    // *** fai due cicli for separati
}
```

Ovviamente prima del main aggiungiamo i prototipi
```c
void render_board(SDL_Renderer * renderer);
void render_game(SDL_Renderer * renderer,  Game * game);
```