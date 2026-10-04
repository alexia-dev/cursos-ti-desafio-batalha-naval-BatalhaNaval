#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM 8
#define NAVIOS 3
#define AGUA '~'
#define NAVIO '#'
#define ACERTO 'X'
#define ERRO 'o'

typedef struct { int linha, coluna, tamanho, vertical, restante; } Navio;

static char jogador[TAM][TAM];
static char inimigo[TAM][TAM];
static char visivel[TAM][TAM];
static Navio frota[NAVIOS];

void limpar(char t[TAM][TAM]) {
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++) t[i][j] = AGUA;
}

void mostrar_tabuleiro(char t[TAM][TAM], int esconder) {
    printf("\n      A B C D E F G H\n");
    printf("    +-----------------+\n");
    for (int i = 0; i < TAM; i++) {
        printf(" %d  | ", i + 1);
        for (int j = 0; j < TAM; j++) {
            char c = t[i][j];
            if (esconder && c == NAVIO) c = AGUA;
            if (c == ACERTO) printf("\033[1;31m%c\033[0m ", c);
            else if (c == ERRO) printf("\033[1;36m%c\033[0m ", c);
            else if (c == NAVIO) printf("\033[1;33m%c\033[0m ", c);
            else printf("%c ", c);
        }
        printf("|\n");
    }
    printf("    +-----------------+\n");
}

int dentro(int l, int c) { return l >= 0 && l < TAM && c >= 0 && c < TAM; }

int pode_colocar(char t[TAM][TAM], int l, int c, int tam, int vertical) {
    for (int k = 0; k < tam; k++) {
        int x = l + (vertical ? k : 0);
        int y = c + (vertical ? 0 : k);
        if (!dentro(x, y) || t[x][y] != AGUA) return 0;
    }
    return 1;
}

void colocar(char t[TAM][TAM], int l, int c, int tam, int vertical) {
    for (int k = 0; k < tam; k++) {
        int x = l + (vertical ? k : 0);
        int y = c + (vertical ? 0 : k);
        t[x][y] = NAVIO;
    }
}

void posicionar_cpu(char t[TAM][TAM]) {
    int tamanhos[NAVIOS] = {3, 2, 2};
    for (int n = 0; n < NAVIOS; n++) {
        int ok = 0;
        while (!ok) {
            int l = rand() % TAM, c = rand() % TAM, v = rand() % 2;
            if (pode_colocar(t, l, c, tamanhos[n], v)) {
                colocar(t, l, c, tamanhos[n], v);
                ok = 1;
            }
        }
    }
}

void posicionar_jogador() {
    int tamanhos[NAVIOS] = {3, 2, 2};
    for (int n = 0; n < NAVIOS; n++) {
        int l, c, v;
        char coluna;
        while (1) {
            system("clear");
            printf("\033[1;35m\n=== BATALHA NAVAL ===\033[0m\n");
            mostrar_tabuleiro(jogador, 0);
            printf("\nNavio %d/%d — tamanho %d\n", n + 1, NAVIOS, tamanhos[n]);
            printf("Digite coordenada inicial (ex.: A1) e orientação (H/V): ");
            if (scanf(" %c%d %c", &coluna, &l, &v) != 3) {
                while (getchar() != '\n');
                continue;
            }
            c = coluna - 'A';
            l--;
            if ((v == 'v' || v == 'V')) v = 1;
            else if ((v == 'h' || v == 'H')) v = 0;
            else continue;

            if (pode_colocar(jogador, l, c, tamanhos[n], v)) {
                colocar(jogador, l, c, tamanhos[n], v);
                break;
            }
            printf("Posição inválida. Pressione ENTER...");
            getchar(); getchar();
        }
    }
}

int restante(char t[TAM][TAM]) {
    int total = 0;
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            if (t[i][j] == NAVIO) total++;
    return total;
}

void atacar_cpu() {
    int l, c;
    do { l = rand() % TAM; c = rand() % TAM; } while (visivel[l][c] == ACERTO || visivel[l][c] == ERRO);

    if (jogador[l][c] == NAVIO) {
        jogador[l][c] = ACERTO;
        visivel[l][c] = ACERTO;
        printf("\n\033[1;31m🤖 CPU acertou %c%d!\033[0m\n", 'A' + c, l + 1);
    } else {
        jogador[l][c] = ERRO;
        visivel[l][c] = ERRO;
        printf("\n\033[1;36m🤖 CPU errou em %c%d.\033[0m\n", 'A' + c, l + 1);
    }
}

int main(void) {
    srand((unsigned)time(NULL));
    limpar(jogador);
    limpar(inimigo);
    limpar(visivel);

    printf("\033[1;35m\n╔══════════════════════════════════╗\n");
    printf("║        BATALHA NAVAL • C         ║\n");
    printf("╚══════════════════════════════════╝\033[0m\n");
    printf("Seu objetivo: afundar os 3 navios inimigos.\n");
    printf("Você terá um tabuleiro 8×8 e três navios.\n");
    printf("\nPressione ENTER para posicionar sua frota...");
    getchar();

    posicionar_jogador();
    posicionar_cpu(inimigo);

    while (restante(jogador) > 0 && restante(inimigo) > 0) {
        char coluna;
        int linha, c;

        system("clear");
        printf("\033[1;35m=== BATALHA NAVAL ===\033[0m\n");
        printf("Sua frota restante: %d casas | Inimigo: %d casas\n", restante(jogador), restante(inimigo));
        printf("\n\033[1;33mSEU TABULEIRO\033[0m");
        mostrar_tabuleiro(jogador, 0);
        printf("\n\033[1;31mRADAR INIMIGO\033[0m");
        mostrar_tabuleiro(inimigo, 1);

        printf("\nAtaque uma coordenada (ex.: D5): ");
        if (scanf(" %c%d", &coluna, &linha) != 2) {
            while (getchar() != '\n');
            continue;
        }
        c = coluna - 'A';
        linha--;

        if (!dentro(linha, c)) {
            printf("Coordenada inválida. Pressione ENTER...");
            getchar(); getchar();
            continue;
        }

        if (inimigo[linha][c] == ACERTO || inimigo[linha][c] == ERRO) {
            printf("Você já atacou essa casa. Pressione ENTER...");
            getchar(); getchar();
            continue;
        }

        if (inimigo[linha][c] == NAVIO) {
            inimigo[linha][c] = ACERTO;
            printf("\033[1;31m\n💥 ACERTO! Você atingiu um navio!\033[0m\n");
        } else {
            inimigo[linha][c] = ERRO;
            printf("\033[1;36m\n🌊 Água!\033[0m\n");
        }

        if (restante(inimigo) > 0) {
            atacar_cpu();
        }
        printf("\nPressione ENTER para continuar...");
        getchar(); getchar();
    }

    system("clear");
    if (restante(inimigo) == 0) {
        printf("\033[1;32m\n🏆 VITÓRIA! Você afundou toda a frota inimiga!\n\033[0m");
    } else {
        printf("\033[1;31m\n💥 DERROTA! A CPU afundou sua frota.\n\033[0m");
    }
    printf("\nObrigado por jogar Batalha Naval.\n");
    return 0;
}
