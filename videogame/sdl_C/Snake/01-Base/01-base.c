
//Using SDL and standard IO

#include <stdio.h>
#include <SDL2/SDL.h>


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// PROTOTIPI
void scc(int code);
void *scp(void *ptr); 

int main( int argc, char* args[] )
{
	SDL_Init(SDL_INIT_VIDEO);

    // centra la finestra sullo schermo (ricordiamo che 0, 0 è la posizione in alto a sinistra dello schermo
    // e sono le coordinate del vertice in alto a sinistra della finestra)
    SDL_Window* const window = scp(SDL_CreateWindow("SDL Tutorial",  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE));

    SDL_Renderer *const renderer = scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));

    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    int quit = 0; // entro nel ciclo la prima volta
    while(!quit)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                quit = 1;
            }
        }
        
        int code = SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 255); // nera opaca
        if (code != 0) {
            printf("Error initializing SDL: %s\n", SDL_GetError());
            exit(1);
        }
        SDL_RenderClear(renderer);
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
