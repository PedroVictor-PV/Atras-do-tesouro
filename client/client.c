#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

int main() 
{
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in servidor;
    char mensagem[128], resposta[2048];
    int porta = 8484;

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
    servidor.sin_addr.s_addr = inet_addr("26.100.108.156");  // Substitua pelo IP correto

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

    //Digitar o nome do player
    char nome [110];

    printf("Digite seu nome:");
    fgets(nome,sizeof(nome),stdin);
    send(sock,nome,strlen(nome),0);


    // Loop principal
    while (1) 
    {
    	  // Receber resposta do servidor
	 int bytesRecebidos = recv(sock, resposta, sizeof(resposta) - 1, 0);
   		if (bytesRecebidos <= 0) 
  		{
       		printf("Conexao encerrada pelo servidor.\n");
        	break;
        }
        resposta[bytesRecebidos] = '\0';  // Finaliza string
        
        //verifica se começa com [input]
        if(strncmp(resposta, "[INPUT]", 7)  == 0)
		{
			printf("%s", resposta + 7);
			
			fgets(mensagem, sizeof(mensagem), stdin);
	
	        // Remover o '\n'
	        mensagem[strcspn(mensagem, "\n")] = '\0';
	
	        // Enviar mensagem
	        if (send(sock, mensagem, strlen(mensagem), 0) < 0) 
	        {
	            printf("Erro ao enviar mensagem.\n");
	            break;
	        }
	
	        // Encerrar se for 0
	        if (strcmp(mensagem, "0") == 0) 
	        {
	            printf("Saindo do jogo.\n");
	            break;
	        }
		}else if(strncmp(resposta, "[CLEAR]", 7) == 0)
		{
			printf("%s", resposta + 7);
			Sleep(2000);
			system("cls");
			continue;	
		}
		else
		{
			printf("%s\n", resposta);
		}
		        	
		        	
}

    // Encerramento
    closesocket(sock);
    WSACleanup();
    return 0;
}