#include <windows.h>  // Para GetCurrentThreadId

#ifndef JOGO_H
#define JOGO_H

#define LINHAS 6
#define COLUNAS 6

typedef struct 
{
    char nome[110];
    int pontos;
} Jogador;



void inicializaMatriz(char tabuleiro[LINHAS][COLUNAS]);
void embaralhaTesouros(char tabuleiro[LINHAS][COLUNAS]);
int tradutorDeCoordenada(int pos, char tabuleiro[LINHAS][COLUNAS], char tabuleiro2[LINHAS][COLUNAS]);
void matrizParaTexto(char tabuleiro[LINHAS][COLUNAS], char *saida);

#endif
