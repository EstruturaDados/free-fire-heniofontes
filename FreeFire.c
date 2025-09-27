#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Código da Ilha – Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.

// Definição da struct Item para representar cada item do inventário
struct Item {
    char nome[30];
    char tipo[20];
    int quantidade;
    int prioridade; // Prioridade indica a importância do item na montagem do plano de fuga (1 a 5)
};

// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).
enum CriterioOrdenacao {
    POR_NOME,
    POR_TIPO,
    POR_PRIORIDADE
};

// Vetor mochila:
// Armazena até 10 itens coletados.
struct Item mochila[10];
int numItens = 0; // Variáveis de controle: numItens (quantidade atual)
int comparacoes = 0; // Variável para análise de desempenho
bool ordenadaPorNome = false; // Para controle da busca binária
enum CriterioOrdenacao criterioAtual = POR_NOME;

// Protótipos das funções
void limparTela();
void exibirMenu();
void inserirItem();
void removerItem();
void listarItens();
void menuDeOrdenacao();
void insertionSort(enum CriterioOrdenacao criterio);
void buscaBinariaPorNome();
int compararItens(const struct Item *a, const struct Item *b, enum CriterioOrdenacao criterio);
void limparBuffer();

int main() {
    // Menu principal com opções:
    // 1. Adicionar um item
    // 2. Remover um item
    // 3. Listar todos os itens
    // 4. Ordenar os itens por critério (nome, tipo, prioridade)
    // 5. Realizar busca binária por nome
    // 0. Sair

    printf("=== SISTEMA DE INVENTÁRIO AVANÇADO - JOGO DE SOBREVIVÊNCIA ===\n");
    printf("Bem-vindo! Organize sua mochila para sobreviver na ilha!\n\n");
    
    int opcao;
    
    do {
        exibirMenu();
        printf("\nDigite sua opção: ");
        scanf("%d", &opcao);
        limparBuffer(); // Limpa o buffer do teclado
        
        // A estrutura switch trata cada opção chamando a função correspondente.
        // A ordenação e busca binária exigem que os dados estejam bem organizados.
        switch(opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                menuDeOrdenacao();
                break;
            case 5:
                buscaBinariaPorNome();
                break;
            case 0:
                printf("Saindo do sistema de inventário... Boa sorte na fuga da ilha!\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
        
        printf("\nPressione Enter para continuar...");
        limparBuffer();
        limparTela();
    } while(opcao != 0);
    
    return 0;
}

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.
void limparTela() {
    for (int i = 0; i < 30; i++) {
        printf("\n");
    }
}

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.
void exibirMenu() {
    printf("=== MENU PRINCIPAL ===\n");
    printf("Status: Mochila (%d/10 itens) | ", numItens);
    
    if (ordenadaPorNome) {
        printf("Ordenada por nome");
    } else {
        printf("Não ordenada por nome");
    }
    
    printf("\n");
    printf("1. Adicionar item à mochila\n");
    printf("2. Remover item da mochila\n");
    printf("3. Listar todos os itens\n");
    printf("4. Ordenar itens por critério\n");
    printf("5. Busca binária por nome\n");
    printf("0. Sair do sistema\n");
}

// Função para limpar o buffer de entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".
void inserirItem() {
    if (numItens >= 10) {
        printf("❌ Mochila cheia! Capacidade máxima de 10 itens atingida.\n");
        return;
    }
    
    printf("\n=== ADICIONAR NOVO ITEM ===\n");
    
    // Solicita os dados do item
    printf("Nome do item (ex: AK-47, Kit Médico, Corda): ");
    fgets(mochila[numItens].nome, 30, stdin);
    mochila[numItens].nome[strcspn(mochila[numItens].nome, "\n")] = 0;
    
    printf("Tipo (arma, municao, cura, ferramenta, recurso): ");
    fgets(mochila[numItens].tipo, 20, stdin);
    mochila[numItens].tipo[strcspn(mochila[numItens].tipo, "\n")] = 0;
    
    printf("Quantidade: ");
    scanf("%d", &mochila[numItens].quantidade);
    limparBuffer();
    
    printf("Prioridade (1-5, onde 5 é mais importante): ");
    scanf("%d", &mochila[numItens].prioridade);
    limparBuffer();
    
    // Validação da prioridade
    if (mochila[numItens].prioridade < 1) mochila[numItens].prioridade = 1;
    if (mochila[numItens].prioridade > 5) mochila[numItens].prioridade = 5;
    
    numItens++;
    ordenadaPorNome = false; // Marca como não ordenada após inserção
    printf("✅ Item '%s' adicionado com sucesso! (Prioridade: %d)\n", 
           mochila[numItens-1].nome, mochila[numItens-1].prioridade);
}

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.
void removerItem() {
    if (numItens == 0) {
        printf("❌ Mochila vazia! Não há itens para remover.\n");
        return;
    }
