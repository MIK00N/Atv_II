#include <iostream>
#include <string>
#include <vector> 

using namespace std;

// --- Estrutura do Nó (conforme solicitado) ---
struct NoArvore {
    // Dados do nó
    string nome;
    char sexo; // 'M' ou 'F'
    int idade;
    double peso;

    // Ponteiros da árvore
    NoArvore* esquerda;
    NoArvore* direita;

    // Construtor para facilitar a criação
    NoArvore(string n, char s, int i, double p)
        : nome(n), sexo(s), idade(i), peso(p), esquerda(nullptr), direita(nullptr) {}
};

/**
 * @brief Classe que implementa a Árvore Binária de Busca (BST).
 */
class ArvoreBinariaBusca {
private:
    NoArvore* raiz; // Este é o ponteiro 'RAIZ' que o enunciado menciona

    void _gerarListasRecursivo(NoArvore* no,
                                vector<NoArvore*>& homens,
                                vector<NoArvore*>& mulheres) {
        
        // Caso Base: Se o nó é nulo, não faz nada e retorna.
        if (no == nullptr) {
            return;
        }

        // 1. Passo Recursivo: Visita a sub-árvore da ESQUERDA
        _gerarListasRecursivo(no->esquerda, homens, mulheres);

        // 2. Processa o nó "RAIZ" (o nó atual)
        // Adiciona o nó à lista correta.
        // Como estamos em-ordem, ele será adicionado na posição correta.
        if (no->sexo == 'M') {
            homens.push_back(no);
        } else if (no->sexo == 'F') {
            mulheres.push_back(no);
        }
        // Nota: Nós com sexo != 'M' ou 'F' seriam ignorados.

        // 3. Passo Recursivo: Visita a sub-árvore da DIREITA
        _gerarListasRecursivo(no->direita, homens, mulheres);
    }


    // --- Funções auxiliares para o exemplo funcionar ---

    
    NoArvore* _inserirRecursivo(NoArvore* no, NoArvore* novoNo) {
        if (no == nullptr) {
            return novoNo; // Encontrou a posição, insere o novo nó
        }

        // Compara por NOME para decidir para onde ir
        if (novoNo->nome < no->nome) {
            no->esquerda = _inserirRecursivo(no->esquerda, novoNo);
        } else {
            // Permite nomes duplicados (ou maiores) à direita
            no->direita = _inserirRecursivo(no->direita, novoNo);
        }
        return no;
    }

    
    void _destruirRecursivo(NoArvore* no) {
        if (no != nullptr) {
            _destruirRecursivo(no->esquerda);
            _destruirRecursivo(no->direita);
            delete no;
        }
    }

public:
    // Construtor: inicializa a árvore como vazia
    ArvoreBinariaBusca() : raiz(nullptr) {}

    // Destrutor: limpa a memória
    ~ArvoreBinariaBusca() {
        _destruirRecursivo(raiz);
    }

    
    void inserir(string nome, char sexo, int idade, double peso) {
        NoArvore* novoNo = new NoArvore(nome, sexo, idade, peso);
        raiz = _inserirRecursivo(raiz, novoNo);
    }

    
    void gerarListasPorSexo(vector<NoArvore*>& listaHomens,
                            vector<NoArvore*>& listaMulheres) {
        
        // Garante que as listas estejam vazias antes de começar
        listaHomens.clear();
        listaMulheres.clear();

        // Chama a função recursiva privada, começando pela raiz
        _gerarListasRecursivo(raiz, listaHomens, listaMulheres);
    }
};

// --- Função auxiliar para imprimir as listas ---
void imprimirLista(const string& titulo, const vector<NoArvore*>& lista) {
    cout << "--- " << titulo << " ---" << endl;
    if (lista.empty()) {
        cout << "(Lista vazia)" << endl;
        return;
    }
    // Itera pela lista e imprime os dados
    for (const NoArvore* no : lista) {
        cout << "  Nome: " << no->nome
                  << " (Sexo: " << no->sexo
                  << ", Idade: " << no->idade << ")" << endl;
    }
    cout << endl;
}

// --- Função principal (main) para testar o código ---
int main() {

    ArvoreBinariaBusca bst;
   
    bst.inserir("Mario", 'M', 30, 80.5);
    bst.inserir("Ana", 'F', 25, 60.0);
    bst.inserir("Bruno", 'M', 40, 90.1);
    bst.inserir("Silvia", 'F', 22, 55.3);
    bst.inserir("Carlos", 'M', 50, 78.0);
    bst.inserir("Paula", 'F', 35, 65.8);
    bst.inserir("Zelia", 'F', 45, 70.0);
    bst.inserir("Abel", 'M', 19, 70.2);

    
    vector<NoArvore*> listaHomens;
    vector<NoArvore*> listaMulheres;

    // Executa o algoritmo principal
    bst.gerarListasPorSexo(listaHomens, listaMulheres);

    // Imprime os resultados
    // As listas já estarão ordenadas por nome.
    imprimirLista("Lista de Homens (Ordenada por NOME)", listaHomens);
    cout<<"A"<<endl;
    imprimirLista("Lista de Mulheres (Ordenada por NOME)", listaMulheres);

    return 0;
}