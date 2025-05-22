#ifndef CLIENTE_HANDLER.H
#define CLIENTE_HANDLER.H

#include <winsock2.h>
#include <windows.h>

DWORD WINAPI threadCliente(LPVOID param);

typedef struct
{
    SOCKET cliente;
} ThreadArgs;

#endif