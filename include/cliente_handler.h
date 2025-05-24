#ifndef CLIENTE_HANDLER_H
#define CLIENTE_HANDLER_H


#include <winsock2.h>
#include <windows.h>
#define LIMITE_CLIENTES 20
extern SOCKET listaClientes[LIMITE_CLIENTES];
extern CRITICAL_SECTION csListaClientes;

DWORD WINAPI threadCliente(LPVOID param);

typedef struct
{
    SOCKET cliente;
} ThreadArgs;

void adicionarCliente(SOCKET cliente);
void removerCliente(SOCKET cliente);
void broadcastAtualizacao();

#endif