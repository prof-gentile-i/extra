# Managiare il food, crescita snake, aumento score 

## Compilare ed eseguire il programma
```
gcc -std=c17 05-eatfood.c  -Wall -lmingw32 -lSDL2main -lSDL2 -o 05-eatfood
```
## Aggiornamento delle strutture dati
Quando mangieremo un food amenterà anche il punteggio del nostro gioco quindi introduciamo la variabile `global_score`
```c
typedef struct {
    Snake snake;
    Food food[FOODS_COUNT];
    double game_speed; 
    int quit;
    int global_score; // NEW 
} Game;
```
## Aggiornamento del main
Una volta che snake si è mosso dobbiamo vedere se con la sua testa tocca un food in tal caso lo mangia, cresce di dimensione e la velocità del gioco aumenta, quindi nel main scriviamo
```c
       // ... dopo aver individuato l'evento
       // main logic loop
        move_snake(&game, game.snake.dir, 0); // automatic movement
        
        Food * f = check_for_food(&game);
        if (f) { // Se f è diverso da NULL
            eat_food(&game, f);
            update_game_speed(&game);
        }
        update_food(renderer, &game);
        // ... prima di render_game
```

## Contollo se la testa dello snake impatta uno dei food
La funzione `check_for_food` controlla se la testa dello snake impatta uno dei food del game (ovviamente food che hanno uno score > 0) in tal caso restituisce un puntatore al food interecettato altrimenti restituisce null
```c
Food * check_for_food(Game *game) {
    Snake *snake = &game->snake;
    Pos head_pos = *get_snake_head(snake);
    
    for (int i = 0; i < FOODS_COUNT; i++) {
       Food *f = &game->food[i];
        if (/* TODO: Condizione: food coincide con la testa del snake e food ha score > 0*/ ) {
            return f;
        }
    }
    return NULL;

}
```

## Mangiare il food

Mangiare in food signfica:
  * Aumentare lo score globale del punteggio pari a quello del food (per il momento tutti i food hanno punteggio 1)
  * mettere il punteggio del food a zero
  * far crescere di una dimensione lo snake che si fa in questo modo
    - calcolare la successiva posizione `new_pos` dello snake
    - aumentare la sua lunghezza
    - la nuova testa diventa `new_pos`

```c
void eat_food(Game *game, Food *f) {
    Snake *snake = &game->snake;

    game->global_score += f->score;
    f->score = 0;
    
    // grow snake's body
    Pos new_pos = peak_next_pos(snake, snake->dir);
    /* TODO: 
        AUMENTARE LA GRANDEZZA DELLO SNAKE 
    */
    
    
}
```

## Aggiornare la velocità di snake

La velocità è regolata dal campo `game_speed` della struttura `Game` (che ricoaridamo a sua volta compare nel `update_snake_movement`). 
Aumentare la velocità significa diminuire il `game_speed` che parte dal valore `MAX_SNAKE_MOVEMENT` e gli viene sottratto un valore di nome `step_update` che è sempre più grande (al mangiare dei food). `step_update` è dato dal punteggio globale per lo `STEP_SNAKE_MOVEMENT`. Ovviamente non si può andare sotto `MIN_SNAKE_MOVEMENT`

```c
void update_game_speed(Game *game) {
    double step_update = game->global_score * STEP_SNAKE_MOVEMENT;
    if (MAX_SNAKE_MOVEMENT - step_update < MIN_SNAKE_MOVEMENT) {
        /*
            TODO: Valore del campo game_speed della struttura puntata da game 
        */
    }
    else {
        /*
            TODO: Valore del campo game_speed della struttura puntata da game 
        */
    }
    
}
```