# Tris: Vincitore
A partire dal file `03-loop.cpp` creare un file `04-winner.cpp` in cui si determina il vincitore (chi ha 3 simboli uguali allineati in verticale, orizzontale o diagonale) oppure il pareggio, quest'ultimo si veririfica se nessuno ha vinto e se la griglia è piena.

Per risolvere il problema implementare le seugenti funzioni
```c++
bool grigliaPiena(char griglia[SIZE][SIZE]);
bool verificaVittoria(char griglia[SIZE][SIZE], char simbolo);
```