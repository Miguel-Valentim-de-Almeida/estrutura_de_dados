#ifndef FUNCOES_H
#define FUNCOES_H

#define MAX_CHAPAS 99
#define MIN_ELEITORES_SEGUNDO_TURNO 10

typedef struct chapa {
    char nomeCandidato[51];
    int numeroCandidato;
    int dataNascimento[3];
    char nomeVice[51];
    int votos;
} Chapa;

void limparTela();
void cadastrarChapas(Chapa *chapas, int maxChapas, int *totalChapas);
void exibirCandidatos(Chapa *chapas, int totalChapas);
void votar(Chapa *chapas, int totalChapas, int *votosBrancos, int *votosNulos, int *votosValidos);
void salvarBoletim(FILE *arquivo, Chapa *chapas, int totalChapas, int votosBrancos, int votosNulos, int votosValidos, int totalEleitores);
void verificarResultado(Chapa *chapas, int totalChapas, int votosValidos, int totalEleitores);

#endif
