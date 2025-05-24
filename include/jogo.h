#ifndef JOGO_H
#define JOGO_H

#include <winsock2.h>  
#include <windows.h> 

#define LINHAS 6
#define COLUNAS 6
#define LIMITE_CLIENTES 20

extern SOCKET listaClientes[LIMITE_CLIENTES];
extern CRITICAL_SECTION csListaClientes;
extern volatile LONG clientesConectados;
typedef struct 
{
    char nome[110];
    int pontos;
} Jogador;


void inicializaMatriz(char tabuleiro[LINHAS][COLUNAS]);
void embaralhaTesouros(char tabuleiro[LINHAS][COLUNAS]);
int tradutorDeCoordenada(int pos, char tabuleiro[LINHAS][COLUNAS], char tabuleiro2[LINHAS][COLUNAS]);
void matrizParaTexto(char tabuleiro[LINHAS][COLUNAS], char *saida);

void adicionarCliente(SOCKET cliente);
void removerCliente(SOCKET cliente);

#endif
