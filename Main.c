#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

    // Adicionar socket

 
void mapeiaMatriz(int linha, int coluna, char tabuleiro[linha][coluna])
{
    //MAPEAR TABULEIRO FORMATADO COM [ 1 ]  - [ 16 ]
    int cont= 1;
    int i=0;
    int j=0;

    for(i; i < 4; i++)
    {
        for(j=0; j<4; j++)
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


boolean tradutorDeCoordenada(int numero, int linha, int coluna, char matriz[linha][coluna], char tabuleiro2[linha][coluna]) 
{
   int linhaEscolhida = (numero - 1) / 4;
   int colunaEscolhida = (numero - 1) % 4;

    if (matriz[linhaEscolhida][colunaEscolhida] == 'o') 
    {
        tabuleiro2[linhaEscolhida][colunaEscolhida] = '$'; // Acertou!
        return TRUE;
            
    } else {
            tabuleiro2[linhaEscolhida][colunaEscolhida] = '*'; // Errou
        return FALSE;
    }

}

void imprimeMatriz(int linha, int coluna, char tabuleiro[linha][coluna])
{
    for (int i = 0; i < 4; i++) 
     {
        printf("\t");
        for (int j = 0; j < 4; j++) 
        {   
            
            printf(" %c", tabuleiro[i][j]);
        }
        printf("\n");
        
    }
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
    
    imprimeMatriz(4, 4, tabuleiroTesouro);
    
    int pontos= 0, tentativas = 3;
    
    do
    {
        printf("\nPONTOS: %d", pontos);
        printf("\nEscolha - [1 | 16]: ");
        scanf("%d", &escolha);
        
        if(escolha < 1 || escolha > 16)
        {
            printf("\nValor invalido!!");
            printf("\nEscolha novamente a coordenada desejada - [1 | 16]: ");
            scanf("%d", &escolha);
        }

        boolean cordPlayer= tradutorDeCoordenada(escolha, 4, 4, tabuleiroTesouro, tabuleiroPlayer);
        
          if(cordPlayer == TRUE)
          {
            pontos+= 1;
            printf("\nVoce encontrou um tesouro!\n");
           
            imprimeMatriz(4, 4, tabuleiroPlayer);

            mapeiaMatriz(4, 4, tabuleiroPlayer);

            mapeiaMatriz(4, 4, tabuleiroTesouro);

            embaralha(4, 4, tabuleiroTesouro);
            
        }else
        {
            printf("\nNada aqui...\n");

            imprimeMatriz(4, 4, tabuleiroPlayer);
            
        }
        tentativas-=1;
                
    } while (tentativas>0);
    
    printf("\nPONTOS: %d", pontos);
         
   
// -------------------------------------------------------------------------

    return 0;
}

 




