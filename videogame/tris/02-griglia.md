# Tris: griglia
A partire dal codice 01-rand.cpp creare un nuovo programma 02-griglia.cpp che disegna una griglia per il tris in formato testo. La griglia è disegnata attraverso una funzione con la seguente firma
```c++
void disegnaGriglia(char griglia[SIZE][SIZE]);
```
dove `SIZE` è un costante definita per il preprocessore e che vale `3` 

la matrice griglia è una matrice che per ogni elemento potrà avere solo uno tra i seguenti caratteri
* `-` posizione vuota
* `O` posizione occupata dal giocatore 1
* `X` posizione occupata dal giocatore 2

Il programma nel nostro caso disegnerà una matrice 3 x 3 tutta vuota (con gli elementi seprati da tabulazioni) ovvero
```
-   -   -
-   -   -
-   -   -
```