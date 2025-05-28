#ifndef CLIENTE_HANDLER_H
#define CLIENTE_HANDLER_H

#include <winsock2.h>
#include <windows.h>
#include <stdbool.h>
#include "core/jogo.h"

extern SOCKET listaClientes[LIMITE_CLIENTES];
extern CRITICAL_SECTION csListaClientes;

typedef struct
{
    SOCKET cliente;
} ThreadArgs;

// Função principal da thread do cliente
DWORD WINAPI threadCliente(LPVOID param);

// Funções de processamento
bool processarJogo(SOCKET cliente);
bool solicitarNome(SOCKET cliente, char* nome);
bool enviarRegras(SOCKET cliente);
bool enviarCreditos(SOCKET cliente);
bool processarHack(SOCKET cliente);

// Funções de gerenciamento de clientes
void adicionarCliente(SOCKET cliente);
void removerCliente(SOCKET cliente);

#endif // CLIENTE_HANDLER_H