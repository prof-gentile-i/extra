# Gestione dei colori

## Compilare ed eseguire il programma
Il modo migliore è creare il makefile e usare il programma `make` e poi dopo eseguendo il programma (`&&` esegue due programmi in sequenza) con una delle seguenti (a seconda se siamo su un terminale PowerShell o CMD) da scrivere a partire dalla directory root dove si trova il makefile
```
mingw32-make SRC_FILE=./completa/07-colors.c; .\completa\07-colors.exe
mingw32-make SRC_FILE=./completa/07-colors.c && .\completa\07-colors.exe
```
in alternativa si può indicare la seguente riga di compilazione da scrivere nella directory dove si trova il file sorgente
```
gcc -std=c17 07-colors.c  -Wall -lmingw32 -lSDL2main -lSDL2 -o 07-colors
```

Ovviamente nel caso della soluzione parziale (se è compilabile) sostituire `07-colors` con `07-colors_p`
```
mingw32-make SRC_FILE=./parziale/07-colors_p.c; .\parziale\07-colors_p.exe
mingw32-make SRC_FILE=./parziale/07-colors_p.c && .\parzile\07-colors_p.exe

```
in alternativa si può indicare la seguente riga di compilazione
```
gcc -std=c17 07-colors_p.c;  -Wall -lmingw32 -lSDL2main -lSDL2 -o 07-colors_p
```

# Definire costanti e macro per i colori
Per la gestione dei colori di sfondo, dello snake, del testo (che useremo per visualizzare lo score), del cibo, etc., definiamo delle costanti in termini di 4 byte esadecimali (che rappresentano Red, Green, Blu, Alpha)
```c
#define BACKGROUND_COLOR    0x000000FF
#define BOARD_COLOR         0xFFFFFF00
#define SNAKE_COLOR         0xEE72F100
#define FOOD_COLOR          0x77B28C00
#define OBSTACLE_COLOR      0x964B0000
#define TEXT_COLOR          0xFFFFFF00 
```

Nelle funzioni di redering abbiamo chiamate del tipo
```c
SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0)
```
in cui i colori e il canale alpha non vanno dati tutti insieme ma singolarmente (possono essere dati anche in valore esadecimale scrivendo per esempio `0xFF`, `0xFF`, `0xFF`, `0x00`); allora ci serve una funzione che prende un numero  dato `8` valori esadecimali (cioè `32` bit) e la strasformi in 4 numeri esacimali da due cifre (4 volte 8 bit) separati da virgole; per farlo usiamo la macro seguente
```c
#define HEX_COLOR(hex) \
    ((hex) >> (3 * 8)) & 0xFF, \
    ((hex) >> (2 * 8)) & 0xFF, \
    ((hex) >> (1 * 8)) & 0xFF, \
    ((hex) & 0xFF)
```

## Aggiornare le funzioni di rendering con la macro HEX_COLOR
A questo punto usiamo la macro `HEX_COLOR` in questa cartella
```c
scc(SDL_SetRenderDrawColor(renderer, HEX_COLOR(BOARD_COLOR))); // in render_board
scc(SDL_SetRenderDrawColor(renderer, HEX_COLOR(SNAKE_COLOR))); // in render_snake
scc(SDL_SetRenderDrawColor(renderer, HEX_COLOR(FOOD_COLOR))); // in render_food
scc(SDL_SetRenderDrawColor(renderer, HEX_COLOR(OBSTACLE_COLOR))); // in render_obstacles
scc(SDL_SetRenderDrawColor(renderer, HEX_COLOR(BACKGROUND_COLOR))); // in remove_food
```

## Nascondere la board
Sfruttando i colori realizzati nascondere la griglia 
