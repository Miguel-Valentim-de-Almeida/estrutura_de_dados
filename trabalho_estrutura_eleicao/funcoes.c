#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "funcoes.h"

void limparTela() {
    sleep(2);
    system("cls");
}

void cadastrarChapas(Chapa *chapas, int maxChapas, int *totalChap) {
    int continuar;
    printf("CADASTRAR CHAPA (0 para sair):\n");
    do {
        Chapa novaChapa;
        printf("Nome do candidato: ");
        while (getchar() != '\n');
        fgets(novaChapa.nomeCandidato, 51, stdin);
        novaChapa.nomeCandidato[strcspn(novaChapa.nomeCandidato, "\n")] = 0;

        int numeroDuplicado;
        do {
            numeroDuplicado = 0;
            printf("Numero do candidato (1-99): ");
            scanf("%d", &novaChapa.numeroCandidato);
            while (novaChapa.numeroCandidato < 1 || novaChapa.numeroCandidato > 99) {
                printf("Numero do candidato invalido. Insira novamente (1 - 99): ");
                scanf("%d", &novaChapa.numeroCandidato);
            }

            for (int i = 0; i < *totalChap; i++) {
                if (chapas[i].numeroCandidato == novaChapa.numeroCandidato) {
                    numeroDuplicado = 1;
                    printf("Ja existe um candidato com o numero %d. Insira novamente.\n", novaChapa.numeroCandidato);
                    break;
                }
            }
        } while (numeroDuplicado);

        printf("Data de nascimento (dia mes ano): ");
        scanf("%d %d %d", &novaChapa.dataNascimento[0], &novaChapa.dataNascimento[1], &novaChapa.dataNascimento[2]);

        printf("Nome do vice: ");
        while (getchar() != '\n');
        fgets(novaChapa.nomeVice, 51, stdin);
        novaChapa.nomeVice[strcspn(novaChapa.nomeVice, "\n")] = 0;        novaChapa.votos = 0;
        chapas[*totalChap] = novaChapa;
        (*totalChap)++;
        printf("Deseja continuar?\n[1] Sim\n[0] Nao\nR: ");
        scanf("%d", &continuar);
    } while ((*totalChap < maxChapas) && continuar != 0);
}


void exibirCandidatos(Chapa *chapas, int totalChapas) {
    printf("Candidatos disponiveis:\n");
    for (int i = 0; i < totalChapas; i++) {
        printf("%d - %s com vice %s\n", chapas[i].numeroCandidato, chapas[i].nomeCandidato, chapas[i].nomeVice);
    }
}

void votar(Chapa *chapas, int totalChapas, int *votosBrancos, int *votosNulos, int *votosValidos) {
    int voto;
    while (1) {
        exibirCandidatos(chapas, totalChapas);
        printf("Digite o numero do candidato (0 para voto em branco, -1 para encerrar): ");
        scanf("%d", &voto);

        if (voto == -1) {
            break;
        }

        if (voto == 0) {
            (*votosBrancos)++;
            printf("Seu voto em branco foi confirmado!\n");
        } else {
            int candidatoValido = 0;
            for (int i = 0; i < totalChapas; i++) {
                if (chapas[i].numeroCandidato == voto) {
                    chapas[i].votos++;
                    candidatoValido = 1;
                    (*votosValidos)++;
                    printf("Seu voto em %s com vice %s foi confirmado!\n", chapas[i].nomeCandidato, chapas[i].nomeVice);
                    break;
                }
            }
            if (candidatoValido == 0) {
                (*votosNulos)++;
                printf("Seu voto nulo foi confirmado!\n");
            }
        }
        limparTela();
    }
}

void salvarBoletim(FILE *arquivo, Chapa *chapas, int totalChapas, int votosBrancos, int votosNulos, int votosValidos, int totalEleitores) {
    int totalVotos = votosBrancos + votosNulos + votosValidos;

    fprintf(arquivo, "BOLETIM DE URNA\n");
    for (int i = 0; i < totalChapas; i++) {
        double percentualChapa = (double)chapas[i].votos / totalEleitores * 100;
        fprintf(arquivo, "%s (%d) - Votos: %d (%.2f%% dos eleitores)\n",
                chapas[i].nomeCandidato, chapas[i].numeroCandidato, chapas[i].votos, percentualChapa);
    }

    fprintf(arquivo, "Votos Brancos: %d\n", votosBrancos);
    fprintf(arquivo, "Votos Nulos: %d\n", votosNulos);
    fprintf(arquivo, "Votos Válidos: %d\n", votosValidos);
    fprintf(arquivo, "Total de Votos: %d\n", totalVotos);
    fprintf(arquivo, "Porcentagem de Votos Válidos: %.2f%%\n", (double)votosValidos / totalVotos * 100);
    fprintf(arquivo, "Porcentagem de Votos Totais: %.2f%%\n", (double)totalVotos / totalEleitores * 100);
    fprintf(arquivo, "Porcentagem de Votos Brancos e Nulos: %.2f%%\n", (double)(votosBrancos + votosNulos) / totalVotos * 100);

    fclose(arquivo);
}


void verificarResultado(Chapa *chapas, int totalChapas, int votosValidos, int totalEleitores) {
    int candidato1 = 0;
    FILE *boletim_primeiro_turno = fopen("boletim_primeiro_turno.txt", "a");
    if (boletim_primeiro_turno == NULL) {
        printf("Erro ao abrir o arquivo boletim_primeiro_turno.txt\n");
        return;
    }
    for (int i = 1; i < totalChapas; i++) {
        if (chapas[i].votos > chapas[candidato1].votos) {
            candidato1 = i;
        }
    }

    if (chapas[candidato1].votos > (votosValidos / 2)) {
        printf("Candidato %s ganhou no primeiro turno!\n", chapas[candidato1].nomeCandidato);
        fprintf(boletim_primeiro_turno, "Nao havera segundo turno");
    } else {
        fprintf(boletim_primeiro_turno, "Havera segundo turno");
        int votosSegundoLugar = -1;
        int candidatosSegundoLugar[MAX_CHAPAS];
        int totalSegundoLugar = 0;

        for (int i = 0; i < totalChapas; i++) {
            if (i != candidato1 && (votosSegundoLugar == -1 || chapas[i].votos >= votosSegundoLugar)) {
                if (chapas[i].votos > votosSegundoLugar) {
                    totalSegundoLugar = 0;
                    votosSegundoLugar = chapas[i].votos;
                }
                candidatosSegundoLugar[totalSegundoLugar++] = i;
            }
        }

        int candidato2 = candidatosSegundoLugar[0];
        if (totalSegundoLugar > 1) {
            for (int i = 1; i < totalSegundoLugar; i++) {
                int idxAtual = candidatosSegundoLugar[i];
                if (chapas[idxAtual].dataNascimento[0] < chapas[candidato2].dataNascimento[0] ||
                    (chapas[idxAtual].dataNascimento[0] == chapas[candidato2].dataNascimento[0] && chapas[idxAtual].dataNascimento[1] < chapas[candidato2].dataNascimento[1]) ||
                    (chapas[idxAtual].dataNascimento[0] == chapas[candidato2].dataNascimento[0] && chapas[idxAtual].dataNascimento[1] == chapas[candidato2].dataNascimento[1] && chapas[idxAtual].dataNascimento[2] < chapas[candidato2].dataNascimento[2])) {
                    candidato2 = idxAtual;
                }
            }
            printf("Empate no segundo lugar. %s eh o mais velho e avanca.\n", chapas[candidato2].nomeCandidato);
        }
        printf("Havera segundo turno entre %s e %s.\n", chapas[candidato1].nomeCandidato, chapas[candidato2].nomeCandidato);

        for (int i = 0; i < totalChapas; i++) {
            if (i == candidato1 || i == candidato2) {
                chapas[i].votos = 0;
            }
        }

        int votosBrancos = 0, votosNulos = 0, votosValidosSegundoTurno = 0;
        votar(chapas + candidato1, 2, &votosBrancos, &votosNulos, &votosValidosSegundoTurno);

        if (chapas[candidato1].votos > chapas[candidato2].votos) {
            printf("Candidato %s ganhou no segundo turno!\n", chapas[candidato1].nomeCandidato);
        } else {
            printf("Candidato %s ganhou no segundo turno!\n", chapas[candidato2].nomeCandidato);
        }

        FILE *boletim_segundo_turno = fopen("boletim_segundo_turno.txt", "w");
        if (boletim_segundo_turno != NULL) {
            salvarBoletim(boletim_segundo_turno, chapas, totalChapas, votosBrancos, votosNulos, votosValidosSegundoTurno, totalEleitores);
        } else {
            printf("Erro ao abrir o arquivo boletim_segundo_turno.txt\n");
        }
        fclose(boletim_segundo_turno);
    }
    fclose(boletim_primeiro_turno);
}
