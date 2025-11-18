#include <iostream>
#include <string>

using namespace std;


struct No {
    int valor;
    No *esquerda;
    No *direita;

    // Construtor
    No(int v) : valor(v), esquerda(nullptr), direita(nullptr) {}
};

class ArvoreBinariaBusca {
private:
    No* raiz;

    
    int _eCheiaRecursivo(No* no) {
        // 1. Caso Base: Árvore vazia é considerada cheia.
        if (no == nullptr) {
            return 1;
        }

        // 2. Caso de Violação: Nó com exatamente UM filho.
        // (Verifica se um é nulo e o outro não é)
        if (no->esquerda == nullptr && no->direita != nullptr) {
            return 0; // Apenas filho à direita
        }
        if (no->esquerda != nullptr && no->direita == nullptr) {
            return 0; // Apenas filho à esquerda
        }

        // 3. Caso de Continuação (Nó é folha ou tem 2 filhos):
        // Verifica recursivamente as sub-árvores esquerda E direita.
        // Se for uma folha, ambas as chamadas serão em 'nullptr',
        // retornarão 1 (pelo Caso Base 1), e (1 && 1) = 1.
        return _eCheiaRecursivo(no->esquerda) && _eCheiaRecursivo(no->direita);
    }


    // --- Funções auxiliares para montar a árvore ---

    No* inserirRec(No* no, int valor) {
        if (no == nullptr) {
            return new No(valor);
        }
        if (valor < no->valor) {
            no->esquerda = inserirRec(no->esquerda, valor);
        } else {
            no->direita = inserirRec(no->direita, valor);
        }
        return no;
    }

    void deletarArvore(No* no) {
        if (no == nullptr) return;
        deletarArvore(no->esquerda);
        deletarArvore(no->direita);
        delete no;
    }

public:
    // Construtor
    ArvoreBinariaBusca() : raiz(nullptr) {}

    // Destrutor
    ~ArvoreBinariaBusca() {
        deletarArvore(raiz);
    }

    // Função pública para inserir
    void inserir(int valor) {
        raiz = inserirRec(raiz, valor);
    }

    // -----------------------------------------------------------------
    // --- FUNÇÃO PÚBLICA SOLICITADA ---
    // -----------------------------------------------------------------
    
    int eCheia() {
        // Apenas chama a função recursiva começando pela raiz.
        return _eCheiaRecursivo(raiz);
    }
};

// --- Função principal (main) para testar o código ---
int main() {
    // Define o encoding do console para UTF-8 (para acentos)
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif

    cout << "--- Teste 1: Arvore Vazia ---" << endl;
    ArvoreBinariaBusca arvoreVazia;
    cout << "A arvore eh cheia? (Esperado: 1): " << arvoreVazia.eCheia() << endl;

    cout << "\n--- Teste 2: Arvore Cheia (Perfeita) ---" << endl;
    ArvoreBinariaBusca arvoreCheia;
    arvoreCheia.inserir(10);
    arvoreCheia.inserir(5);  // Filho esquerdo de 10
    arvoreCheia.inserir(15); // Filho direito de 10
    arvoreCheia.inserir(2);  // Filho esquerdo de 5
    arvoreCheia.inserir(7);  // Filho direito de 5
    /*
             10
            /  \
           5    15
          / \
         2   7
    */
    cout << "A arvore eh cheia? (Esperado: 1): " << arvoreCheia.eCheia() << endl;

    cout << "\n--- Teste 3: Arvore NAO Cheia (no 5 so tem 1 filho) ---" << endl;
    ArvoreBinariaBusca arvoreNaoCheia1;
    arvoreNaoCheia1.inserir(10);
    arvoreNaoCheia1.inserir(5);  // Filho esquerdo de 10
    arvoreNaoCheia1.inserir(15); // Filho direito de 10
    arvoreNaoCheia1.inserir(2);  // Filho esquerdo de 5
    // O nó 5 tem apenas 1 filho (o 2). Viola a regra.
    /*
             10
            /  \
           5    15
          /
         2
    */
    cout << "A arvore eh cheia? (Esperado: 0): " << arvoreNaoCheia1.eCheia() << endl;

    cout << "\n--- Teste 4: Arvore NAO Cheia (no 15 so tem 1 filho) ---" << endl;
    ArvoreBinariaBusca arvoreNaoCheia2;
    arvoreNaoCheia2.inserir(10);
    arvoreNaoCheia2.inserir(5);
    arvoreNaoCheia2.inserir(15);
    arvoreNaoCheia2.inserir(20); // Filho direito de 15
    // O nó 15 tem apenas 1 filho (o 20). Viola a regra.
    /*
             10
            /  \
           5    15
                 \
                  20
    */
    cout << "A arvore eh cheia? (Esperado: 0): " << arvoreNaoCheia2.eCheia() << endl;
    
    return 0;
}