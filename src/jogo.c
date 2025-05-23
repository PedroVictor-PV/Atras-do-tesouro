#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "jogo.h"

void inicializaMatriz(char tabuleiro[LINHAS][COLUNAS])
{
    //MAPEAR TABULEIRO FORMATADO COM [ 1 ]  - [ 16 ]
   int i = 0, j = 0;
	for ( i = 0; i < LINHAS; i++)
        for ( j = 0; j < COLUNAS; j++)
            tabuleiro[i][j] = '-';
}

void embaralhaTesouros(char matriz[LINHAS][COLUNAS]) 
{
    srand((unsigned int)time(NULL) ^ GetCurrentThreadId());
    int colocados = 0, especiaisColocados = 0;
    while (colocados < 12) 
    {
        int randLinha = rand() % LINHAS;
        int randColuna = rand() % COLUNAS;
        
        if (matriz[randLinha][randColuna] != 't' && matriz[randLinha][randColuna] != 's') 
        {
            char tipoTesouro;
            if(especiaisColocados < 3 && (rand() % (12 - colocados)< (3- especiaisColocados)))
            {
                tipoTesouro= 's';
                especiaisColocados++;
            }
            else
            {
                tipoTesouro = 't';
            }

            matriz[randLinha][randColuna] = tipoTesouro;
            colocados++;
        }
    }
}

int tradutorDeCoordenada(int pos, char tesouros[LINHAS][COLUNAS], char player[LINHAS][COLUNAS]) {
    int linhaEscolhida = (pos - 1) / COLUNAS;
    int colunaEscolhida = (pos - 1) % COLUNAS;

    if (player[linhaEscolhida][colunaEscolhida] == '*')
    {
        return 3;  
    } 

    if (tesouros[linhaEscolhida][colunaEscolhida] == 't') 
    {
        player[linhaEscolhida][colunaEscolhida] = '$';
        return 1;
    } 
    else if (tesouros[linhaEscolhida][colunaEscolhida] == 's') 
    {
        player[linhaEscolhida][colunaEscolhida] = 'S';
        return 2;
    } 
    else 
    {
        player[linhaEscolhida][colunaEscolhida] = '*';
        return 0;
    }
}

void matrizParaTexto(char matriz[LINHAS][COLUNAS], char *saida) 
{
    char buffer[8];
    saida[0] = '\0';
     int i = 0, j = 0;
    for (i = 0; i < LINHAS; i++) 
    {
        strcat(saida, "\t");
        for ( j = 0; j < COLUNAS; j++) 
        {
            sprintf(buffer, " %c", matriz[i][j]);
            strcat(saida, buffer);
        }
        strcat(saida, "\n");
    }
}