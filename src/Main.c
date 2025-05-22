#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include "cliente_handler.h"
#include "jogo.h"
#pragma comment(lib, "ws2_32.lib")

#define LIMITE_CLIENTES 1

CRITICAL_SECTION csRanking;
Jogador ranking[LIMITE_CLIENTES];
int totalJogadores = 0;
volatile LONG clientesConectados = 0;

int main(int argc, char const *argv[])
{
   
    WSADATA wsa;
    SOCKET servidor, cliente;
    struct sockaddr_in serv, cli;
    int tamanho;

    WSAStartup(MAKEWORD(2, 2), &wsa);
    InitializeCriticalSection(&csRanking);

    servidor = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor == INVALID_SOCKET) 
    {
        printf("Erro ao criar socket\n");
        return 1;
    }

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(8888);

    if (bind(servidor, (struct sockaddr *)&serv, sizeof(serv)) == SOCKET_ERROR) 
    {
        printf("Erro no bind\n");
        closesocket(servidor);
        WSACleanup();
        return 1;
    }

    if (listen(servidor, SOMAXCONN) == SOCKET_ERROR) 
    {
        printf("Erro no listen\n");
        closesocket(servidor);
        WSACleanup();
        return 1;
    }

    printf("Servidor aguardando conexoes...\n");

     while (1) 
    {
        tamanho = sizeof(cli);
        cliente = accept(servidor, (struct sockaddr *)&cli, &tamanho);
        if (cliente == INVALID_SOCKET) 
        {
            printf("Erro no accept\n");
            continue;
        }

        // Verifica se atingiu o limite
        if (clientesConectados >= LIMITE_CLIENTES) 
        {
            const char* msg = "Servidor cheio. Tente novamente mais tarde.\n";
            send(cliente, msg, (int)strlen(msg), 0);
            closesocket(cliente);
            printf("Cliente rejeitado: limite de %d conexoes.\n", LIMITE_CLIENTES);
            continue;
        }

        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->cliente = cliente;

        HANDLE thread = CreateThread(NULL, 0, threadCliente, args, 0, NULL);
        if (thread == NULL) 
        {
            printf("Erro ao criar thread\n");
            closesocket(cliente);
            free(args);
        } else 
        {
            CloseHandle(thread);
        }
    }

    DeleteCriticalSection(&csRanking);
    closesocket(servidor);
    WSACleanup();

    /*
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
    */
    return 0;
}
