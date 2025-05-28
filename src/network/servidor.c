#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include "../include/network/servidor.h"
#include "../include/network/cliente_handler.h"
#include "../include/threads/threads.h"

extern volatile LONG clientesConectados;
extern SOCKET clientes[LIMITE_CLIENTES];
extern CRITICAL_SECTION csListaClientes;

SOCKET inicializarServidor()
{
    WSADATA wsa;
    SOCKET servidor;
    struct sockaddr_in serv;

    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Erro ao inicializar Winsock: %d\n", WSAGetLastError());
        return INVALID_SOCKET;
    }

    // Criar socket
    servidor = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor == INVALID_SOCKET) {
        printf("Erro ao criar socket: %d\n", WSAGetLastError());
        WSACleanup();
        return INVALID_SOCKET;
    }

    // Configurar endereço do servidor
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(PORTA_SERVIDOR);

    // Bind
    if (bind(servidor, (struct sockaddr *)&serv, sizeof(serv)) == SOCKET_ERROR) {
        printf("Erro no bind: %d\n", WSAGetLastError());
        closesocket(servidor);
        WSACleanup();
        return INVALID_SOCKET;
    }

    // Listen
    if (listen(servidor, SOMAXCONN) == SOCKET_ERROR) {
        printf("Erro no listen: %d\n", WSAGetLastError());
        closesocket(servidor);
        WSACleanup();
        return INVALID_SOCKET;
    }

    return servidor;
}

bool executarLoopPrincipal(SOCKET servidor)
{
    struct sockaddr_in cli;
    int tamanho;
    SOCKET cliente;

    while (1) {
        tamanho = sizeof(cli);
        cliente = accept(servidor, (struct sockaddr *)&cli, &tamanho);
        
        if (cliente == INVALID_SOCKET) {
            printf("Erro no accept: %d\n", WSAGetLastError());
            continue;
        }

        // Verificar limite de clientes ANTES de incrementar
        LONG clientesAtuais = InterlockedCompareExchange(&clientesConectados, 0, 0);
        if (clientesAtuais >= LIMITE_CLIENTES) {
            const char* msg = "Servidor cheio. Tente novamente mais tarde.\n";
            send(cliente, msg, (int)strlen(msg), 0);
            closesocket(cliente);
            printf("Cliente rejeitado: limite de %d conexoes atingido.\n", LIMITE_CLIENTES);
            continue;
        }

        // Incrementar contador de clientes
        LONG novoIndice = InterlockedIncrement(&clientesConectados) - 1;

        // Double-check após incremento (proteção contra race condition)
        if (novoIndice >= LIMITE_CLIENTES) {
            const char* msg = "Servidor cheio. Tente novamente mais tarde.\n";
            send(cliente, msg, (int)strlen(msg), 0);
            closesocket(cliente);
            InterlockedDecrement(&clientesConectados);
            printf("Cliente rejeitado após race condition check.\n");
            continue;
        }

        // Armazenar socket na lista protegida
        EnterCriticalSection(&csListaClientes);
        int indiceDisponivel = -1;
        for (int i = 0; i < LIMITE_CLIENTES; i++) {
            if (clientes[i] == 0) {
                clientes[i] = cliente;
                indiceDisponivel = i;
                break;
            }
        }
        LeaveCriticalSection(&csListaClientes);

        // Verificação adicional de segurança
        if (indiceDisponivel == -1) {
            printf("Erro: não foi possível encontrar slot disponível\n");
            closesocket(cliente);
            InterlockedDecrement(&clientesConectados);
            continue;
        }

        // Criar thread para o cliente
        HANDLE hThreadCliente = iniciaThreadCliente(cliente);
        if (hThreadCliente == NULL) {
            printf("Erro ao criar thread para cliente\n");
            
            // Cleanup em caso de erro
            EnterCriticalSection(&csListaClientes);
            clientes[indiceDisponivel] = 0;
            LeaveCriticalSection(&csListaClientes);
            
            InterlockedDecrement(&clientesConectados);
            closesocket(cliente);
            continue;
        }

        // Fechar handle da thread (ela roda independente)
        CloseHandle(hThreadCliente);
        
        printf("Cliente conectado. Total: %ld/%d\n", 
               InterlockedCompareExchange(&clientesConectados, 0, 0), 
               LIMITE_CLIENTES);
    }

    return true;
}