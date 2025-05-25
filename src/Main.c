#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <stdbool.h>

// Headers reorganizados
#include "core/jogo.h"
#include "network/cliente_handler.h"
#include "network/servidor.h"
#include "threads/threads.h"

// Variáveis globais compartilhadas
CRITICAL_SECTION csRanking;
CRITICAL_SECTION csClientes;
CRITICAL_SECTION csListaClientes;

Jogador ranking[LIMITE_CLIENTES] = {0};
SOCKET clientes[LIMITE_CLIENTES] = {0};
SOCKET listaClientes[LIMITE_CLIENTES] = {0};

volatile LONG clientesConectados = 0;
volatile LONG jogoIniciado = 0;

int main(void)
{
    printf("=== SERVIDOR ATRAS DO TESOURO ===\n");
    printf("Inicializando servidor...\n");

    // Inicializar critical sections
    if (!inicializarCriticalSections()) {
        printf("Erro ao inicializar critical sections\n");
        return 1;
    }

    // Inicializar Winsock e criar servidor
    SOCKET servidor = inicializarServidor();
    if (servidor == INVALID_SOCKET) {
        printf("Erro ao inicializar servidor\n");
        limpezaFinal();
        return 1;
    }

    printf("Servidor aguardando conexoes na porta %d...\n", PORTA_SERVIDOR);

    // Criar thread de controle de início do jogo
    HANDLE hThreadControle = CreateThread(NULL, 0, controleInicio, NULL, 0, NULL);
    if (hThreadControle == NULL) {
        fprintf(stderr, "Erro ao criar thread de controle.\n");
        closesocket(servidor);
        limpezaFinal();
        return 1;
    }

    // Loop principal - aceitar conexões
    if (!executarLoopPrincipal(servidor)) {
        printf("Erro no loop principal do servidor\n");
    }

    // Limpeza final
    WaitForSingleObject(hThreadControle, INFINITE);
    CloseHandle(hThreadControle);
    closesocket(servidor);
    limpezaFinal();

    printf("Servidor encerrado.\n");
    return 0;
}

bool inicializarCriticalSections() 
{
    InitializeCriticalSection(&csRanking);
    InitializeCriticalSection(&csClientes);
    InitializeCriticalSection(&csListaClientes);
    return true;
}

void limpezaFinal() 
{
    DeleteCriticalSection(&csRanking);
    DeleteCriticalSection(&csClientes);
    DeleteCriticalSection(&csListaClientes);
    WSACleanup();
}