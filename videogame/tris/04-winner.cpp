#include <iostream>
#include <cstdlib> // Libreria per srand() e rand()
#include <ctime>   // Libreria per time()

#define SIZE 3

void disegnaGriglia(char griglia[SIZE][SIZE]);
bool mossaValida(char griglia[SIZE][SIZE], int riga, int colonna);
bool verificaVittoria(char griglia[SIZE][SIZE], char simbolo);
bool grigliaPiena(char griglia[SIZE][SIZE]);

int main() {
    // Inizializzare il generatore di numeri casuali
    std::srand(std::time(0));

    // Generare un numero casuale, 0 o 1 per determinare chi inizia
    int giocatore = std::rand() % 2 + 1;

    // Inizializzare la griglia
    char griglia[SIZE][SIZE] = {
        {'-', '-', '-'},
        {'-', '-', '-'},
        {'-', '-', '-'}
    };

    // Loop del gioco
    while (true) {
        // Disegna la griglia
        disegnaGriglia(griglia);

        // Determinare il simbolo del giocatore corrente
        char simbolo = (giocatore == 1) ? 'O' : 'X';

        // Chiedere al giocatore di fare una mossa
        int riga, colonna;
        std::cout << "Giocatore " << giocatore << " (" << simbolo << "): inserisci riga e colonna separate da spazio (valori da 1 a 3): ";
        std::cin >> riga >> colonna;

        // Controllare se la mossa è valida
        if (mossaValida(griglia, riga, colonna)) {
            griglia[riga-1][colonna-1] = simbolo;

            // Controllare se il giocatore corrente ha vinto
            if (verificaVittoria(griglia, simbolo)) {
                disegnaGriglia(griglia);
                std::cout << "Giocatore " << (giocatore) << " (" << simbolo << ") ha vinto!" << std::endl;
                break;
            }


            // Controllare se la griglia è piena
            if (grigliaPiena(griglia)) {
                disegnaGriglia(griglia);
                std::cout << "La griglia è piena. Partita terminata in pareggio." << std::endl;
                break;
            }

            // Alternare il giocatore
            giocatore = 3 - giocatore;

        } else {
            std::cout << "Mossa non valida. Riprova." << std::endl;
        }
    }

    return 0;
}

void disegnaGriglia(char griglia[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            std::cout << griglia[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}

bool mossaValida(char griglia[SIZE][SIZE], int riga, int colonna) {
    if (riga < 1 || riga > SIZE || colonna < 1 || colonna > SIZE) {
        return false;
    }
    return griglia[riga-1][colonna-1] == '-';
}

bool verificaVittoria(char griglia[SIZE][SIZE], char simbolo) {
    // Controlla righe e colonne
    for (int i = 0; i < SIZE; ++i) {
        if ((griglia[i][0] == simbolo && griglia[i][1] == simbolo && griglia[i][2] == simbolo) || // Riga i 
            (griglia[0][i] == simbolo && griglia[1][i] == simbolo && griglia[2][i] == simbolo)) { // Colonna i
            return true;
        }
    }

    // Controlla diagonali
    if ((griglia[0][0] == simbolo && griglia[1][1] == simbolo && griglia[2][2] == simbolo) || // Diagonale principale
        (griglia[0][2] == simbolo && griglia[1][1] == simbolo && griglia[2][0] == simbolo)) { // Diagonale secondaria
        return true;
    }

    return false;
}

bool grigliaPiena(char griglia[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (griglia[i][j] == '-') {
                return false;
            }
        }
    }
    return true;
}

