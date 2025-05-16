#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "../include/jogo.h"

// Adicionar socket

int main(int argc, char const *argv[])
{
    // -------------------------------------------------------------------------
    char tabuleiroPlayer[6][6];
    char tabuleiroTesouro[6][6];
    int escolha;

    mapeiaEmbaralha(tabuleiroPlayer, tabuleiroTesouro);

    // teste
    imprimeMatriz(tabuleiroTesouro);
    int pontos = 0, tentativas = 4, tesouros = 0;

    do
    {
        printf("\n====== GO TO TREASURE! ======\n");
        imprimeMatriz(tabuleiroPlayer);

        printf("\nPONTOS: %d", pontos);
        printf("\nEscolha - [1 | 36]: ");
        scanf("%d", &escolha);

        if (escolha < 1 || escolha > 36)
        {
            printf("\nValor invalido!!");
            printf("\nEscolha novamente a coordenada desejada - [1 | 36]: ");
            scanf("%d", &escolha);
        }

        boolean cordPlayer = tradutorDeCoordenada(escolha, tabuleiroTesouro, tabuleiroPlayer);

        while (cordPlayer == 3)
        {
            printf("\nEssa coordenada ja foi escolhida, escolha outra!\n");
            printf("\nEscolha - [1 | 36]: ");
            scanf("%d", &escolha);

            cordPlayer = tradutorDeCoordenada(escolha, tabuleiroTesouro, tabuleiroPlayer);
        }

        switch (cordPlayer)
        {
        case 0:
            printf("\nNada aqui...\n");
            Sleep(2000);
            system("cls");
            break;

        case 1:
            pontos += 1;
            tesouros += 1;
            printf("\nVoce encontrou um tesouro!\n");
            imprimeMatriz(tabuleiroPlayer);
            mapeiaEmbaralha(tabuleiroPlayer, tabuleiroTesouro);
            Sleep(2000);
            system("cls");
            break;

        case 2:
            pontos += 2;
            tesouros += 1;
            printf("\nVoce encontrou um tesouro especial!\n");
            imprimeMatriz(tabuleiroPlayer);
            mapeiaEmbaralha(tabuleiroPlayer, tabuleiroTesouro);

            Sleep(2000);
            system("cls");

        default:
            printf("\n Nenhuma das alternativas \n");
            break;
        }

        tentativas -= 1;
    } while (tentativas > 0);

    printf("\nPONTOS: %d", pontos);

    Sleep(2000);
    printf("\n\nFIM DE JOGO!\n");
    Sleep(1000);
    printf("\nVoce encontrou %d tesouro!", tesouros);
    Sleep(2000);
    printf("\n\nObrigado por jogar!\n");
    printf("\n\nDesenvolvido por: \n");
    printf("Pedro of War\n");
    printf("Gabriel Chico\n");
    printf("Miguel Bunda\n");
    printf("Reginado Rossi\n");
    Sleep(10000);
    // -------------------------------------------------------------------------

    return 0;
}
