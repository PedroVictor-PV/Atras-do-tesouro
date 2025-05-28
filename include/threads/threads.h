#ifndef THREADS_H
#define THREADS_H

#include <winsock2.h>
#include <windows.h>
#include <stdbool.h>

// Função principal da thread de controle de início do jogo
DWORD WINAPI controleInicio(LPVOID param);

// Função para criar thread do cliente
HANDLE iniciaThreadCliente(SOCKET cliente);

// Função para notificar todos os clientes sobre o início do jogo
void notificarInicioJogo(void);

// Função para verificar se o jogo foi iniciado
bool jogoFoiIniciado(void);

#endif // THREADS_H