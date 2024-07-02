# Gesgtione degli ostacoli

Creare un nuovo progetto cartella di nome `06-Obstacles` in cui creare un file C di nome `06-obstacles.c` che inizialmente è pari a quello di `05-eatfood.c`. Ora lavoreremo sul queesto file `06-obstacles.c` per creare degli ostacoli e gestire il fatto che se lo snake impatta su essi o su stesso il gioco termina.

## Compilare ed eseguire il programma

Scrivere nella directory dove si trova il file sorgente
```
gcc -std=c17 06-obstacles.c  -Wall -lmingw32 -lSDL2main -lSDL2 -o 04-obstacles
```

## Definizione di costanti e strutture
Definiamo il nuero massimo di ostacoli (cioè quanti quadrati di ostacoli massimo vogliamo sulla board)
```c
#define MAX_OBSTACLES 20 
```
e la struttura che definisce l'osatcolo
```c
typedef struct {
    Pos pos;
    int init; // Vale 1 se è stato inizializzato
} Obstacle;
```
Nella struttura `Game` abbiamo anche gli ostacoli
```c
typedef struct {
    Snake snake;
    Food food[FOODS_COUNT];
    Obstacle obs[MAX_OBSTACLES]; // NEW
    double game_speed; 
    int quit;
    int global_score;
} Game;
```

## Inizializzazione degli ostacoli
Nella `init_game` dobbiamo posizionare gli ostacoli. Creiamo degli ostacoli fatti a croce greca, per farlo definiamo la seguente macro
```c
#define STAR_OBSTACLE(game, off, x, y) \
    (game)->obs[off++]  = (Obstacle) {.pos = {x    , y    }, .init = 1}; \
    (game)->obs[off++]  = (Obstacle) {.pos = {x + 1, y    }, .init = 1}; \
    (game)->obs[off++]  = (Obstacle) {.pos = {x - 1, y    }, .init = 1}; \
    (game)->obs[off++]  = (Obstacle) {.pos = {x    , y + 1}, .init = 1}; \
    (game)->obs[off++]  = (Obstacle) {.pos = {x    , y - 1}, .init = 1}; 
```
Poi ci creiamo un'altra macro per un ostacolo orizzontale 
```c
#define HORIZONTAL_WALL_OBSTACLE(game, off, x, y, length) \
    for(int i = 0; i < length; i++) { \
        (game)->obs[off++]  = (Obstacle) {.pos = {x + i, y}, .init = 1}; \
    }
```
e quindi in `init_game` chiamiamo queste due macro per creare 2 ostacoli a croce greca e uno orizzontale
```c
    int off = 0;
    STAR_OBSTACLE(game, off, 3, 3); // Ostacolo centrato su (3,3)
    STAR_OBSTACLE(game, off, 7, 7); // Ostacolo centrato su (7,7)
    HORIZONTAL_WALL_OBSTACLE(game, off, 10, 10, 5); // Muro orizzontale lungo tutta la prima riga
```

## Rendering degli obstacles
Per il rendering degli ostacoli creiamo la funzione `render_obstacles` in cui iteriamo su tutti gli ostacoli

```c
void render_obstacles(SDL_Renderer * renderer, Game * game) {
    scc(SDL_SetRenderDrawColor(renderer, 150, 75, 0, 0)); // colore marrone

    /* TODO: CREA UN CICLO FOR SU TUTTI GLI OSTACOLI, SE QUESTO é INIZIALIZZATO DISEGNA IL RETTANGOLO
    */

}
```

Tale funzione è chiamata in `render_game` 
```c
void render_game(SDL_Renderer * renderer, Game * game) {
    scc(SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0));
    SDL_RenderClear(renderer);
    
    render_board(renderer);
    render_snake(renderer, game);
    render_food(renderer, game); 
    render_obstacles(renderer, game); // NEW
    
    SDL_RenderPresent(renderer);
}
```
## Interazione tra snake o obstacles
Ora potremmo già compilare il nostro codice e vediamo che però il serpente passa nell'ostacolo senza che succede nulla invece noi dobbiamo far si che se tocca un ostacolo il gioco finisce.

Per gestire questa interazione nel `main` impostiamo la variaible `game.quit` come or tra il valore di `game.quit` stessa e una funzione `ceck_for_obstacols` che ci darà `1` se il serpente impatta l'ostacolo. Ci vuole la or perché `game.quit` può essere utile anche se accade qualche altro evento come il fatto che l'utente ha deciso di uscire e quindi io devo modificare il valore di `game.quit` mettendolo a `1` solo se non lo è già di per se (se lo è già di per se resta a `1`).
```c
        // main logic loop
        move_snake(&game, game.snake.dir, 0); // automatic movement
  
        // ** INIZIO NEW
        // La seguente è equivalente a 
        // game.quit = game.quit | check_for_obstacle(&game);
        game.quit |=  check_for_obstacle(&game);
        // ** FINE NEW

        Food * f = check_for_food(&game);
        if (f) { // Se f è diverso da NULL
            eat_food(&game, f);
            update_game_speed(&game);
        }
        update_food(renderer, &game);
        
        // rendering stuff
        render_game(renderer, &game);         
```

La funzione `check_for_obstacles` resituisce `1` che poi settando `game.quit` fa terminare il gioco se il serpente tocca il suo stesso corpo o tocca gli ostacoli
```c
// restituisce 1 se lo snake ha colpito un ostacolo o se ha colpito se stesso, 0 altrimenti
int check_for_obstacle(Game *game) {
    Snake *snake = &game->snake;
    Pos head_pos = *get_snake_head(snake);
    
    // ** TODO: controllo se la testa dello snake è sul proprio corpo (esclusa la testa stessa ovviamente)
    for(int i = 0; i < snake->length -2; i++) {
      // SCRIVERE CODICE QUI
    }
    // **

    // ** TODO: controllo se la testa dello snake è sopra un ostacolo
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        Obstacle o = game->obs[i];
       // SCRIVERE CODICE QUI
    }
    return 0;
    // ** 
}
```

## Il food deve evitare le posizioni di snake e obstacles
Possiamo ora completare il metodo `pos_not_empty` mettendo controllando che restituisce `1` anche se la posizione è occupata dal serpente o da un ostacolo
```c
int pos_is_not_empty(Game *game, Pos p) {
    // Controllo se la posizione è occupata da un cibo
    for(int i = 0; i < FOODS_COUNT; i++) {
        if(game->food[i].pos.x == p.x && game->food[i].pos.y == p.y) return 1;
    }

    // INIZIO NEW
    // ** TODO: Controllo se la posizione è occupata dal serpente
    // SCRIVI CODICE QUI
    // **
    // TODO: Controllo se la posizione è occupata da un ostacolo
    // SCRIVI CODICE QUI
    // FINE NEW

    return 0;
}
```
