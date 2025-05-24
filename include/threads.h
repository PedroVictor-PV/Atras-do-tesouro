#ifndef THREADS_H
#define THREADS_H

#include <winsock2.h>
#include <windows.h>
#include <stdbool.h>

extern SOCKET clientes[20];

// Funções para iniciar threads, retornam HANDLE das threads criadas
DWORD WINAPI controleInicioWrapper(LPVOID lpParam);
HANDLE iniciaThreadCliente(SOCKET cliente);

#endif // THREADS_H
