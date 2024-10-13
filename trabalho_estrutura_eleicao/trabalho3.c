#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"

int main() {
    int numeroEleitores, totalChapas = 0, votosBrancos = 0, votosNulos = 0, votosValidos = 0;
    FILE* boletim_primeiro_turno = fopen("boletim_primeiro_turno.txt", "w");
    if (boletim_primeiro_turno == NULL) {
        printf("Erro ao abrir o arquivo boletim_primeiro_turno.txt\n");
        return 1;
    }

    printf("Digite o numero de eleitores da cidade: ");
    scanf("%d", &numeroEleitores);
    int totalEleitores = numeroEleitores;

    Chapa chapas[MAX_CHAPAS];

    cadastrarChapas(chapas, MAX_CHAPAS, &totalChapas);

    votar(chapas, totalChapas, &votosBrancos, &votosNulos, &votosValidos);

    salvarBoletim(boletim_primeiro_turno, chapas, totalChapas, votosBrancos, votosNulos, votosValidos, totalEleitores);

    verificarResultado(chapas, totalChapas, votosValidos, totalEleitores);

    return 0;
}
