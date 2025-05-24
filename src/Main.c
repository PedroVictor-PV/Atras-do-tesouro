#include <stdio.h> 
#include <winsock2.h> //biblioteca para utilizar sockets
#include <windows.h> 
#include <stdbool.h>

#include "jogo.h"
#include "cliente_handler.h"
#include "threads.h"

CRITICAL_SECTION csRanking;
CRITICAL_SECTION csClientes;
Jogador ranking[LIMITE_CLIENTES];
SOCKET clientes[20];

volatile LONG clientesConectados = 0;
extern volatile LONG jogoIniciado;
DWORD WINAPI controleInicio(LPVOID param); 

int main(int argc, char const *argv[])
{
    WSADATA wsa;
    SOCKET servidor, cliente;
    struct sockaddr_in serv, cli;
    int tamanho;

    WSAStartup(MAKEWORD(2, 2), &wsa);

    InitializeCriticalSection(&csRanking);
    InitializeCriticalSection(&csClientes);
    InitializeCriticalSection(&csListaClientes);  // Inicializar apenas uma vez, fora do loop

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

    HANDLE hThreadControle = CreateThread(NULL, 0, controleInicio, NULL, 0, NULL);
    if (hThreadControle == NULL) 
    {
        fprintf(stderr, "Erro ao criar thread de controle.\n");
        return 1;
    }

    while (1) 
    {   
        tamanho = sizeof(cli);
        cliente = accept(servidor, (struct sockaddr *)&cli, &tamanho);
        if (cliente == INVALID_SOCKET) 
        {
            printf("Erro no accept\n");
            continue;
        }

        LONG idx = InterlockedIncrement(&clientesConectados) - 1;  // Incrementa e usa índice

        if (idx >= LIMITE_CLIENTES) {

            const char* msg = "Servidor cheio. Tente novamente mais tarde.\n";
            send(cliente, msg, (int)strlen(msg), 0);
            closesocket(cliente);
            printf("Cliente rejeitado: limite de %d conexoes.\n", LIMITE_CLIENTES);

            InterlockedDecrement(&clientesConectados); // Reverte incremento
            continue;  // Não use return para continuar rodando o servidor
        }

        EnterCriticalSection(&csListaClientes);
        clientes[idx] = cliente;   // Armazena socket na lista protegida
        LeaveCriticalSection(&csListaClientes);

        iniciaThreadCliente(cliente);
        
    }

    WaitForSingleObject(hThreadControle, INFINITE);
    CloseHandle(hThreadControle);
    DeleteCriticalSection(&csRanking);
    DeleteCriticalSection(&csClientes);
    DeleteCriticalSection(&csListaClientes);
    closesocket(servidor);
    WSACleanup();

    return 0;
}
