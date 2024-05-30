
# Base
A partire dal file 01-base_p.c creare un metodo wrapper di nome `scp` che ci consentono di gestire tutte quelle situzioni come per le funzioni  `SDL_CreateWindow`, `SDL_CreateRenderer` che restitiscono un puntatore `NULL` in caso di errore e quindi dobbiamo intercettare questo caso stampando un messaggio sulla console usando anche la funzione `SDL_GetError`.

Creare un metodo wrapper di nome `ssc` anche per la funzione `SDL_SetRenderDrawColor` che restituisce un valore diverso da 0 in caso di errore.




