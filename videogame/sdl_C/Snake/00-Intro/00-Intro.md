
## Compilare ed eseguire
Il modo migliore è creare il makefile e usare il programma make e poi dopo eseguendo il programma (&& esegue due programmi in sequenza) con una delle seguenti (a seconda se siamo su un terminale PowerShell o CMD)
```
mingw32-make; .\main.exe
mingw32-make && .\main.exe
```
in alternativa si può indicare la seguente riga di compilazione
```
gcc -std=c17 main.c  -Wall -lmingw32 -lSDL2main -lSDL2 -o main
```


## Il primo programma
```c
#include <stdio.h>
#include <SDL2/SDL.h>

int main(int argc, char* args[] ) {
    SDL_Init(SDL_INIT_VIDEO);
    
    printf("It Works\n");
    return 0;
}
```
