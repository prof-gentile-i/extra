# Tris: Loop di gioco
A partire dal file `02-griglia.cpp` creare un file `03-loop.cpp` in cui creiamo un ciclo infinito e in esso 
* Si disegna la griglia
* Si chiede una mossa alternativamente al giocatore 1 e al giocatore 2, con una domanda seguente (esempio per il giocatore 1, dove il suo simbolo è `O` mentre per quello 2 è `X`)
```
    Giocatore 1 (O): inserisci riga e colonna separate da spazio (valori da 1 a 3):
``` 
* si crea una funzione di nome di nome `mossaValida` con il seguente prototipo
```c++
bool mossaValida(char griglia[SIZE][SIZE], int riga, int colonna);
```
che controlla se la mossa è valida ovvero si trova nella griglia (valori compresi tra `1` e `SIZE`) e non occupata già da un altro simbolo. Nel caso non valido richiede la ripetizione della mossa, mentre nel caso di mossa valida inserisce il simbolo `O` (per il giocatore 1) e `X` per il giorcatore (2) nella griglia.

Chiaramente il programma allo stato attuale non calcolando ancora il vincitore va terminato solo forzatamente.


