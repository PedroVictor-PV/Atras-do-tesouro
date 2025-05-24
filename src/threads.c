#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "threads.h"
#include "threads.h"
#include "cliente_handler.h"

// Variáveis globais (externas) usadas pelas threads
extern SOCKET clientes[20];
extern volatile LONG clientesConectados;
volatile LONG jogoIniciado = 0;
extern CRITICAL_SECTION csClientes;


#define MIN_CLIENTES_PARA_INICIAR 5

// Wrapper para a thread controleInicio — função que já existia (definida em controle_inicio.c)
DWORD WINAPI controleInicio(LPVOID param);

// Wrapper para a threadCliente — função que já existia (definida em cliente_handler.c)
DWORD WINAPI threadClienteWrapper(LPVOID lpParam) {
    return threadCliente(lpParam);
}

// Função para criar a thread de controleInicio
DWORD WINAPI controleInicio(LPVOID param)
{
    char resposta[10];
   while(1)
    {
        // Verifica se há pelo menos MIN_CLIENTES_PARA_INICIAR conectados
        if (InterlockedCompareExchange(&clientesConectados, 0, 0) >= MIN_CLIENTES_PARA_INICIAR)
        {
            printf("Clientes conectados: %ld. Deseja iniciar o jogo? (S/N): ", clientesConectados);
            fflush(stdout);  // Garante que o prompt apareça imediatamente

            if (fgets(resposta, sizeof(resposta), stdin) != NULL)
            {
                // Remove o \n do final da string lida
                resposta[strcspn(resposta, "\r\n")] = 0;

                if (resposta[0] == 'S' || resposta[0] == 's')
                {
                    // Marca que o jogo iniciou e sai do loop
                    InterlockedExchange(&jogoIniciado, 1);
                    printf("Jogo iniciado!\n");
                    break;
                }
                else
                {
                    printf("Esperando mais conexões...\n");
     
                }
            }
        } 
        Sleep(3000);   
    }
}

// Função para criar a thread do cliente passando socket
HANDLE iniciaThreadCliente(SOCKET cliente)
 {
    ThreadArgs* args = malloc(sizeof(ThreadArgs));
    if (!args) 
    {
        fprintf(stderr, "Erro ao alocar memoria para ThreadArgs.\n");
        return NULL;
    }
    args->cliente = cliente;

    HANDLE hThread = CreateThread(NULL, 0, threadClienteWrapper, args, 0, NULL);
    if (hThread == NULL) 
    {
        fprintf(stderr, "Erro ao criar thread cliente.\n");
        free(args);
    }
    return hThread;
}


