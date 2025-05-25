#ifndef RANKING_H
#define RANKING_H

#define MAX_NAME_LEN 50
#define MAX_RANKINGS 100
#define RANKING_FILE "ranking.txt"

typedef struct {
    char nome[MAX_NAME_LEN];
    int pontos;
} RankingEntry;

int carregar_ranking(RankingEntry ranking[], int max_entries);
int salvar_ranking(RankingEntry ranking[], int count);
void adicionar_ao_ranking(RankingEntry ranking[], int *count, const char *nome, int pontos);
void ordenar_ranking(RankingEntry ranking[], int count);
void exibir_ranking(const RankingEntry ranking[], int count);

#endif