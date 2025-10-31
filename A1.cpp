#include <iostream>
#include <string>

// Usar 'using namespace std;' é comum em exercícios acadêmicos para simplificar.
using namespace std;

/**
 * @brief Define a estrutura do Nó da árvore.
 * Cada nó contém um valor e ponteiros para os filhos da esquerda e direita.
 */
struct No {
    int valor;
    No *esquerda;
    No *direita;

    // Construtor para facilitar a criação de um novo nó
    No(int v) {
        valor = v;
        esquerda = nullptr;
        direita = nullptr;
    }
};

/**
 * @brief Classe que implementa a Árvore Binária de Busca (BST).
 */
class ArvoreBinariaBusca {
private:
    No* raiz; // Ponteiro para o nó raiz da árvore

    No* inserirRec(No* no, int valor) {
        // 1. Caso base: Se o nó é nulo, encontramos o local para inserir.
        if (no == nullptr) {
            return new No(valor);
        }

        // 2. Caso recursivo: Decide se vai para a esquerda ou direita.
        if (valor < no->valor) {
            no->esquerda = inserirRec(no->esquerda, valor);
        } else if (valor > no->valor) {
            // Assumindo que não inserimos valores duplicados
            no->direita = inserirRec(no->direita, valor);
        }

        // Retorna o nó (inalterado ou com o novo filho)
        return no;
    }

    
    void listarEmOrdemRec(No* no) {
        if (no != nullptr) {
            listarEmOrdemRec(no->esquerda);
            cout << no->valor << " ";
            listarEmOrdemRec(no->direita);
        }
    }

    /**
     * @brief Percurso Pré-Ordem (Pre-Order): Raiz, Esquerda, Direita.
     * Útil para ver a estrutura da árvore como ela foi montada.
     */
    void listarPreOrdemRec(No* no) {
        if (no != nullptr) {
            cout << no->valor << " ";
            listarPreOrdemRec(no->esquerda);
            listarPreOrdemRec(no->direita);
        }
    }

    // -----------------------------------------------------------------
    // F3: FUNÇÃO DE REMOÇÃO (Privada - Recursiva)
    // -----------------------------------------------------------------
    
    /**
     * @brief Encontra o nó com o menor valor na sub-árvore (o sucessor Em-Ordem).
     * Usado na remoção de um nó com dois filhos.
     */
    No* encontrarMinimo(No* no) {
        No* atual = no;
        // O menor valor está sempre no filho mais à esquerda.
        while (atual != nullptr && atual->esquerda != nullptr) {
            atual = atual->esquerda;
        }
        return atual;
    }

    /**
     * @brief Remove um valor da árvore recursivamente.
     * @param no Nó atual na recursão.
     * @param valor O valor a ser removido.
     * @return O nó que substitui o nó removido na árvore.
     */
    No* removerRec(No* no, int valor) {
        // 1. Caso base: Nó nulo, valor não encontrado.
        if (no == nullptr) {
            return no;
        }

        // 2. Procurando o nó a ser removido
        if (valor < no->valor) {
            no->esquerda = removerRec(no->esquerda, valor);
        } else if (valor > no->valor) {
            no->direita = removerRec(no->direita, valor);
        }
        // 3. Nó encontrado (valor == no->valor)
        else {
            // CASO 1: Nó com 0 ou 1 filho (à direita)
            if (no->esquerda == nullptr) {
                No* temp = no->direita;
                delete no; // Libera a memória
                return temp; // Retorna o filho da direita (ou nullptr) para o pai
            }
            // CASO 2: Nó com 1 filho (à esquerda)
            else if (no->direita == nullptr) {
                No* temp = no->esquerda;
                delete no; // Libera a memória
                return temp; // Retorna o filho da esquerda para o pai
            }

            // CASO 3: Nó com 2 filhos
            // 1. Encontra o sucessor Em-Ordem (menor valor na sub-árvore direita)
            No* temp = encontrarMinimo(no->direita);
            
            // 2. Copia o valor do sucessor para este nó
            no->valor = temp->valor;

            // 3. Remove o nó sucessor (que agora é duplicado) da sub-árvore direita
            no->direita = removerRec(no->direita, temp->valor);
        }
        return no;
    }

    /**
     * @brief Libera toda a memória da árvore (usado no destrutor).
     * Faz um percurso Pós-Ordem para deletar.
     */
    void deletarArvore(No* no) {
        if (no == nullptr) return;
        deletarArvore(no->esquerda);
        deletarArvore(no->direita);
        delete no;
    }

public:
    // Construtor: Inicializa a árvore com a raiz nula.
    ArvoreBinariaBusca() : raiz(nullptr) {}

    // Destrutor: Limpa toda a memória alocada.
    ~ArvoreBinariaBusca() {
        deletarArvore(raiz);
    }

    // -----------------------------------------------------------------
    // INTERFACE PÚBLICA (Funções F1, F2, F3)
    // -----------------------------------------------------------------

    /**
     * @brief F1: Insere um valor na árvore.
     */
    void inserir(int valor) {
        raiz = inserirRec(raiz, valor);
    }

    /**
     * @brief F2: Lista a árvore (Em-Ordem).
     * Em-Ordem (In-Order) mostra os elementos de forma ordenada.
     */
    void listarEmOrdem() {
        cout << "[Em-Ordem]:   ";
        listarEmOrdemRec(raiz);
        cout << endl;
    }
    
    /**
     * @brief F2: Lista a árvore (Pré-Ordem).
     * Pré-Ordem (Pre-Order) é útil para ver a estrutura da árvore.
     */
    void listarPreOrdem() {
        cout << "[Pre-Ordem]:  ";
        listarPreOrdemRec(raiz);
        cout << endl;
    }

    /**
     * @brief F3: Remove um valor da árvore.
     */
    void remover(int valor) {
        raiz = removerRec(raiz, valor);
    }
};

/**
 * @brief Função principal para executar os passos do exercício.
 */
int main() {
    // Define a localidade para PT-BR, para imprimir acentos corretamente.
    // setlocale(LC_ALL, "Portuguese"); // Pode ser necessário no Windows
    
    ArvoreBinariaBusca arvore;
    int valores[] = {20, 5, 12, 36, 27, 45, 9, 2, 6, 17, 40};
    int n = sizeof(valores) / sizeof(valores[0]);

    cout << "------------------------------------------" << endl;
    cout << "a) Inserindo a sequencia: ";
    for (int i = 0; i < n; i++) {
        cout << valores[i] << " ";
        arvore.inserir(valores[i]);
    }
    cout << "\n------------------------------------------" << endl;
    
    cout << "\nArvore apos insercoes:" << endl;
    arvore.listarEmOrdem();  // Mostra os valores ordenados
    arvore.listarPreOrdem(); // Mostra a estrutura (Raiz, Esq, Dir)

    /*
     Visualização da Árvore (a):
              20
             /  \
            5    36
           / \   / \
          2  12 27  45
             / \    /
            9  17  40
           /
          6
    */

    cout << "\n------------------------------------------" << endl;
    cout << "b) Iniciando remocoes..." << endl;
    cout << "------------------------------------------" << endl;

    // 1. Removendo o nó 9
    cout << "\n1. Removendo o no 9 (tem 1 filho a esquerda: 6)" << endl;
    arvore.remover(9);
    cout << "Arvore apos remover o 9:" << endl;
    arvore.listarEmOrdem();
    arvore.listarPreOrdem();
    /*
     Árvore após remover 9:
     O nó 12 (pai de 9) adota o 6 (filho de 9).
              20
             /  \
            5    36
           / \   / \
          2  12 27  45
             / \    /
            6  17  40 
    */

    // 2. Removendo o nó 5
    cout << "\n2. Removendo o no 5 (tem 2 filhos: 2 e 12)" << endl;
    arvore.remover(5);
    cout << "Arvore apos remover o 5:" << endl;
    arvore.listarEmOrdem();
    arvore.listarPreOrdem();
    /*
     Árvore após remover 5:
     O nó 5 é substituído pelo seu sucessor (menor da direita: 6).
     O nó 6 é removido da sua posição original (filho esquerdo de 12).
              20
             /  \
            6    36
           / \   / \
          2  12 27  45
             / \    /
           (n) 17  40  (n = nullptr)
    */


    // 3. Removendo o nó 20
    cout << "\n3. Removendo o no 20 (RAIZ, tem 2 filhos: 6 e 36)" << endl;
    arvore.remover(20);
    cout << "Arvore apos remover o 20 (Estado Final):" << endl;
    arvore.listarEmOrdem();
    arvore.listarPreOrdem();
    /*
     Árvore após remover 20:
     O nó 20 é substituído pelo seu sucessor (menor da direita: 27).
     O nó 27 é removido da sua posição original (filho esquerdo de 36).
              27
             /  \
            6    36
           / \   / \
          2  12 (n) 45
             / \    /
           (n) 17  40
    */
    
    cout << "\n------------------------------------------" << endl;
    cout << "Execucao finalizada." << endl;
    cout << "------------------------------------------" << endl;

    return 0;
}