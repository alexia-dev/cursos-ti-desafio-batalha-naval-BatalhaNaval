#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif

int main(void) {
    printf("\n=== BATALHA NAVAL | GUI MINIMA ===\n\n");
    printf("+---+---+---+---+---+---+---+---+\n");
    for (int r = 0; r < 8; r++) {
        printf("|");
        for (int c = 0; c < 8; c++) printf(" %c |", '.');
        printf("\n+---+---+---+---+---+---+---+---+\n");
    }
    printf("\nDigite coordenadas como A1 para jogar.\n");
    return 0;
}
