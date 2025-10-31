#include <iostream>
#include <string>

// Usar 'using namespace std;' é comum em exercícios acadêmicos para simplificar.
using namespace std;

/**
 * @brief Define a estrutura do Nó da árvore.
 * MODIFICADO: Adicionado 'contador'.
 */
struct No {
    int valor;
    int contador; // <-- MODIFICAÇÃO: Contador de ocorrências
    No *esquerda;
    No *direita;

    // Construtor para facilitar a criação de um novo nó
    No(int v) {
        valor = v;
        contador = 1; // <-- MODIFICAÇÃO: Inicia o contador em 1
        esquerda = nullptr;
        direita = nullptr;
    }
};


class ArvoreBinariaBusca {
private:
    No* raiz; // Ponteiro para o nó raiz da árvore

    No* inserirRec(No* no, int valor) {
        // 1. Caso base: Se o nó é nulo, encontramos o local para inserir.
        if (no == nullptr) {
            return new No(valor); // O construtor já define contador = 1
        }

        // 2. Caso recursivo: Decide se vai para a esquerda ou direita.
        if (valor < no->valor) {
            no->esquerda = inserirRec(no->esquerda, valor);
        } else if (valor > no->valor) {
            no->direita = inserirRec(no->direita, valor);
        } else {
            // 3. <-- MODIFICAÇÃO: Valor duplicado encontrado!
            // Apenas incrementa o contador.
            no->contador++;
        }

        // Retorna o nó (inalterado ou com o novo filho)
        return no;
    }

    void listarEmOrdemRec(No* no) {
        if (no != nullptr) {
            listarEmOrdemRec(no->esquerda);
            // Imprime o valor e o contador
            cout << no->valor << " (" << no->contador << "x) ";
            listarEmOrdemRec(no->direita);
        }
    }

    void listarPreOrdemRec(No* no) {
        if (no != nullptr) {
            // Imprime o valor e o contador
            cout << no->valor << " (" << no->contador << "x) ";
            listarPreOrdemRec(no->esquerda);
            listarPreOrdemRec(no->direita);
        }
    }

    // -----------------------------------------------------------------
    // FUNÇÕES DE REMOÇÃO (Privadas - Recursivas)
    // -----------------------------------------------------------------
    

    No* encontrarMinimo(No* no) {
        No* atual = no;
        while (atual != nullptr && atual->esquerda != nullptr) {
            atual = atual->esquerda;
        }
        return atual;
    }

    No* removerMinimo(No* no) {
        // Caso base: encontramos o menor (não tem filho esquerdo)
        if (no->esquerda == nullptr) {
            No* temp = no->direita; // O substituto dele (pode ser nullptr)
            delete no; // Deleta o nó mínimo
            return temp; // Retorna o substituto
        }
        // Caso recursivo: continua descendo pela esquerda
        no->esquerda = removerMinimo(no->esquerda);
        return no;
    }

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
            // --- MODIFICAÇÃO PRINCIPAL ---
            // Se o contador for maior que 1, apenas o decrementamos.
            if (no->contador > 1) {
                no->contador--;
                return no; // Retorna o nó sem excluí-lo
            }
            // Se o contador for 1, procedemos com a remoção física do nó.
            // -----------------------------

            // CASO 1: Nó com 0 ou 1 filho (à direita)
            if (no->esquerda == nullptr) {
                No* temp = no->direita;
                delete no; // Libera a memória
                return temp; 
            }
            // CASO 2: Nó com 1 filho (à esquerda)
            else if (no->direita == nullptr) {
                No* temp = no->esquerda;
                delete no; // Libera a memória
                return temp; 
            }

            // CASO 3: Nó com 2 filhos
            // 1. Encontra o sucessor Em-Ordem (menor valor na sub-árvore direita)
            No* temp = encontrarMinimo(no->direita);
            
            // 2. Copia os dados do sucessor (valor E contador) para este nó
            no->valor = temp->valor;
            no->contador = temp->contador; // <-- MODIFICAÇÃO: Copia o contador

            // 3. Remove fisicamente o nó sucessor da sub-árvore direita
            // Usamos a nova função 'removerMinimo' para garantir a remoção
            // física, e não apenas decrementar o contador do sucessor.
            no->direita = removerMinimo(no->direita);
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
    // Construtor: Inicializa a árvore com a raiz nula.
    ArvoreBinariaBusca() : raiz(nullptr) {}

    // Destrutor: Limpa toda a memória alocada.
    ~ArvoreBinariaBusca() {
        deletarArvore(raiz);
    }

    // --- INTERFACE PÚBLICA (Sem alterações nas assinaturas) ---

    void inserir(int valor) {
        raiz = inserirRec(raiz, valor);
    }

    void listarEmOrdem() {
        cout << "[Em-Ordem]:   ";
        if (raiz == nullptr) cout << "(Arvore Vazia)";
        listarEmOrdemRec(raiz);
        cout << endl;
    }
    
    void listarPreOrdem() {
        cout << "[Pre-Ordem]:  ";
        if (raiz == nullptr) cout << "(Arvore Vazia)";
        listarPreOrdemRec(raiz);
        cout << endl;
    }

    void remover(int valor) {
        raiz = removerRec(raiz, valor);
    }
};

int main() {
    // Define o encoding do console para UTF-8 (para acentos)
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif

    ArvoreBinariaBusca arvore;
    int valores[] = {20, 10, 30, 5, 15, 20, 10, 5, 20, 40};
    int n = sizeof(valores) / sizeof(valores[0]);

    cout << "------------------------------------------" << endl;
    cout << "a) Inserindo a sequencia com duplicatas: ";
    for (int i = 0; i < n; i++) {
        cout << valores[i] << " ";
        arvore.inserir(valores[i]);
    }
    cout << "\n------------------------------------------" << endl;
    
    cout << "\nArvore apos insercoes:" << endl;
    arvore.listarEmOrdem();   // Deve mostrar contadores
    arvore.listarPreOrdem();  // Deve mostrar contadores

    /*
      Visualização da Árvore (a):
                 20 (3x)
                /   \
           10 (2x)   30 (1x)
           /    \        \
         5 (2x) 15 (1x)   40 (1x)
    */

    cout << "\n------------------------------------------" << endl;
    cout << "b) Iniciando remocoes com contador..." << endl;
    cout << "------------------------------------------" << endl;

    // 1. Removendo o nó 5 (deve apenas decrementar o contador)
    cout << "\n1. Removendo o no 5 (deve ir de 2x para 1x)" << endl;
    arvore.remover(5);
    arvore.listarEmOrdem();
    arvore.listarPreOrdem();

    // 2. Removendo o nó 20 (deve apenas decrementar o contador)
    cout << "\n2. Removendo o no 20 (deve ir de 3x para 2x)" << endl;
    arvore.remover(20);
    arvore.listarEmOrdem();
    arvore.listarPreOrdem();

    // 3. Removendo o nó 5 novamente (deve remover fisicamente o nó)
    cout << "\n3. Removendo o no 5 (deve ir de 1x para 0 -> remocao fisica)" << endl;
    arvore.remover(5);
    arvore.listarEmOrdem();
    arvore.listarPreOrdem();

    // 4. Removendo o nó 30 (nó com 1 filho, contador 1)
    cout << "\n4. Removendo o no 30 (remocao fisica, caso 1 filho)" << endl;
    arvore.remover(30);
    arvore.listarEmOrdem();
    arvore.listarPreOrdem();

    // 5. Removendo o nó 20 novamente (deve ir de 2x para 1x)
    cout << "\n5. Removendo o no 20 (deve ir de 2x para 1x)" << endl;
    arvore.remover(20);
    arvore.listarEmOrdem();
    arvore.listarPreOrdem();

    // 6. Removendo o nó 20 (RAIZ, contador 1, 2 filhos -> remoção física)
    cout << "\n6. Removendo o no 20 (RAIZ, remocao fisica, caso 2 filhos)" << endl;
    arvore.remover(20);
    cout << "Arvore apos remover o 20 (Estado Final):" << endl;
    arvore.listarEmOrdem();
    arvore.listarPreOrdem();
    
    /*
      Árvore após remover 20 (Caso 3):
      O nó 20 é substituído pelo seu sucessor (40).
                 40 (1x)
                /   
           10 (2x)   
              \        
              15 (1x)
    */

    cout << "\n------------------------------------------" << endl;
    cout << "Execucao finalizada." << endl;
    cout << "------------------------------------------" << endl;

    return 0;
}