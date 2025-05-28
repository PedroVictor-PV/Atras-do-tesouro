#include <stdio.h>
#include <string.h>
#include "utils/matriz_utils.h"

void matrizParaTexto(char matriz[LINHAS][COLUNAS], char *saida) 
{
    char buffer[8];
    saida[0] = '\0'; // Limpa string de saída
    
    for (int i = 0; i < LINHAS; i++) 
    {
        strcat(saida, "\t"); // Indentação
        for (int j = 0; j < COLUNAS; j++) 
        {
            sprintf(buffer, " %c", matriz[i][j]);
            strcat(saida, buffer);
        }
        strcat(saida, "\n");
    }
}

void imprimirMatriz(char matriz[LINHAS][COLUNAS])
{
    printf("\n  Matriz atual:\n");
    for (int i = 0; i < LINHAS; i++) 
    {
        printf("\t");
        for (int j = 0; j < COLUNAS; j++) 
        {
            printf(" %c", matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}