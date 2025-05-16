#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/jogo.h"

void mapeiaMatriz(char tabuleiro[6][6])
{
    //MAPEAR TABULEIRO FORMATADO COM [ 1 ]  - [ 16 ]
    int i=0;
    int j=0;

    for(i; i < 6; i++)
    {
        for(j=0; j<6; j++)
        {
            tabuleiro[i][j] = '-';
        }
    }
}

void embaralha(char tabuleiro[6][6])
{
    srand(time(NULL));
    int randLinha = -1;
    int randColuna = -1;
    int qtd = 0;
    int qtdSpecial = 0;
    while(qtd<10)
    {
        randLinha = rand()%6;
        randColuna = rand()%6;
        if(tabuleiro[randLinha][randColuna] != 't')
        {
            tabuleiro[randLinha][randColuna] = 't';
            qtd++;
        }
    }while (qtdSpecial < 2)
    {
        randLinha = rand()%6;
        randColuna = rand()%6;
        if((tabuleiro[randLinha][randColuna] != 't') && (tabuleiro[randLinha][randColuna] != 's'))
        {
            tabuleiro[randLinha][randColuna] = 's';
            qtdSpecial++;
        }
    }
    
}

int tradutorDeCoordenada(int numero, char tabuleiroPlayer[6][6], char tabuleiroTesouro[6][6]) 
{
   int linhaEscolhida = (numero - 1) / 6;
   int colunaEscolhida = (numero - 1) % 6;

    if (tabuleiroPlayer[linhaEscolhida][colunaEscolhida] == 't')
    {
        tabuleiroTesouro[linhaEscolhida][colunaEscolhida] = '$'; // Acertou!
        return 1;
    }else if(tabuleiroPlayer[linhaEscolhida][colunaEscolhida] == 's'){
        tabuleiroTesouro[linhaEscolhida][colunaEscolhida] = 'S'; // Acertou!
        return 2;
    }
    else if(tabuleiroTesouro[linhaEscolhida][colunaEscolhida] == '*')
    {
        return 3;
    }
    else {
        tabuleiroTesouro[linhaEscolhida][colunaEscolhida] = '*'; // Errou
        return 0;
    }
}

void imprimeMatriz(char tabuleiro[6][6])
{
    for (int i = 0; i < 6; i++) 
     {
        printf("\t");
        for (int j = 0; j < 6; j++) 
        {   
            printf(" %c", tabuleiro[i][j]);
        }
        printf("\n");
        
    }
}

void mapeiaEmbaralha(char tabuleiroPlayer[6][6], char tabuleiroTesouro[6][6])
{

    mapeiaMatriz(tabuleiroPlayer);

    mapeiaMatriz(tabuleiroTesouro);

    embaralha(tabuleiroTesouro);
}