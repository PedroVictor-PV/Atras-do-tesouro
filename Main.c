#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

    // Adicionar socket


    // Criar duas matrizes, uma abistraida e outra com as tesouros(pedro). 
    // A matriz1[posicao][posicao] recebe matriz2[posicao][posicao] e verificar se o que 
    // veio da segunda matriz é igual 'o';

void mapeiaMatriz(char tabuleiro[4][4]){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            tabuleiro[i][j] = '-';
        }
    }
}

void embaralha(char tabuleiro[4][4]){
    srand(time(NULL));
    int linha = -1;
    int coluna = -1;
    int qtd=0;
    while(qtd<4){
        linha = rand()%4;
        coluna = rand()%4;
            if(tabuleiro[linha][coluna] != 'o'){
                tabuleiro[linha][coluna] = 'o';
			    qtd++;
            }
    }
}

int main(int argc, char const *argv[]){
    char tabuleiroPlayer[4][4];
    char tabuleiroTesouro[4][4];

    int pontos= 0, tempo= 4;

    printf("\n======GO TO TREASURE!======\n");
    
    mapeiaMatriz(tabuleiroPlayer);
    mapeiaMatriz(tabuleiroTesouro);
    embaralha(tabuleiroTesouro);

    int linhaJog= -1, colunaJog= -1;

    char cont = 'n';
    do
    {   
        printf("\nPONTOS: %d", pontos);
        printf("\nEscolha a linha [0-3]: ");
        scanf("%d", &linhaJog);
        printf("\nEscolha a coluna [0-3]: ");
        scanf("%d", &colunaJog);

        if(linhaJog > 3 || colunaJog > 3)
        {
            printf("\nValor invalido!!");
            printf("\nEscolha novamente a linha [0-3]: ");
            scanf("%d", &linhaJog);
            printf("\nEscolha novamente a coluna [0-3]: ");
            scanf("%d", &colunaJog);
        }

        tabuleiroPlayer[linhaJog][colunaJog] = tabuleiroTesouro[linhaJog][colunaJog];
        if(tabuleiroPlayer[linhaJog][colunaJog] == 'o')
        {
            tabuleiroPlayer[linhaJog][colunaJog] = '$';
            pontos+=1;
                for(int i =0; i<4; i++)
                {
                    for(int j=0; j<4; j++)
                    {
                        printf(" %c", tabuleiroPlayer[i][j]);
                    }
                    printf("\n");
                    
                }
                    mapeiaMatriz(tabuleiroPlayer);
                    mapeiaMatriz(tabuleiroTesouro);
                    embaralha(tabuleiroTesouro);

                 
        }else
        {
            tabuleiroPlayer[linhaJog][colunaJog] = '*';
            for(int i =0; i<4; i++)
            {
                for(int j=0; j<4; j++)
                {
                    printf(" %c", tabuleiroPlayer[i][j]);
                }
                printf("\n");
            }
        }

        // Trocar por tentativas

        printf("\nCONTINUE?[s/n]");
        scanf(" %c", &cont);
    }while(cont== 's');

    printf("\nPONTOS: %d", pontos);

    return 0;
}
