#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



// Definições de constantes
#define TAM_NOME 30
#define TAM_COR 10
#define TAM_MISSAO 100
#define MAX_MISSOES 5 // Número de missões pré-definidas

// --- Estrutura de Dados ---
/**
 * @brief Representa um território no jogo.
 */
struct Territorio {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
};

/**
 * @brief Representa um jogador/exército no jogo, que possui uma missão.
 */
struct Jogador {
    char cor[TAM_COR];
    char *missao; // Armazenamento dinâmico da missão
    int missaoCumprida;
};

// --- Protótipos das Funções ---
void limparBufferEntrada();
int rolarDado();

// Funções de Gerenciamento de Territórios
void cadastrarTerritorios(struct Territorio *mapa, int totalTerritorios);
void exibirTerritorios(const struct Territorio *mapa, int totalTerritorios);
void atacar(struct Territorio *atacante, struct Territorio *defensor);

// Funções de Missão e Jogadores
void inicializarJogadores(struct Territorio *mapa, int totalTerritorios, struct Jogador **jogadores, int *totalJogadores);
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes);
void exibirMissao(const char* missao);
int verificarMissao(const char* missao, const struct Territorio* mapa, int totalTerritorios, const char* corJogador);

// Funções do Jogo e Memória
void simularJogo(struct Territorio *mapa, int totalTerritorios, struct Jogador *jogadores, int totalJogadores);
void liberarMemoria(struct Territorio *mapa, struct Jogador *jogadores, int totalJogadores);

// --- Vetor de Missões Pré-Definidas ---
const char *MISSOES[MAX_MISSOES] = {
    "Conquistar 4 territórios seguidos em um único turno.",
    "Eliminar completamente todas as tropas do exército 'Vermelho'.",
    "Conquistar 5 territórios que possuam mais de 3 tropas.",
    "Possuir 3 territórios na América do Sul e 2 na África.", // Simplificação: verificar por nome específico
    "Conquistar um total de 7 territórios ao longo do jogo."
};

// --- Funções Auxiliares ---

/**
 * @brief Limpa o buffer de entrada do teclado após o uso de scanf.
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Simula o lançamento de um dado de 6 faces.
 * @return Um número inteiro aleatório entre 1 e 6.
 */
int rolarDado() {
    // rand() % 6 gera números de 0 a 5. Adicionando 1, temos de 1 a 6.
    return (rand() % 6) + 1;
}

// --- Funções Principais de Gerenciamento de Territórios ---

/**
 * @brief Aloca memória e solicita o cadastro inicial dos territórios.
 * @param mapa Ponteiro para o início do array de structs.
 * @param totalTerritorios Número de territórios a serem cadastrados.
 */
void cadastrarTerritorios(struct Territorio *mapa, int totalTerritorios) {
    printf("\n--- FASE DE CADASTRO DE TERRITÓRIOS ---\n");
    for (int i = 0; i < totalTerritorios; i++) {
        // Usa ponteiros para acessar e modificar a struct na posição 'i'
        struct Territorio *t = mapa + i; 
        
        printf("\nTerritório %d:\n", i + 1);

        printf("  Nome: ");
        fgets(t->nome, TAM_NOME, stdin);
        t->nome[strcspn(t->nome, "\n")] = 0; // Remove o '\n'

        printf("  Cor do Exército (Dono): ");
        fgets(t->cor, TAM_COR, stdin);
        t->cor[strcspn(t->cor, "\n")] = 0;

        printf("  Número de Tropas (Mínimo 1): ");
        if (scanf("%d", &(t->tropas)) != 1 || t->tropas < 1) {
            t->tropas = 1; // Garante que tem pelo menos 1 tropa
            printf("  (Valor inválido, definido para 1 tropa)\n");
        }
        limparBufferEntrada();
    }
}

/**
 * @brief Exibe o estado atual de todos os territórios.
 * @param mapa Ponteiro constante para o array de structs (não permite alteração).
 * @param totalTerritorios Número de territórios.
 */
void exibirTerritorios(const struct Territorio *mapa, int totalTerritorios) {
    printf("\n======================================= ");
    printf("\n      MAPA DO MUNDO - ESTADO ATUAL      ");
    printf("\n======================================= \n");

    for (int i = 0; i < totalTerritorios; i++) {
        const struct Territorio *t = mapa + i;
        printf("\n[%d] TERRITÓRIO: %s", i + 1, t->nome);
        printf("\n    - Dono: Exército %s", t->cor);
        printf("\n    - Tropas: %d", t->tropas);
    }
    printf("\n\n");
}

/**
 * @brief Simula uma batalha entre dois territórios e atualiza seus dados.
 * @param atacante Ponteiro para a struct do território atacante.
 * @param defensor Ponteiro para a struct do território defensor.
 */
void atacar(struct Territorio *atacante, struct Territorio *defensor) {
    printf("\n--- SIMULAÇÃO DE ATAQUE ---\n");
    printf("%s (%s) ataca %s (%s)!\n", atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    // Requisito: Não pode atacar o próprio território
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("ERRO: Você não pode atacar um território da sua própria cor (%s)!\n", atacante->cor);
        return;
    }
    
    // Requisito: O atacante precisa de pelo menos 2 tropas para atacar (1 para atacar, 1 para ficar)
    if (atacante->tropas < 2) {
        printf("ATAQUE REJEITADO: O atacante %s precisa de no mínimo 2 tropas para atacar.\n", atacante->nome);
        return;
    }

    // Lançamento de dados (apenas um dado para simplificar a lógica)
    int dadoAtacante = rolarDado();
    int dadoDefensor = rolarDado();

    printf("  Dado Atacante: %d\n", dadoAtacante);
    printf("  Dado Defensor: %d\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        // Ataque VENCEU!
        printf("\n*** VITÓRIA DO ATACANTE! ***\n");

        // Requisito: O território defensor deve mudar de dono (cor do exército)
        // Requisito: Transfere a cor
        // NOTE: Se o defensor perder a cor, isso pode afetar a contagem de territórios do jogador original.
        // Na lógica do WAR, o atacante move as tropas para o novo território.
        char corAntiga[TAM_COR];
        strcpy(corAntiga, defensor->cor); // Guarda a cor antiga para possível verificação de eliminação

        strcpy(defensor->cor, atacante->cor);

        // Requisito: Transfere metade das tropas para o território defensor
        int tropasTransferidas = atacante->tropas / 2;
        if (tropasTransferidas == 0) tropasTransferidas = 1; // Garante pelo menos 1
        
        atacante->tropas -= tropasTransferidas;
        defensor->tropas = tropasTransferidas; // O defensor recebe as tropas transferidas
        
        printf("%s agora pertence ao exército %s!\n", defensor->nome, defensor->cor);
        printf("  %d tropas foram transferidas do atacante.\n", tropasTransferidas);
    
    } else {
        // Ataque PERDEU ou EMPATE (defensor vence no empate)
        printf("\n*** VITÓRIA DO DEFENSOR (ou empate)! ***\n");

        // Requisito: Se perder, o atacante perde uma tropa.
        atacante->tropas -= 1;
        
        printf("O atacante %s perdeu 1 tropa.\n", atacante->nome);
    }
    
    // Garante que o número mínimo de tropas é 1 (após as perdas)
    if (atacante->tropas < 1) atacante->tropas = 1;
    if (defensor->tropas < 1) defensor->tropas = 1;

    // Exibição pós-ataque
    printf("\n-- ESTADO ATUAL --\n");
    printf("  %s: Tropas %d | Dono %s\n", atacante->nome, atacante->tropas, atacante->cor);
    printf("  %s: Tropas %d | Dono %s\n", defensor->nome, defensor->tropas, defensor->cor);
}


// --- Funções de Missão e Jogadores ---

/**
 * @brief Identifica as cores de exército únicas e as inicializa como Jogadores, atribuindo uma missão.
 * @param mapa Ponteiro para o array de territórios.
 * @param totalTerritorios Número total de territórios.
 * @param jogadores Ponteiro para o ponteiro do array de Jogadores (saída).
 * @param totalJogadores Ponteiro para o total de jogadores (saída).
 */
void inicializarJogadores(struct Territorio *mapa, int totalTerritorios, struct Jogador **jogadores, int *totalJogadores) {
    // 1. Contar cores únicas e armazená-las temporariamente
    char coresUnicas[totalTerritorios][TAM_COR];
    int count = 0;

    for (int i = 0; i < totalTerritorios; i++) {
        int corExiste = 0;
        for (int j = 0; j < count; j++) {
            if (strcmp(mapa[i].cor, coresUnicas[j]) == 0) {
                corExiste = 1;
                break;
            }
        }
        if (!corExiste) {
            strcpy(coresUnicas[count], mapa[i].cor);
            count++;
        }
    }

    *totalJogadores = count;

    // 2. Alocar memória para os jogadores (struct Jogador)
    *jogadores = (struct Jogador *)calloc(count, sizeof(struct Jogador));
    if (*jogadores == NULL) {
        printf("ERRO: Falha ao alocar memória para jogadores.\n");
        exit(1);
    }

    // 3. Inicializar cada jogador e atribuir missão
    printf("\n--- ATRIBUIÇÃO DE MISSÕES ---\n");
    for (int i = 0; i < count; i++) {
        struct Jogador *j = &(*jogadores)[i];

        strcpy(j->cor, coresUnicas[i]);
        j->missaoCumprida = 0;

        // Armazenamento dinâmico da missão (Requisito: usar malloc/calloc)
        j->missao = (char *)malloc(TAM_MISSAO * sizeof(char));
        if (j->missao == NULL) {
            printf("ERRO: Falha ao alocar memória para a missão do jogador %s.\n", j->cor);
            // Liberar memória já alocada antes de sair
            for (int k = 0; k < i; k++) free((*jogadores)[k].missao);
            free(*jogadores);
            exit(1);
        }
        
        // Sorteio e cópia da missão
        atribuirMissao(j->missao, MISSOES, MAX_MISSOES);
        
        printf("\nJogador %s (Cor %s) recebeu a missão:\n", j->cor, j->cor);
        exibirMissao(j->missao); // Requisito: Passagem por valor (para exibição)
    }
}

/**
 * @brief Sorteia uma missão do vetor e copia para o destino.
 * @param destino Ponteiro de destino (string) onde a missão será copiada.
 * @param missoes Vetor de ponteiros para strings (missões pré-definidas).
 * @param totalMissoes Número total de missões no vetor.
 */
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes) {
    // Sorteia um índice entre 0 e totalMissoes - 1
    int indiceSorteado = rand() % totalMissoes;
    // Cópia da string sorteada para a variável de missão do jogador
    strcpy(destino, missoes[indiceSorteado]);
}

/**
 * @brief Exibe a missão do jogador (Passagem por valor).
 * @param missao A string da missão.
 */
void exibirMissao(const char* missao) {
    printf("  [OBJETIVO]: %s\n", missao);
}


/**
 * @brief Verifica se a missão de um jogador foi cumprida.
 * * NOTE: A lógica de verificação é simplificada e deve ser expandida em um jogo real.
 * Aqui, implementaremos a verificação baseada em um número de territórios e na eliminação.
 * * @param missao Ponteiro constante para a string da missão do jogador.
 * @param mapa Ponteiro constante para o array de territórios.
 * @param totalTerritorios Número total de territórios.
 * @param corJogador A cor do exército do jogador.
 * @return 1 se a missão foi cumprida, 0 caso contrário.
 */
int verificarMissao(const char* missao, const struct Territorio* mapa, int totalTerritorios, const char* corJogador) {
    // Lógica simples: Se a missão é 'Conquistar X territórios...'
    // Apenas verificamos se o jogador possui 4 territórios OU se eliminou o Vermelho
    
    // Contagem de territórios do jogador atual
    int countTerritorios = 0;
    for (int i = 0; i < totalTerritorios; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) {
            countTerritorios++;
        }
    }
    
    // Exemplo de verificação simples (Requisito: Lógica simples inicial)

    // 1. Missão: "Conquistar 4 territórios seguidos em um único turno." (Simplificada para 4 no total)
    if (strstr(missao, "Conquistar 4 territórios seguidos")) {
        return countTerritorios >= 4;
    }
    
    // 2. Missão: "Eliminar completamente todas as tropas do exército 'Vermelho'."
    if (strstr(missao, "Eliminar todas as tropas da cor vermelha") || strstr(missao, "exército 'Vermelho'")) {
        // Verifica se a cor "Vermelho" ainda está no mapa.
        int corEliminada = 1;
        for (int i = 0; i < totalTerritorios; i++) {
            if (strcmp(mapa[i].cor, "Vermelho") == 0) {
                corEliminada = 0;
                break;
            }
        }
        // A missão é só para o jogador que não é "Vermelho"
        if (strcmp(corJogador, "Vermelho") != 0) {
            return corEliminada; 
        }
    }

    // 3. Missão: "Conquistar um total de 7 territórios ao longo do jogo."
    if (strstr(missao, "Conquistar um total de 7 territórios")) {
        return countTerritorios >= 7;
    }

    // Se a missão não foi reconhecida ou não tem condição simples, retorna 0
    // Lógica Padrão de Emergência: Conquistar mais da metade dos territórios
    return countTerritorios > (totalTerritorios / 2);
}


// --- Funções do Jogo e Memória ---

/**
 * @brief Gerencia a interação do jogo e os ataques.
 * @param mapa Ponteiro para o array de structs dos territórios.
 * @param totalTerritorios Número de territórios.
 * @param jogadores Ponteiro para o array de structs dos jogadores.
 * @param totalJogadores Número de jogadores.
 */
void simularJogo(struct Territorio *mapa, int totalTerritorios, struct Jogador *jogadores, int totalJogadores) {
    int opcao = 1;
    int indiceAtacante, indiceDefensor;
    
    // Loop principal do jogo
    while (opcao != 0) {
        printf("\n\n----------------- PRÓXIMO TURNO -------------------\n");
        exibirTerritorios(mapa, totalTerritorios);
        
        printf("\nMenu:\n");
        printf("[1] Realizar Ataque\n");
        printf("[0] Sair do Jogo\n");
        printf("Opção: ");
        
        if (scanf("%d", &opcao) != 1) {
            limparBufferEntrada();
            opcao = -1; // Opção inválida
            continue;
        }
        limparBufferEntrada(); // Limpa o buffer após a leitura

        if (opcao == 1) {
            printf("\n--- ESCOLHA DE ATAQUE ---\n");
            
            // Escolha do Território Atacante
            printf("Digite o número do TERRITÓRIO ATACANTE (1 a %d): ", totalTerritorios);
            if (scanf("%d", &indiceAtacante) != 1) {
                printf("Entrada inválida.\n");
                limparBufferEntrada();
                continue;
            }
            limparBufferEntrada();
            
            // Validação do índice
            if (indiceAtacante < 1 || indiceAtacante > totalTerritorios) {
                printf("Índice de território atacante inválido.\n");
                continue;
            }

            // Escolha do Território Defensor
            printf("Digite o número do TERRITÓRIO DEFENSOR (1 a %d): ", totalTerritorios);
            if (scanf("%d", &indiceDefensor) != 1) {
                printf("Entrada inválida.\n");
                limparBufferEntrada();
                continue;
            }
            limparBufferEntrada();
            
            // Validação do índice
            if (indiceDefensor < 1 || indiceDefensor > totalTerritorios) {
                printf("Índice de território defensor inválido.\n");
                continue;
            }
            
            // Validação de ataque a si mesmo
            if (indiceAtacante == indiceDefensor) {
                printf("Você não pode atacar seu próprio território. Tente novamente.\n");
                continue;
            }

            // Chamada da função de ataque
            atacar(mapa + (indiceAtacante - 1), mapa + (indiceDefensor - 1));

            // Requisito: Verificação de Missão
            printf("\n--- VERIFICAÇÃO DE MISSÕES ---\n");
            for(int i = 0; i < totalJogadores; i++) {
                struct Jogador *j = &jogadores[i];
                // Verifica a missão se ainda não foi cumprida
                if (j->missaoCumprida == 0 && verificarMissao(j->missao, mapa, totalTerritorios, j->cor)) {
                    j->missaoCumprida = 1;
                    printf("  *** VITÓRIA! O exército %s cumpriu sua missão! ***\n", j->cor);
                    printf("  Missão: %s\n", j->missao);
                    opcao = 0; // Encerra o jogo
                    break;
                }
            }

        } else if (opcao != 0) {
            printf("Opção inválida. Tente novamente.\n");
        }
    }
}

/**
 * @brief Libera a memória alocada dinamicamente para o mapa e jogadores.
 * @param mapa Ponteiro para o array de structs Territorio.
 * @param jogadores Ponteiro para o array de structs Jogador.
 * @param totalJogadores Número total de jogadores.
 */
void liberarMemoria(struct Territorio *mapa, struct Jogador *jogadores, int totalJogadores) {
    // 1. Libera a memória das missões dos jogadores
    if (jogadores != NULL) {
        for (int i = 0; i < totalJogadores; i++) {
            // Requisito: free() para evitar vazamento de memória
            free(jogadores[i].missao); 
        }
        // 2. Libera a memória do array de jogadores
        free(jogadores);
        printf("\nMemória das missões e jogadores liberada.");
    }

    // 3. Libera a memória do array de territórios (mapa)
    if (mapa != NULL) {
        free(mapa);
        printf("\nMemória do mapa de territórios liberada.");
    }
    printf("\n");
}

// --- Função Principal (Main) ---

int main() {
    // Inicializa a semente para a geração de números aleatórios (dados)
    srand(time(NULL)); 
    
    struct Territorio *mapa = NULL;
    struct Jogador *jogadores = NULL;
    int totalTerritorios = 0;
    int totalJogadores = 0;
    
    printf("=======================================\n");
    printf("        SIMULADOR DE WAR EM C          \n");
    printf("       (Com Missões Estratégicas)      \n");
    printf("=======================================\n");

    // 1. Alocação Dinâmica de Territórios
    printf("\nDigite o número total de territórios para o jogo: ");
    if (scanf("%d", &totalTerritorios) != 1 || totalTerritorios <= 1) {
        printf("Número inválido. O jogo precisa de pelo menos 2 territórios.\n");
        return 1;
    }
    limparBufferEntrada();

    // Requisito: Usar calloc para alocar memória
    mapa = (struct Territorio *) calloc(totalTerritorios, sizeof(struct Territorio));

    if (mapa == NULL) {
        printf("ERRO: Falha ao alocar memória para o mapa.\n");
        return 1;
    }
    
    // 2. Cadastro Inicial
    cadastrarTerritorios(mapa, totalTerritorios);
    
    // 3. Inicializa Jogadores e Atribui Missões
    inicializarJogadores(mapa, totalTerritorios, &jogadores, &totalJogadores);
    
    // 4. Simulação de Ataques
    simularJogo(mapa, totalTerritorios, jogadores, totalJogadores);

    printf("\n----------------- FIM DE JOGO ---------------------\n");
    
    // 5. Liberação de Memória (Requisito: liberar toda a memória alocada dinamicamente)
    liberarMemoria(mapa, jogadores, totalJogadores);

    return 0;
}