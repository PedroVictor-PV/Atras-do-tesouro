# Go to Treasure

## Descrição

**Go to Treasure** (ou "Jogo Atrás do Tesouro") é um jogo multiplayer cliente-servidor desenvolvido em C para Windows. Os jogadores se conectam a um servidor via TCP/IP e competem para encontrar tesouros escondidos em um tabuleiro 6x6, acumulando pontos registrados em um ranking global thread-safe. O tabuleiro é gerado aleatoriamente, proporcionando um gameplay dinâmico, e o jogo suporta até 20 jogadores simultâneos.

---

## Características Principais

-   **Multiplayer:** Suporte para até 20 jogadores conectados simultaneamente.
-   **Threading:** Cada cliente é gerenciado por uma thread dedicada no servidor.
-   **Ranking Global:** Sistema de pontuação protegido por sincronização.
-   **Interface de Rede:** Comunicação via protocolo TCP personalizado.
-   **Gameplay Dinâmico:** Tabuleiros 6x6 com tesouros aleatórios.
-   **Menu Interativo:** Opções para iniciar o jogo, ver regras, créditos ou acessar um Easter egg.

---

## Regras e Mecânica do Jogo

### Objetivo

Encontrar o máximo de tesouros possível em até **10 tentativas** por sessão.

### Tabuleiro

-   **Tamanho:** 6x6 (36 posições).
-   **Numeração:** Posições de 1 a 36.
-   **Distribuição:** Tesouros são posicionados aleatoriamente no início de cada sessão.

| Tipo | Símbolo | Pontos |
| :--- | :---: | :--- |
| **Tesouro Normal** | `$` | 1 ponto |
| **Tesouro Especial**| `$$` | 2 pontos |
| **Posição Vazia** | `*` | 0 pontos |

### Mecânica

-   O jogador escolhe uma posição (1 a 36) por tentativa.
-   Encontrar um tesouro reorganiza o tabuleiro com novos tesouros aleatórios.
-   Posições já escolhidas não podem ser selecionadas novamente na mesma sessão.
-   Limite de 10 tentativas por sessão.
-   Digite `0` para sair do jogo e retornar ao menu.

### Menu do Cliente

| Opção | Descrição |
| :--- | :--- |
| **INICIAR** | Inicia uma partida de caça ao tesouro. |
| **REGRAS** | Exibe as regras do jogo. |
| **CRÉDITOS** | Mostra informações dos desenvolvedores. |
| **HACK** | Easter egg especial. |

---

## Arquitetura e Tecnologias

### Estrutura de Pastas

```
Go to Treasure/
├── Makefile
├── README.md
├── include/
│   ├── core/
│   │   └── jogo.h
│   ├── network/
│   │   ├── cliente_handler.h
│   │   └── servidor.h
│   ├── threads/
│   │   └── threads.h
│   └── utils/
│       └── matriz.h
├── src/
│   ├── Main.c
│   ├── client.c
│   ├── core/
│   │   └── jogo.c
│   ├── network/
│   │   ├── cliente_handler.c
│   │   └── servidor.c
│   ├── threads/
│   │   └── threads.c
│   └── util/
│       └── matriz.c
└── bin/
    ├── servidor.exe
    └── client.exe
```

### Tecnologias Utilizadas

-   **Linguagem e APIs:**
    -   **C:** Linguagem principal para servidor e cliente.
    -   **Winsock2:** Comunicação TCP/IP.
    -   **Windows API:** Threading e sincronização (`<windows.h>`).
    -   **Standard Library:** Manipulação de strings, I/O e gerenciamento de memória.
    -   **Time:** Semente para geração de números aleatórios (`<time.h>`).

### Estruturas de Dados

#### Jogador
```c
typedef struct {
    char nome[100]; // Nome do jogador
    int pontos;     // Pontuação total
} Jogador;
```

#### Argumentos de Thread
```c
typedef struct {
    SOCKET client; // Socket da conexão
} ThreadArgs;
```

#### Variáveis Globais Compartilhadas
```c
CRITICAL_SECTION csRanking;      // Proteção do ranking
Jogador ranking[LIMITE_CLIENTES];  // Array de jogadores (máx. 20)
int totalJogadores = 0;          // Contador total
volatile LONG clientesConectados = 0; // Contador atômico
```

#### Thread Safety
-   **Critical Section:** Protege o acesso ao ranking global.
-   **Atomic Operations:** Gerencia o contador de clientes conectados de forma segura.
-   **Thread-local Data:** Cada thread possui suas próprias matrizes de jogo.

---

## Instalação e Uso

### Pré-requisitos

-   Sistema operacional **Windows** (devido ao uso de Winsock2 e Windows.h).
-   Compilador **GCC (MinGW)** ou Visual Studio.
-   **Windows SDK** (para a biblioteca `ws2_32.lib`).

### Compilação

Use o Makefile para simplificar o processo:

```bash
# Compilar servidor e cliente
make all

# Limpar arquivos de build
make clean
```

Ou compile manualmente:
```bash
# Servidor
gcc -Iinclude src/Main.c src/core/jogo.c src/network/cliente_handler.c src/network/servidor.c src/threads/threads.c src/util/matriz.c -o bin/servidor.exe -lws2_32

# Cliente
gcc -Iinclude src/client.c -o bin/client.exe -lws2_32
```

### Execução

1.  **Inicie o Servidor:**
    ```bash
    ./bin/servidor.exe
    ```
    *Saída esperada:* `Servidor aguardando conexoes...`

2.  **Execute o Cliente:**
    ```bash
    ./bin/client.exe
    ```
    *Alternativamente, use um cliente Telnet:* `telnet 127.0.0.1 8888`

3.  **Inicie o Jogo:**
    -   Execute o servidor.
    -   Conecte dois ou mais clientes.
    -   No console do servidor, digite `S` e pressione Enter para iniciar a partida.
    -   Nos clientes, escolha uma opção do menu para jogar.

---

## Protocolo de Comunicação

| Tag | Descrição |
| :--- | :--- |
| **`[INPUT]`** | Solicita entrada do usuário. |
| **`[IAGUARDE]`** | Mensagem de espera para o cliente. |
| **`[CLEAR]`** | Limpa a tela e exibe novo conteúdo. |
| **`[FIMJOGO]`** | Indica o fim da partida. |
| **`[VOLTAR]`** | Permite retorno ao menu principal. |
| **`[INVALID]`** | Indica entrada inválida. |
| **`[INICIOJOGO]`** | Notifica o início do jogo. |

#### Exemplo de Sessão do Cliente
```
[INPUT]
1 - INICIAR
2 - REGRAS
3 - CRÉDITOS
4 - HACK
Escolha uma opção: 1

[CLEAR]
Pontos: 0
Tentativas restantes: 4
Para SAIR digite 0.
Escolha uma posicao - [1 | 36]: 15

[CLEAR]
Voce encontrou um tesouro!
```

---

## Desenvolvimento

### Limitações Atuais
-   **Técnicas:** Máximo de 20 clientes, porta fixa (`8888`), exclusivo para Windows, sem persistência de dados.
-   **Gameplay:** Limite fixo de 10 tentativas, tabuleiro fixo 6x6, sem níveis de dificuldade.

### Melhorias Futuras
-   **Funcionalidades:** Múltiplos níveis, sistema de autenticação, persistência do ranking, chat e modo torneio.
-   **Técnicas:** Suporte multiplataforma (Linux/Mac), configuração via arquivo, logging detalhado e interface gráfica.
-   **Performance:** Pool de threads, otimização de memória e balanceamento de carga.

### Troubleshooting

| Problema | Causa | Solução |
| :--- | :--- | :--- |
| **"Servidor cheio"** | Limite de 20 clientes atingido. | Aumentar `LIMITE_CLIENTES` no código ou aguardar. |
| **Erro de `bind`** | Porta 8888 já em uso. | Fechar o programa que usa a porta ou mudar no código. |
| **Falha de thread**| Recursos do sistema insuficientes. | Reiniciar o sistema ou reduzir o número de clientes. |
| **Erro WSA** | Winsock não inicializado corretamente. | Reinstalar ou reparar o Windows SDK. |

### Contribuição
1.  Faça um **fork** do projeto.
2.  Crie uma nova **branch** (`git checkout -b feature/nova-funcionalidade`).
3.  Faça o **commit** de suas mudanças (`git commit -m 'Adiciona nova funcionalidade'`).
4.  Faça o **push** para a branch (`git push origin feature/nova-funcionalidade`).
5.  Abra um **Pull Request**.

### Desenvolvedores
-   [Pedro Victor Cavalcante De Brito](https://github.com/PedroVictor-PV)
-   [Gabriel Luis dos Reis Feitosa Oliveira](https://github.com/Gabriel-programmer-rookie)
-   [Luis Miguel De Sousa De Castro](https://github.com/LuisMiguelD)
-   [Regis Michael de Sousa Bastos](https://github.com/RegisMichael25)
