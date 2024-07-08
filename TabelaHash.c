#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura do nó da tabela hash
typedef struct NoHash {
    char *chave;
    int valor;
    struct NoHash *proximo;
} NoHash;

// Definição da estrutura da tabela hash
typedef struct TabelaHash {
    NoHash **baldes;
    int tamanho;
} TabelaHash;

// Função hash DJB2
unsigned long hash_DJB2(const char *string) {
    unsigned long hash = 5381;
    int caracter_atual;

    while ((caracter_atual = *string++)) {
        hash = ((hash << 5) + hash) + caracter_atual;
    }

    return hash;
}

// Inicialização da Tabela Hash
TabelaHash* criarTabela(int tamanho) {
    TabelaHash *tabela = (TabelaHash *)malloc(sizeof(TabelaHash));
    tabela->tamanho = tamanho;
    tabela->baldes = (NoHash **)malloc(sizeof(NoHash *) * tamanho);
    for (int i = 0; i < tamanho; i++)
        tabela->baldes[i] = NULL;
    return tabela;
}

// Inserção de Valores
void inserirValor(TabelaHash *tabela, char *chave, int valor) {
    unsigned long indice = hash_DJB2(chave) % tabela->tamanho;
    NoHash *novo_no = (NoHash *)malloc(sizeof(NoHash));
    novo_no->chave = strdup(chave);
    novo_no->valor = valor;
    novo_no->proximo = tabela->baldes[indice];
    tabela->baldes[indice] = novo_no;
}

// Busca de Valores
int buscarValor(TabelaHash *tabela, char *chave) {
    unsigned long indice = hash_DJB2(chave) % tabela->tamanho;
    NoHash *no = tabela->baldes[indice];
    while (no != NULL) {
        if (strcmp(no->chave, chave) == 0)
            return no->valor;
        no = no->proximo;
    }
    return -1; 
}

// Exclusão de Valores
int excluirValor(TabelaHash *tabela, char *chave) {
    unsigned long indice = hash_DJB2(chave) % tabela->tamanho;
    NoHash *no = tabela->baldes[indice];
    NoHash *anterior = NULL;
    
    while (no != NULL && strcmp(no->chave, chave) != 0) {
        anterior = no;
        no = no->proximo;
    }
    if (no == NULL) return 0;
    if (anterior == NULL) 
        tabela->baldes[indice] = no->proximo;
    else
        anterior->proximo = no->proximo;
    free(no->chave);
    free(no);
    return 1;
}

// Função para liberar a memória alocada para a tabela hash
void liberarTabela(TabelaHash *tabela) {
    for (int i = 0; i < tabela->tamanho; i++) {
        NoHash *no = tabela->baldes[i];
        while (no != NULL) {
            NoHash *temp = no;
            no = no->proximo;
            free(temp->chave);
            free(temp);
        }
    }
    free(tabela->baldes);
    free(tabela);
}

// Função principal com o menu
int main() {
    int tamanho = 10;
    TabelaHash *tabela = criarTabela(tamanho);
    int opcao;
    char chave[100];
    int valor;

    do {
        system("CLS");
        printf("\n===========================");
        printf("\n        Tabela Hash        \n");
        printf("===========================\n");
        printf("[1] Inserir valor\n");
        printf("[2] Buscar valor\n");
        printf("[3] Excluir valor\n");
        printf("[0] Sair\n");
        printf("===========================\n");
        printf("Escolha uma opcao --> ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                // Utilizando a função de inserção do valor.
                system("CLS");
                printf("\n=============================\n");
                printf("\n        Inserir Valor        \n");
                printf("\n=============================\n");
                printf("Digite a chave --> ");
                scanf("%s", chave);
                printf("Digite o valor --> ");
                scanf("%d", &valor);
                inserirValor(tabela, chave, valor);
                printf("Valor inserido com sucesso!\n");
                system("PAUSE");
                break;
            case 2:
                // Utilizando a função de busca pelo valor.
                system("CLS");
                printf("\n============================\n");
                printf("\n        Buscar Valor        \n");
                printf("\n============================\n");
                printf("Digite a chave --> ");
                scanf("%s", chave);
                valor = buscarValor(tabela, chave);
                if (valor != -1)
                    printf("\nValor encontrado --> %d\n", valor);
                else
                    printf("\nChave nao encontrada.\n");
                system("PAUSE");
                break;
            case 3:
                // Utilizando a função de exclusão do valor.
                system("CLS");
                printf("\n============================\n");
                printf("\n        Excluir Valor       \n");
                printf("\n============================\n");
                printf("Digite a chave --> ");
                scanf("%s", chave);
                if (excluirValor(tabela, chave))
                    printf("\nValor excluido com sucesso!\n");
                else
                    printf("\nChave nao encontrada.\n");
                system("PAUSE");
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida.\n");
                system("PAUSE");
                break;
        }
    } while (opcao != 0);

    liberarTabela(tabela);
    return 0;
}
