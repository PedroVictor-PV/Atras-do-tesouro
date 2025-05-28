#ifndef MATRIZ_UTILS_H
#define MATRIZ_UTILS_H

#include "core/jogo.h"

// Função para converter matriz em string para envio
void matrizParaTexto(char matriz[LINHAS][COLUNAS], char *saida);

// Função para exibir matriz formatada (para debug do servidor)
void imprimirMatriz(char matriz[LINHAS][COLUNAS]);

#endif