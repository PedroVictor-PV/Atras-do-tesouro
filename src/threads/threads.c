#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "threads/threads.h"
#include "network/cliente_handler.h"

extern SOCKET clientes[LIMITE_CLIENTES];
extern volatile LONG clientesConectados;
extern volatile LONG jogoIniciado;
extern CRITICAL_SECTION csClientes;

#define MIN_CLIENTES_PARA_INICIAR 2  // Reduzido para testes

// Wrapper para a threadCliente
DWORD WINAPI threadClienteWrapper(LPVOID lpParam) {
    return threadCliente(lpParam);
}

// Thread de controle de início do jogo
DWORD WINAPI controleInicio(LPVOID param)
{
    (void)param;
    char resposta[10];
    printf("Thread de controle iniciada. Aguardando clientes...\n");
    
    while(1)
    {
        LONG clientesAtuais = InterlockedCompareExchange(&clientesConectados, 0, 0);
        
        // Verifica se há clientes suficientes conectados
        if (clientesAtuais >= MIN_CLIENTES_PARA_INICIAR)
        {
            printf("\n=== CONTROLE DO JOGO ===\n");
            printf("Clientes conectados: %ld/%d\n", clientesAtuais, LIMITE_CLIENTES);
            printf("Deseja iniciar o jogo? (S/N): ");
            fflush(stdout);

            if (fgets(resposta, sizeof(resposta), stdin) != NULL)
            {
                // Remove quebras de linha
                resposta[strcspn(resposta, "\r\n")] = '\0';

                if (resposta[0] == 'S' || resposta[0] == 's' || resposta[0] == '1')
                {
                    // Marca que o jogo iniciou
                    InterlockedExchange(&jogoIniciado, 1);
                    printf("*** JOGO INICIADO! ***\n");
                    printf("Os clientes agora podem acessar o menu do jogo.\n");
                    
                    // Envia sinal para todos os clientes conectados
                    notificarInicioJogo();
                    break;
                }
                else if (resposta[0] == 'N' || resposta[0] == 'n' || resposta[0] == '0')
                {
                    printf("Continuando a aguardar mais conexoes...\n");
                }
                else 
                {
                    printf("Opcao invalida. Digite S para Sim ou N para Nao.\n");
                }
            }
        } 
        else 
        {
            if (clientesAtuais > 0) {
                printf("Aguardando mais clientes... (%ld/%d conectados)\n", 
                       clientesAtuais, MIN_CLIENTES_PARA_INICIAR);
            }
        }
        
        Sleep(5000); // Verifica a cada 5 segundos
    }
    
    printf("Thread de controle finalizada.\n");
    return 0;
}

// Função para criar thread do cliente
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
        fprintf(stderr, "Erro ao criar thread cliente: %lu\n", GetLastError());
        free(args);
        return NULL;
    }
    
    return hThread;
}

// Função para notificar todos os clientes sobre o início do jogo
void notificarInicioJogo()
{
    const char* mensagem = "[INICIOJOGO]\nO jogo foi iniciado pelo administrador!\n";
    
    EnterCriticalSection(&csClientes);
    
    for (int i = 0; i < LIMITE_CLIENTES; i++) 
    {
        if (clientes[i] != 0) 
        {
            int resultado = send(clientes[i], mensagem, (int)strlen(mensagem), 0);
            if (resultado == SOCKET_ERROR) 
            {
                printf("Erro ao notificar cliente %d sobre inicio do jogo\n", i);
            }
        }
    }
    
    LeaveCriticalSection(&csClientes);
    
    printf("Notificacao de inicio enviada para todos os clientes conectados.\n");
}

// Função para verificar se o jogo foi iniciado
bool jogoFoiIniciado()
{
    return InterlockedCompareExchange(&jogoIniciado, 1, 1) == 1;
}