#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

    // Adicionar socket

    //MAPEAR  A MATRIZ DE 1 AO FINAL DO TAMAHO
    //ABSTRAIR A LOGICA PARA INFORMAR O NÚMERO DO QUADRA
void mapeiaMatriz(int linha, int coluna, char tabuleiro[linha][coluna])
{
    //MAPEAR TABULEIRO FORMATADO COM [ 1 ]  - [ 16 ]
    int cont= 1;
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            tabuleiro[i][j] = '-';
            cont ++;
        }
    }
}

void embaralha(int linha, int coluna, char tabuleiro[linha][coluna])
{
    srand(time(NULL));
    int randLinha = -1;
    int randColuna = -1;
    int qtd=0;
    while(qtd<3)
    {
        randLinha = rand()%4;
        randColuna = rand()%4;
        if(tabuleiro[randLinha][randColuna] != 'o')
        {
            tabuleiro[randLinha][randColuna] = 'o';
            qtd++;
        }
    }
}

char tradutorDeCoordenada(int numero, int linha, int coluna, char matriz[linha][coluna]) 
{
    int cont = 1;
    char result = '?'; 

    for (int i = 0; i < linha; i++)
    {
        for (int j = 0; j < coluna; j++)
        {
            if (numero == cont)
            {
                return matriz[i][j];
            }
            cont++;
        }
    }

    return result;
}


int main(int argc, char const *argv[])
{

// -------------------------------------------------------------------------
    char tabuleiroPlayer[4][4];
    char tabuleiroTesouro[4][4];
    int escolha;

    printf("\n======GO TO TREASURE!======\n");

    mapeiaMatriz(4, 4, tabuleiroPlayer);
    mapeiaMatriz(4, 4, tabuleiroTesouro);
    embaralha(4, 4, tabuleiroTesouro);
    
    for (int i = 0; i < 4; i ++)
    {
        for(int j = 0; j < 4; j ++)
        {
            printf(" %c", tabuleiroTesouro[i][j]);       
        }
        printf("\n");
    }
    
    int pontos= 0, tentativas = 3;

    do
    {
        printf("\nPONTOS: %d", pontos);
        printf("\nEscolha - [1 | 16]: ");
        scanf("%d", &escolha);
        
        if(escolha >16)
        {
           printf("\nValor invalido!!");
           printf("\nEscolha novamente a coordenada desejada - [1 | 16]: ");
           scanf("%d", &escolha);
        }

        char a = tradutorDeCoordenada(escolha, 4, 4, tabuleiroPlayer);
        char b = tradutorDeCoordenada(escolha, 4, 4, tabuleiroTesouro);

        a = b;
    
            if(a == 'o')
            {              
                a = '$';
                pontos+= 1;
                //ADICIONAR COMO IMPRIMIR NA TELA QUE O PONTO FOI COMPUTADO
                    for (int i = 0; i < 4; i ++)
                    {
                        for (int j = 0; j < 4; j ++)
                        {
                            printf(" %c", tabuleiroPlayer[i][j]);
                        }
                        printf("\n");
                        
                    }

                mapeiaMatriz(4, 4, tabuleiroPlayer);
                mapeiaMatriz(4, 4, tabuleiroTesouro);
                embaralha(4, 4, tabuleiroTesouro);

            }else
            {
                a = '*';
                for(int i = 0; i < 4; i ++)
                {
                    for(int j = 0; j < 4; j ++)
                    {
                        printf(" %c", tabuleiroPlayer[i][j]);
                    }
                    printf("\n");
                }
            }

            tentativas-=1;

        printf("\n %c", a);
        printf("\n %c", b);


    } while (tentativas>0);

    printf("\nPONTOS: %d", pontos);
    
   
// -------------------------------------------------------------------------

    // int pontos= 0, tentativas = 10;

    // printf("\n======GO TO TREASURE!======\n");
    
    // //CRIAR UMA FUNÇAO PARA REDUZIR O CHAMAMENTO DUPLO
    // mapeiaMatriz(tabuleiroPlayer);
    // mapeiaMatriz(tabuleiroTesouro);
    // embaralha(tabuleiroTesouro);

    // int linhaJog= -1, colunaJog= -1;
    
    // do
    // {   
    //     printf("\n%d Tentativas restantes!", tentativas);
    //     printf("\nPONTOS: %d", pontos);
    //     printf("\nEscolha a linha [0-3]: ");
    //     scanf("%d", &linhaJog);
    //     printf("\nEscolha a coluna [0-3]: ");
    //     scanf("%d", &colunaJog);

    //     if(linhaJog > 3 || colunaJog > 3)
    //     {
    //         printf("\nValor invalido!!");
    //         printf("\nEscolha novamente a linha [0-3]: ");
    //         scanf("%d", &linhaJog);
    //         printf("\nEscolha novamente a coluna [0-3]: ");
    //         scanf("%d", &colunaJog);
    //     }

    //     tabuleiroPlayer[linhaJog][colunaJog] = tabuleiroTesouro[linhaJog][colunaJog];

    //     if(tabuleiroPlayer[linhaJog][colunaJog] == 'o')
    //     {
    //         tabuleiroPlayer[linhaJog][colunaJog] = '$';
    //         pontos+=1;
    //             for(int i =0; i<4; i++)
    //             {
    //                 for(int j=0; j<4; j++)
    //                 {
    //                     printf(" %c", tabuleiroPlayer[i][j]);
    //                 }
    //                 printf("\n");
                    
    //             }

    //                 mapeiaMatriz(tabuleiroPlayer);
    //                 mapeiaMatriz(tabuleiroTesouro);
    //                 embaralha(tabuleiroTesouro);

    //     }else
    //     {
    //         tabuleiroPlayer[linhaJog][colunaJog] = '*';
    //         for(int i =0; i<4; i++)
    //         {
    //             for(int j=0; j<4; j++)
    //             {
    //                 printf(" %c", tabuleiroPlayer[i][j]);
    //             }
    //             printf("\n");
    //         }
    //     }
    //     tentativas-=1;
        
    // }while(tentativas > 0);

    // printf("\nPONTOS: %d", pontos);

    return 0;
}