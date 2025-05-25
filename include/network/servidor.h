#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <winsock2.h>
#include <windows.h>
#include <stdbool.h>

#define PORTA_SERVIDOR 8888

// Funções principais do servidor
SOCKET inicializarServidor();
bool executarLoopPrincipal(SOCKET servidor);

// Funções auxiliares de inicialização
bool inicializarCriticalSections();
void limpezaFinal();

#endif // SERVIDOR_H