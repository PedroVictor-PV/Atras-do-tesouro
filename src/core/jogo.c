#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <winsock2.h>
#include <windows.h>
#include <string.h>
#include "../include/core/jogo.h" // jogo h
#include "../include/utils/matriz_utils.h"// matriz.h

// extern SOCKET listaClientes[LIMITE_CLIENTES];
// extern CRITICAL_SECTION csListaClientes;
// extern volatile LONG clientesConectados;

void inicializaMatriz(char tabuleiro[LINHAS][COLUNAS])
{
    // Inicializa tabuleiro com '-'
    int i = 0, j = 0;
    for (i = 0; i < LINHAS; i++)
        for (j = 0; j < COLUNAS; j++)
            tabuleiro[i][j] = '-';
}

void embaralhaTesouros(char matriz[LINHAS][COLUNAS]) 
{
    // Gera seed diferente para cada thread
    srand((unsigned int)time(NULL) ^ GetCurrentThreadId());
    int colocados = 0, especiaisColocados = 0;
    
    while (colocados < 12) 
    {
        int randLinha = rand() % LINHAS;
        int randColuna = rand() % COLUNAS;
        
        // Verifica se posição está livre
        if (matriz[randLinha][randColuna] != 't' && matriz[randLinha][randColuna] != 's') 
        {
            char tipoTesouro;
            
            // Distribui tesouros especiais (máximo 3)
            if (especiaisColocados < 3 && (rand() % (12 - colocados) < (3 - especiaisColocados)))
            {
                tipoTesouro = 's'; // tesouro especial
                especiaisColocados++;
            }
            else
            {
                tipoTesouro = 't'; // tesouro normal
            }

            matriz[randLinha][randColuna] = tipoTesouro;
            colocados++;
        }
    }
}

int tradutorDeCoordenada(int pos, char tesouros[LINHAS][COLUNAS], char player[LINHAS][COLUNAS]) 
{
    // Converte posição (1-36) para coordenadas da matriz
    int linhaEscolhida = (pos - 1) / COLUNAS;
    int colunaEscolhida = (pos - 1) % COLUNAS;

    // Verifica se já foi escolhida
    if (player[linhaEscolhida][colunaEscolhida] == '*' || 
        player[linhaEscolhida][colunaEscolhida] == '$' || 
        player[linhaEscolhida][colunaEscolhida] == 'S')
    {
        return 3; // Posição já escolhida
    } 

    // Verifica o que há na posição
    if (tesouros[linhaEscolhida][colunaEscolhida] == 't') 
    {
        player[linhaEscolhida][colunaEscolhida] = '$'; // Tesouro normal
        return 1;
    } 
    else if (tesouros[linhaEscolhida][colunaEscolhida] == 's') 
    {
        player[linhaEscolhida][colunaEscolhida] = 'S'; // Tesouro especial
        return 2;
    } 
    else 
    {
        player[linhaEscolhida][colunaEscolhida] = '*'; // Vazio
        return 0;
    }
}

// void adicionarCliente(SOCKET cliente) 
// {
//     EnterCriticalSection(&csListaClientes);
//     for (int i = 0; i < LIMITE_CLIENTES; i++) {
//         if (listaClientes[i] == 0) {
//             listaClientes[i] = cliente;
//             break;
//         }
//     }
//     LeaveCriticalSection(&csListaClientes);
// }

// void removerCliente(SOCKET cliente) 
// {
//     EnterCriticalSection(&csListaClientes);
//     for (int i = 0; i < LIMITE_CLIENTES; i++) {
//         if (listaClientes[i] == cliente) {
//             listaClientes[i] = 0;
//             break;
//         }
//     }
//     LeaveCriticalSection(&csListaClientes);
// }