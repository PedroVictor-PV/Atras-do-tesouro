#include <stdio.h>
#include <string.h>
#include "utils/ranking.h"

int carregar_ranking(RankingEntry ranking[], int max_entries) {
    FILE *file = fopen(RANKING_FILE, "r");
    if (!file) return 0;
    int count = 0;
    while (fscanf(file, "%49s %d", ranking[count].nome, &ranking[count].pontos) == 2 && count < max_entries) {
        count++;
    }
    fclose(file);
    return count;
}

int salvar_ranking(RankingEntry ranking[], int count) {
    FILE *file = fopen(RANKING_FILE, "w");
    if (!file) return 0;
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %d\n", ranking[i].nome, ranking[i].pontos);
    }
    fclose(file);
    return 1;
}

void adicionar_ao_ranking(RankingEntry ranking[], int *count, const char *nome, int pontos) {
    if (*count >= MAX_RANKINGS) return;
    strncpy(ranking[*count].nome, nome, MAX_NAME_LEN - 1);
    ranking[*count].nome[MAX_NAME_LEN - 1] = '\0';
    ranking[*count].pontos = pontos;
    (*count)++;
}

void ordenar_ranking(RankingEntry ranking[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (ranking[j].pontos > ranking[i].pontos) {
                RankingEntry temp = ranking[i];
                ranking[i] = ranking[j];
                ranking[j] = temp;
            }
        }
    }
}

void exibir_ranking(const RankingEntry ranking[], int count) {
    printf("==== RANKING ====\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s - %d pontos\n", i + 1, ranking[i].nome, ranking[i].pontos);
    }
}