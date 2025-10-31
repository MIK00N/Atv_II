#include <iostream>
#include <stdexcept> // Para lançar exceções

using namespace std;

struct No {
    int dados;     
    No* esquerda;  
    No* direita;   

  
    No(int valor) : dados(valor), esquerda(nullptr), direita(nullptr) {}
};


class ArvoreBinariaBusca {
private:
    No* raiz; 

    int encontrarMaxRecursivo(No* no) {
        
        if (no->direita == nullptr) {
            return no->dados;
        }
        
        return encontrarMaxRecursivo(no->direita);
    }

    // --- Funções auxiliares para o exemplo funcionar ---

    /**
     * @brief Função recursiva privada para inserir um valor.
     */
    No* inserirRecursivo(No* no, int valor) {
        // Se a árvore/sub-árvore estiver vazia, cria um novo nó
        if (no == nullptr) {
            return new No(valor);
        }

        // Senão, desce pela árvore
        if (valor < no->dados) {
            no->esquerda = inserirRecursivo(no->esquerda, valor);
        } else {
            // Nota: Permite valores duplicados à direita
            no->direita = inserirRecursivo(no->direita, valor);
        }
        return no;
    }

    /**
     * @brief Função recursiva privada para deletar todos os nós (limpeza).
     */
    void destruirRecursivo(No* no) {
        if (no != nullptr) {
            destruirRecursivo(no->esquerda);
            destruirRecursivo(no->direita);
            delete no;
        }
    }

public:
    // Construtor: inicializa a árvore como vazia
    ArvoreBinariaBusca() : raiz(nullptr) {}

    // Destrutor: limpa a memória
    ~ArvoreBinariaBusca() {
        destruirRecursivo(raiz);
    }

   
    void inserir(int valor) {
        raiz = inserirRecursivo(raiz, valor);
    }


    int encontrarMaiorValor() {
        // Trata o caso da árvore estar vazia
        if (raiz == nullptr) {
            throw runtime_error("Erro: A árvore está vazia.");
        }
        // Chama a função recursiva privada começando pela raiz
        return encontrarMaxRecursivo(raiz);
    }
};

// --- Função principal (main) para testar o código ---
int main() {
    // Define o encoding do console para UTF-8 (para acentos)
    // (Pode não funcionar em todos os terminais)
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif

    ArvoreBinariaBusca bst;

    // Inserindo valores na árvore
    bst.inserir(50);
    bst.inserir(30);
    bst.inserir(70);
    bst.inserir(20);
    bst.inserir(40);
    bst.inserir(60);
    bst.inserir(80); // <-- Este será o maior valor
    bst.inserir(75);
    bst.inserir(85); // <-- Agora este é o maior
    bst.inserir(55);

    try {
        int maior = bst.encontrarMaiorValor();
        cout << "O maior valor na árvore é: " << maior << endl;
    } catch (const exception& e) {
        cout << e.what() << endl;
    }

    // Teste com árvore vazia
    ArvoreBinariaBusca bstVazia;
    cout << "\nTestando em árvore vazia:" << endl;
    try {
        int maior = bstVazia.encontrarMaiorValor();
        cout << "O maior valor na árvore é: " << maior << endl;
    } catch (const exception& e) {
        cout << e.what() << endl;
    }

    return 0;
}