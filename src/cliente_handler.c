#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include "cliente_handler.h"
#include "jogo.h"

#define LIMITE_CLIENTES 1

extern Jogador ranking[LIMITE_CLIENTES];
extern int totalJogadores;
extern CRITICAL_SECTION csRanking;
extern volatile LONG clientesConectados;

DWORD WINAPI threadCliente(LPVOID param)
{
    SOCKET cliente = ((ThreadArgs*)param)->cliente;
    free(param);

     //Nome do players conectados
    char nome [110];
    int namePlayer = recv(cliente,nome,sizeof(nome) - 1, 0);
    nome[namePlayer] = '\0';
    nome[strcspn(nome, "\n")] = '\0'; 
    

    InterlockedIncrement(&clientesConectados);
 
    char buffer[1024];
    char tabuleiroTesouro[6][6];
    char tabuleiroPlayer[6][6];
    int pontos = 0, tentativas = 4, tesouros = 0;

    inicializaMatriz(tabuleiroTesouro);
    inicializaMatriz(tabuleiroPlayer);
    embaralhaTesouros(tabuleiroTesouro);

    while (tentativas > 0) 
    {   
        char msg[255], inicio[500], imprimeMatriz[1024];

        matrizParaTexto(tabuleiroPlayer, imprimeMatriz);
        sprintf(inicio, "[INPUT]\n\n====== GO TO TREASURE! ======\n%s\nPontos: %d\nTentativas restantes: %d\nPara SAIR digite 0.\nEscolha uma posicao - [1 | 36]: ", imprimeMatriz, pontos, tentativas);
        send(cliente, inicio, (int)strlen(inicio), 0);

        int bytes = recv(cliente, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) break;
        buffer[bytes] = '\0';

        int escolha = atoi(buffer);

        while(escolha < 0 || escolha > 36)
        {
            sprintf(msg, "[INPUT]\nValor invalido!!\nEscolha novamente: ");
            send(cliente, msg, (int)strlen(msg), 0);

            bytes = recv(cliente, buffer, sizeof(buffer) - 1, 0);
            if (bytes <= 0) break;
            buffer[bytes] = '\0';

            escolha = atoi(buffer);
        }

        int resultado = tradutorDeCoordenada(escolha, tabuleiroTesouro, tabuleiroPlayer);

        while (resultado == 3) {
            send(cliente, "[INPUT]\nEssa coordenada ja escolhida, escolha outra!\nPara SAIR digite 0.\nEscolha - [1| 36]:  ", 65, 0);

            bytes = recv(cliente, buffer, sizeof(buffer) - 1, 0);
            if (bytes <= 0) break;
            buffer[bytes] = '\0';

            escolha = atoi(buffer);
            resultado = tradutorDeCoordenada(escolha, tabuleiroTesouro, tabuleiroPlayer);
        }

       
        switch(resultado)
        {
            case 0:
                send(cliente, "[CLEAR]\nNada aqui...\n", 25, 0);
                break;
            
            case 1:
                pontos+=1; tesouros+=1;
                matrizParaTexto(tabuleiroPlayer, imprimeMatriz);
                sprintf(msg, "[CLEAR]\nVoce encontrou um tesouro!\n%s\n", imprimeMatriz);
                send(cliente, msg, (int)strlen(msg), 0);
                inicializaMatriz(tabuleiroTesouro);
                inicializaMatriz(tabuleiroPlayer);
                embaralhaTesouros(tabuleiroTesouro);
                break;
            
            case 2:
                pontos+=2;tesouros+=1;
                matrizParaTexto(tabuleiroPlayer, imprimeMatriz);
                sprintf(msg, "[CLEAR]\nVoce encontrou um tesouro especial!\n%s\n", imprimeMatriz);
                send(cliente, msg, (int)strlen(msg), 0);
                inicializaMatriz(tabuleiroTesouro);
                inicializaMatriz(tabuleiroPlayer);
                embaralhaTesouros(tabuleiroTesouro);
                break;
        }
        
        tentativas --;
    }
    char fim[256];
    sprintf(fim, "\nFIM DE JOGO!\nTesouros encontrados: %d\nPontuação final: %d", tesouros, pontos);
    send(cliente, fim, (int)strlen(fim), 0);
    
    EnterCriticalSection(&csRanking);
    strcpy(ranking[totalJogadores].nome, nome);
    ranking[totalJogadores].pontos = pontos;
    totalJogadores++;
    LeaveCriticalSection(&csRanking);

    closesocket(cliente);
    InterlockedDecrement(&clientesConectados);
    return 0;

}
