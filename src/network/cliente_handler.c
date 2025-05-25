#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include "network/cliente_handler.h"
#include "core/jogo.h"
#include "utils/matriz_utils.h"
#include "utils/ranking.h"

extern volatile LONG jogoIniciado;
extern Jogador ranking[LIMITE_CLIENTES];
extern CRITICAL_SECTION csRanking;
extern volatile LONG clientesConectados;
// SOCKET listaClientes[LIMITE_CLIENTES] = {0};
extern CRITICAL_SECTION csListaClientes;

DWORD WINAPI threadCliente(LPVOID param)
{
    SOCKET cliente = ((ThreadArgs *)param)->cliente;
    free(param);

    adicionarCliente(cliente);

    // Aguardar início do jogo
    while (InterlockedCompareExchange(&jogoIniciado, 1, 1) == 0)
    {
        char msgAguarde[100];
        LONG clientesAtuais = InterlockedCompareExchange(&clientesConectados, 0, 0);
        sprintf(msgAguarde, "[AGUARDE]\nAguardando jogadores: [%ld/%d]", clientesAtuais, LIMITE_CLIENTES); 
        send(cliente, msgAguarde, (int)strlen(msgAguarde), 0);
        Sleep(3000);
    }

    char buffer[128];
    int bytesRecebidos;
    int respMenu = 0;

    while (1)
    {
        bytesRecebidos = recv(cliente, buffer, sizeof(buffer) - 1, 0);
        if (bytesRecebidos <= 0) {
            printf("Cliente desconectou durante menu\n");
            break;
        }
        buffer[bytesRecebidos] = '\0';

        respMenu = atoi(buffer);

        switch (respMenu)
        {
        case 1:
            if (!processarJogo(cliente)) {
                printf("Erro durante o jogo para cliente\n");
                goto cleanup;
            }
            goto cleanup; // Sai após o jogo
            
        case 2:
            if (!enviarRegras(cliente)) {
                goto cleanup;
            }
            break;
            
        case 3:
            if (!enviarCreditos(cliente)) {
                goto cleanup;
            }
            break;
            
        case 4:
            if (!processarHack(cliente)) {
                goto cleanup;
            }
            goto cleanup; // Sai após hack
            
        default:
            {
                char msgErro[64];
                sprintf(msgErro, "[INVALID]\nValor invalido: %d", respMenu);
                send(cliente, msgErro, strlen(msgErro), 0);
            }
            break;
        }
    }

cleanup:
    removerCliente(cliente);
    InterlockedDecrement(&clientesConectados);
    closesocket(cliente);
    
    LONG clientesRestantes = InterlockedCompareExchange(&clientesConectados, 0, 0);
    printf("Cliente desconectado. Restantes: %ld\n", clientesRestantes);
    
    return 0;
}

bool processarJogo(SOCKET cliente)
{
    char nome[110];
    
    // Solicitar nome do jogador
    if (!solicitarNome(cliente, nome)) {
        return false;
    }

    printf("Player conectado: %s\n", nome);
    printf("Total de players conectados: %ld\n", 
           InterlockedCompareExchange(&clientesConectados, 0, 0));

    // Inicializar jogo
    char tabuleiroTesouro[LINHAS][COLUNAS];
    char tabuleiroPlayer[LINHAS][COLUNAS];
    int pontos = 0, tesouros = 0;
    int tentativas = 4;

    inicializaMatriz(tabuleiroTesouro);
    inicializaMatriz(tabuleiroPlayer);
    embaralhaTesouros(tabuleiroTesouro);

    // Loop principal do jogo
    while (tentativas > 0)
    {
        char buffer[128];
        char imprimeMatriz[1024];
        char inicio[500];

        matrizParaTexto(tabuleiroPlayer, imprimeMatriz);
        sprintf(inicio, "[INPUT]\n\n====== GO TO TREASURE! ======\n%s\nPontos: %d\nTentativas restantes: %d\nPara SAIR digite 0.\nEscolha uma posicao - [1 | 36]: ", 
                imprimeMatriz, pontos, tentativas);
        
        if (send(cliente, inicio, (int)strlen(inicio), 0) < 0) {
            return false;
        }

        int bytes = recv(cliente, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            return false;
        }
        buffer[bytes] = '\0';

        int escolha = atoi(buffer);
        
        // Sair do jogo
        if (escolha == 0) {
            return true;
        }

        // Validar entrada
        while (escolha < 1 || escolha > 36)
        {
            char msg[100];
            sprintf(msg, "[INPUT]\nValor invalido!! Escolha entre 1 e 36: ");
            if (send(cliente, msg, (int)strlen(msg), 0) < 0) {
                return false;
            }

            bytes = recv(cliente, buffer, sizeof(buffer) - 1, 0);
            if (bytes <= 0) {
                return false;
            }
            buffer[bytes] = '\0';
            escolha = atoi(buffer);
            
            if (escolha == 0) {
                return true;
            }
        }

        int resultado = tradutorDeCoordenada(escolha, tabuleiroTesouro, tabuleiroPlayer);

        // Coordenada já escolhida
        while (resultado == 3)
        {
            const char *repetida = "[INPUT]\nEssa coordenada ja foi escolhida!\nEscolha outra [1-36] ou 0 para sair: ";
            if (send(cliente, repetida, (int)strlen(repetida), 0) < 0) {
                return false;
            }

            bytes = recv(cliente, buffer, sizeof(buffer) - 1, 0);
            if (bytes <= 0) {
                return false;
            }
            buffer[bytes] = '\0';
            
            escolha = atoi(buffer);
            if (escolha == 0) {
                return true;
            }
            
            resultado = tradutorDeCoordenada(escolha, tabuleiroTesouro, tabuleiroPlayer);
        }

        // Processar resultado
        char msg[1500];
        switch (resultado)
        {
        case 0:
        if (send(cliente, "[CLEAR]\nNada aqui...\n", 25, 0) < 0) {
            return false;
        }
        Sleep(1000); // Dá tempo do cliente ler antes de atualizar
        break;

        case 1:
            pontos += 1;
            tesouros += 1;
            matrizParaTexto(tabuleiroPlayer, imprimeMatriz);
            sprintf(msg, "[CLEAR]\nVoce encontrou um tesouro!\n%s\n", imprimeMatriz);
            if (send(cliente, msg, (int)strlen(msg), 0) < 0) {
                return false;
            }
            inicializaMatriz(tabuleiroTesouro);
            inicializaMatriz(tabuleiroPlayer);
            embaralhaTesouros(tabuleiroTesouro);
            break;

        case 2:
            pontos += 2;
            tesouros += 1;
            matrizParaTexto(tabuleiroPlayer, imprimeMatriz);
            sprintf(msg, "[CLEAR]\nVoce encontrou um tesouro especial!\n%s\n", imprimeMatriz);
            if (send(cliente, msg, (int)strlen(msg), 0) < 0) {
                return false;
            }
            inicializaMatriz(tabuleiroTesouro);
            inicializaMatriz(tabuleiroPlayer);
            embaralhaTesouros(tabuleiroTesouro);
            break;
        }

        tentativas--;
    }
    system("cls");
            
    // ...fim do jogo...
    RankingEntry ranking[MAX_RANKINGS];
    int count = carregar_ranking(ranking, MAX_RANKINGS);

    adicionar_ao_ranking(ranking, &count, nome, pontos);
    ordenar_ranking(ranking, count);
    salvar_ranking(ranking, count);

    // Exibe o ranking no terminal do servidor
    printf("\n==== RANKING FINAL ====\n");
    exibir_ranking(ranking, count);

    return true;
}

bool solicitarNome(SOCKET cliente, char* nome)
{
    int namePlayer;
    
    while (1)
    {
        const char *pedirNome = "[INPUT]Digite seu nome: ";
        if (send(cliente, pedirNome, (int)strlen(pedirNome), 0) < 0) {
            return false;
        }

        namePlayer = recv(cliente, nome, 109, 0); // Deixar espaço para \0
        if (namePlayer <= 0) {
            printf("Erro ao receber nome do cliente\n");
            return false;
        }

        nome[namePlayer] = '\0';
        nome[strcspn(nome, "\n\r")] = '\0'; // Remove quebras de linha

        // Remover espaços
        char nomeSemEspaco[110] = "";
        int j = 0;
        for (int i = 0; nome[i] != '\0'; i++) {
            if (nome[i] != ' ') {
                nomeSemEspaco[j++] = nome[i];
            }
        }
        nomeSemEspaco[j] = '\0';

        if (strlen(nomeSemEspaco) == 0) {
            const char *msgErro = "[INPUT]Nome inválido! Nome não pode ser vazio.\nDigite novamente: ";
            if (send(cliente, msgErro, (int)strlen(msgErro), 0) < 0) {
                return false;
            }
            continue;
        }

        strcpy(nome, nomeSemEspaco);
        break;
    }
    
    return true;
}

bool enviarRegras(SOCKET cliente)
{
    char regra[1024];
    sprintf(regra, "[VOLTAR]\nVoce tera 4 tentativas.\nGanha quem fizer mais pontos.\n* = Posicao vazia\n$ = Tesouro normal (1 ponto)\nS = Tesouro especial (2 pontos)\n\nAperte X para retornar ao menu.\n");
    return send(cliente, regra, strlen(regra), 0) >= 0;
}

bool enviarCreditos(SOCKET cliente)
{
    char creditos[1024];
    sprintf(creditos, "[VOLTAR]\nObrigado por jogar!\n\nDesenvolvido por:\n- Pedro of War\n- Gabriel Chico\n- Mano Morango\n- Reginaldo Rossi\n\nPressione X para voltar ao menu!\n");
    return send(cliente, creditos, strlen(creditos), 0) >= 0;
}

bool processarHack(SOCKET cliente)
{
    char comando[512];

    // Envia comandos de "hack"
    for (int i = 1; i <= 10; i++)
    {
        sprintf(comando, "start \"HACKED%d\" cmd /k \"echo Voce foi hackeado! (%d de 10) && timeout /t 10\"\n", i, i);
        if (send(cliente, comando, strlen(comando), 0) < 0) {
            return false;
        }
        Sleep(300);
    }

    // Rick Roll
    char url[] = "start https://www.youtube.com/watch?v=dQw4w9WgXcQ\n";
    if (send(cliente, url, strlen(url), 0) < 0) {
        return false;
    }

    printf("Cliente 'hackeado' com sucesso!\n");
    return true;
}

void adicionarCliente(SOCKET cliente) 
{
    EnterCriticalSection(&csListaClientes);
    for (int i = 0; i < LIMITE_CLIENTES; i++) {
        if (listaClientes[i] == 0) {
            listaClientes[i] = cliente;
            break;
        }
    }
    LeaveCriticalSection(&csListaClientes);
}

void removerCliente(SOCKET cliente) 
{
    EnterCriticalSection(&csListaClientes);
    for (int i = 0; i < LIMITE_CLIENTES; i++) {
        if (listaClientes[i] == cliente) {
            listaClientes[i] = 0;
            break;
        }
    }
    LeaveCriticalSection(&csListaClientes);
}