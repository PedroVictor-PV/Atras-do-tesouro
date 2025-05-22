#ifndef CLIENTE_HANDLER_H
#define CLIENTE_HANDLER_H

#include <winsock2.h>
#include <windows.h>

DWORD WINAPI threadCliente(LPVOID param);

typedef struct
{
    SOCKET cliente;
} ThreadArgs;

#endif