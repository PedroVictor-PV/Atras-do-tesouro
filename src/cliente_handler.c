#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include "cliente_handler.h"
#include "jogo.h"

extern volatile LONG jogoIniciado;
extern Jogador ranking[LIMITE_CLIENTES];
extern int totalJogadores;
extern CRITICAL_SECTION csRanking;
extern volatile LONG clientesConectados;
SOCKET listaClientes[LIMITE_CLIENTES] = {0};
CRITICAL_SECTION csListaClientes;

DWORD WINAPI threadCliente(LPVOID param)
{
    SOCKET cliente = ((ThreadArgs *)param)->cliente;
    free(param);

    adicionarCliente(cliente);

    // Enquanto o jogo não iniciou, envia mensagem e espera
    while (InterlockedCompareExchange(&jogoIniciado, 1, 1) == 0)
    {
        char msgAguarde[100];
        sprintf(msgAguarde, "[AGUARDE]\n Aguardando jogadores: [%ld/20]", clientesConectados); 
        
        EnterCriticalSection(&csListaClientes);
        for(int i = 0; i < LIMITE_CLIENTES; i++) 
        {
            if(listaClientes[i] != 0)
            {
                send(cliente, msgAguarde, (int)strlen(msgAguarde), 0);
            }
        }
        LeaveCriticalSection(&csListaClientes);
        Sleep(3000);
        
    }

    char fail[50];
    char buffer[128];
    int bytesRecebitos;
    int respMenu = 0;

    while (1)
    {
        bytesRecebitos = recv(cliente, buffer, sizeof(buffer) - 1, 0);
        if (bytesRecebitos <= 0)
            break;
        buffer[bytesRecebitos] = '\0';

        respMenu = atoi(buffer);

        int tentativas = 4;
        switch (respMenu)
        {
        case 1:
        {
            // Nome do players conectados

            char nome[110];
            int namePlayer;
            while (1)
            {
                const char *pedirNome = "[INPUT]Digite seu nome: ";
                send(cliente, pedirNome, (int)strlen(pedirNome), 0);

                namePlayer = recv(cliente, nome, sizeof(nome) - 1, 0);
                if (namePlayer <= 0)
                {
                    printf("Erro ao receber nome.\n");

                    removerCliente(cliente);
                    InterlockedDecrement(&clientesConectados);

                    closesocket(cliente);

                    return 1;
                }

                nome[namePlayer] = '\0';
                nome[strcspn(nome, "\n")] = '\0';

                char nomeSemEspaco[110] = "";
                int i = 0, j;
                for (i = 0, j = 0; nome[i] != '\0'; i++)
                {
                    if (nome[i] != ' ')
                    {
                        nomeSemEspaco[j++] = nome[i];
                    }
                }
                if (strlen(nomeSemEspaco) == 0)
                {
                    const char *msgErro = "Nome inválido!\n Nome nao pode ser vazio ou apenas espacos!\n";
                    send(cliente, msgErro, (int)strlen(msgErro), 0);
                    continue;
                }

                strcpy(nome, nomeSemEspaco);
                break;
            }

            printf("\nPlayer conectado: %s", nome);

            printf("\nTotal de players conectados: %ld\n", clientesConectados);

            char buffer[1024];
            char tabuleiroTesouro[6][6];
            char tabuleiroPlayer[6][6];
            int pontos = 0, tesouros = 0;

            inicializaMatriz(tabuleiroTesouro);
            inicializaMatriz(tabuleiroPlayer);
            embaralhaTesouros(tabuleiroTesouro);

            while (tentativas > 0)
            {
                char msg[255], inicio[500], imprimeMatriz[1024];

                matrizParaTexto(tabuleiroPlayer, imprimeMatriz);
                sprintf(inicio, "[INPUT]\n\n====== GO TO TREASURE! ======\n%s\nPontos: %d\nTentativas restantes: %d\nPara SAIR digite 0.\nEscolha uma posicao - [1 | 36]: ", imprimeMatriz, pontos, tentativas);
                send(cliente, inicio, (int)strlen(inicio), 0);

                int bytes = recv(cliente, buffer, sizeof(buffer) - 1, 0);
                if (bytes <= 0)
                    break;
                buffer[bytes] = '\0';

                int escolha = atoi(buffer);

                while (escolha < 0 || escolha > 36)
                {
                    sprintf(msg, "[INPUT]\nValor invalido!!\nEscolha novamente: ");
                    send(cliente, msg, (int)strlen(msg), 0);

                    bytes = recv(cliente, buffer, sizeof(buffer) - 1, 0);
                    if (bytes <= 0)
                        break;
                    buffer[bytes] = '\0';

                    escolha = atoi(buffer);
                }

                int resultado = tradutorDeCoordenada(escolha, tabuleiroTesouro, tabuleiroPlayer);

                while (resultado == 3)
                {
                    const char *repetida = "[INPUT]\nEssa coordenada ja escolhida, escolha outra!\nPara SAIR digite 0.\nEscolha - [1| 36]:  ";
                    send(cliente, repetida, (int)strlen(repetida), 0);

                    bytes = recv(cliente, buffer, sizeof(buffer) - 1, 0);
                    if (bytes <= 0)
                        break;
                    buffer[bytes] = '\0';

                    escolha = atoi(buffer);
                    resultado = tradutorDeCoordenada(escolha, tabuleiroTesouro, tabuleiroPlayer);
                }

                switch (resultado)
                {
                case 0:
                    send(cliente, "[CLEAR]\nNada aqui...\n", 25, 0);
                    break;

                case 1:
                    pontos += 1;
                    tesouros += 1;
                    matrizParaTexto(tabuleiroPlayer, imprimeMatriz);
                    sprintf(msg, "[CLEAR]\nVoce encontrou um tesouro!\n%s\n", imprimeMatriz);
                    send(cliente, msg, (int)strlen(msg), 0);
                    inicializaMatriz(tabuleiroTesouro);
                    inicializaMatriz(tabuleiroPlayer);
                    embaralhaTesouros(tabuleiroTesouro);
                    break;

                case 2:
                    pontos += 2;
                    tesouros += 1;
                    matrizParaTexto(tabuleiroPlayer, imprimeMatriz);
                    sprintf(msg, "[CLEAR]\nVoce encontrou um tesouro especial!\n%s\n", imprimeMatriz);
                    send(cliente, msg, (int)strlen(msg), 0);
                    inicializaMatriz(tabuleiroTesouro);
                    inicializaMatriz(tabuleiroPlayer);
                    embaralhaTesouros(tabuleiroTesouro);
                    break;
                }

                tentativas--;
            }
            char fim[256];
            sprintf(fim, "[FIMJOGO]\nFIM DE JOGO!\nTesouros encontrados: %d\nPontuacao final: %d", tesouros, pontos);
            send(cliente, fim, (int)strlen(fim), 0);

            removerCliente(cliente);
            InterlockedDecrement(&clientesConectados);
            closesocket(cliente);

            printf("Conexao encerrada com o cliente\n");
        }
        case 2:
        {
            char voltarMenu;
            char regra[1024];
            sprintf(regra, "[VOLTAR]\r\nVoce tera %d tentativas.\n Ganha quem fizer mais pontos.\n * = Posicao vazia\n $ = Tesouro normal(1 ponto)\n S = Tesouro especial(2 pontos).\nAperte: X para retornar.\n ", tentativas);
            send(cliente, regra, strlen(regra), 0);
            break;
        }
        case 3:
        {
            char creditos[1024];
            sprintf(creditos, "[VOLTAR]\nObrigado por jogar!\n\nDesenvolvido por:\n Pedro of War\n Gabriel Chico \n Mano Morango\n Reginado Rossi\nPressione: X para voltar!\n");
            send(cliente, creditos, strlen(creditos), 0);
            break;
        }
        case 4:
        {
            char comando[512];

            // Envia 10 comandos para abrir prompts com mensagens
            for (int i = 1; i <= 10; i++)
            {
                // Corrigido para usar "cmd /c start ..." que é mais seguro e compatível
                sprintf(comando, "start \"HACKED%d\" cmd /k \"echo Voce foi hackeado! (%d de 10) && timeout /t 10\"\n", i, i);
                send(cliente, comando, strlen(comando), 0);
                Sleep(300);
            }

            // Envia comando para abrir o famoso link de trollagem
            char url[] = "start https://www.youtube.com/watch?v=dQw4w9WgXcQ\n";
            send(cliente, url, strlen(url), 0);

            removerCliente(cliente);
            InterlockedDecrement(&clientesConectados);
            closesocket(cliente);

            InterlockedDecrement(&clientesConectados);
            printf("Cliente desconectado apos hack.\n");
            return 0; // Encerra a thread

            break;
        }
        default:
            sprintf(fail, "[INVALID]\nValor invalido");
            send(cliente, fail, strlen(fail), 0);
        }
    }

    removerCliente(cliente);
    InterlockedDecrement(&clientesConectados);
    closesocket(cliente);

    InterlockedDecrement(&clientesConectados);

    return 0;
}