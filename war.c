#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definições de constantes
#define TAM_NOME 30
#define TAM_COR 10

// --- Estrutura de Dados ---
/**
 * @brief Representa um território no jogo.
 */
struct Territorio {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
};

// --- Protótipos das Funções ---
void limparBufferEntrada();
void cadastrarTerritorios(struct Territorio *mapa, int totalTerritorios);
void exibirTerritorios(const struct Territorio *mapa, int totalTerritorios);
int rolarDado();
void atacar(struct Territorio *atacante, struct Territorio *defensor);
void simularJogo(struct Territorio *mapa, int totalTerritorios);
void liberarMemoria(struct Territorio *mapa);

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

// --- Funções Principais de Gerenciamento ---

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


/**
 * @brief Gerencia a interação do jogo e os ataques.
 * @param mapa Ponteiro para o array de structs dos territórios.
 * @param totalTerritorios Número de territórios.
 */
void simularJogo(struct Territorio *mapa, int totalTerritorios) {
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

            // Chamada da função de ataque, usando aritmética de ponteiros
            // (mapa + (indice - 1)) retorna o endereço da struct desejada.
            atacar(mapa + (indiceAtacante - 1), mapa + (indiceDefensor - 1));

        } else if (opcao != 0) {
            printf("Opção inválida. Tente novamente.\n");
        }
    }
}

/**
 * @brief Libera a memória alocada dinamicamente para o mapa.
 * @param mapa Ponteiro para o array de structs.
 */
void liberarMemoria(struct Territorio *mapa) {
    if (mapa != NULL) {
        free(mapa);
        printf("\nMemória alocada dinamicamente liberada com sucesso.\n");
    }
}

// --- Função Principal (Main) ---

int main() {
    // Inicializa a semente para a geração de números aleatórios (dados)
    srand(time(NULL)); 
    
    struct Territorio *mapa = NULL;
    int totalTerritorios = 0;
    
    printf("=======================================\n");
    printf("         SIMULADOR DE WAR EM C         \n");
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
        printf("ERRO: Falha ao alocar memória.\n");
        return 1;
    }
    
    // 2. Cadastro Inicial
    // O ponteiro 'mapa' é passado para a função
    cadastrarTerritorios(mapa, totalTerritorios);
    
    // 3. Simulação de Ataques
    simularJogo(mapa, totalTerritorios);

    printf("\n----------------- FIM DE JOGO ---------------------\n");
    
    // 4. Liberação de Memória
    liberarMemoria(mapa);

    return 0;
}