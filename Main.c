#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

    // Adicionar socket

    //MAPEAR  A MATRIZ DE 1 AO FINAL DO TAMAHO
    //ABSTRAIR A LOGICA PARA INFORMAR O NÚMERO DO QUADRA
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
    while(qtd<5){
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
    
    int pontos= 0, tentativas = 10;

    printf("\n======GO TO TREASURE!======\n");
    
    //CRIAR UMA FUNÇAO PARA REDUZIR O CHAMAMENTO DUPLO
    mapeiaMatriz(tabuleiroPlayer);
    mapeiaMatriz(tabuleiroTesouro);
    embaralha(tabuleiroTesouro);

    int linhaJog= -1, colunaJog= -1;
    
    do
    {   
        printf("\n%d Tentativas restantes!", tentativas);
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
        tentativas-=1;
        
    }while(tentativas > 0);

    printf("\nPONTOS: %d", pontos);

    return 0;
}
