#include <iostream>
#include <string>
#include <algorithm> // Para std::max
#include <limits>    // Para std::numeric_limits

using namespace std;

// --- Estrutura do Nó (Modificada para AVL) ---
struct NoArvore {
    // Dados do nó
    string nome;
    char sexo; // 'M' ou 'F'
    int idade;
    double peso;

    // Ponteiros da árvore
    NoArvore* esquerda;
    NoArvore* direita;

    // Campo essencial para AVL: altura do nó
    int altura;

    // Construtor atualizado
    NoArvore(string n, char s, int i, double p)
        : nome(n), sexo(s), idade(i), peso(p), 
          esquerda(nullptr), direita(nullptr), altura(1) {} // Novas folhas têm altura 1
};

/**
 * @brief Classe que implementa a Árvore AVL.
 */
class ArvoreAVL {
private:
    NoArvore* raiz;

    // --- Funções Auxiliares de Balanceamento (Núcleo do AVL) ---

    // Retorna a altura de um nó (0 se for nulo)
    int getAltura(NoArvore* no) {
        if (no == nullptr) {
            return 0;
        }
        return no->altura;
    }

    // Atualiza a altura de um nó com base em seus filhos
    void atualizarAltura(NoArvore* no) {
        if (no != nullptr) {
            no->altura = 1 + max(getAltura(no->esquerda), getAltura(no->direita));
        }
    }

    // Calcula o Fator de Balanceamento
    int getFatorBalanceamento(NoArvore* no) {
        if (no == nullptr) {
            return 0;
        }
        // Altura da Esquerda - Altura da Direita
        return getAltura(no->esquerda) - getAltura(no->direita);
    }

    // Rotação Simples à Direita (Caso Esquerda-Esquerda)
    NoArvore* rotacaoDireita(NoArvore* y) {
        NoArvore* x = y->esquerda;
        NoArvore* T2 = x->direita;

        // Realiza a rotação
        x->direita = y;
        y->esquerda = T2;

        // Atualiza as alturas (IMPORTANTE: primeiro 'y', depois 'x')
        atualizarAltura(y);
        atualizarAltura(x);

        // Retorna a nova raiz da sub-árvore
        return x;
    }

    // Rotação Simples à Esquerda (Caso Direita-Direita)
    NoArvore* rotacaoEsquerda(NoArvore* x) {
        NoArvore* y = x->direita;
        NoArvore* T2 = y->esquerda;

        // Realiza a rotação
        y->esquerda = x;
        x->direita = T2;

        // Atualiza as alturas (IMPORTANTE: primeiro 'x', depois 'y')
        atualizarAltura(x);
        atualizarAltura(y);

        // Retorna a nova raiz da sub-árvore
        return y;
    }

    // Encontra o nó com o menor valor (mais à esquerda)
    NoArvore* encontrarMinimo(NoArvore* no) {
        NoArvore* atual = no;
        while (atual->esquerda != nullptr) {
            atual = atual->esquerda;
        }
        return atual;
    }


    // --- F1: Inserção (Recursiva com Balanceamento) ---
    NoArvore* _inserir(NoArvore* no, string nome, char sexo, int idade, double peso) {
        // 1. Inserção padrão de BST
        if (no == nullptr) {
            return new NoArvore(nome, sexo, idade, peso);
        }

        if (nome < no->nome) {
            no->esquerda = _inserir(no->esquerda, nome, sexo, idade, peso);
        } else if (nome > no->nome) {
            no->direita = _inserir(no->direita, nome, sexo, idade, peso);
        } else {
            // Nomes duplicados não são permitidos (ou podem ser tratados)
            cout << "Erro: Nome '" << nome << "' ja existe." << endl;
            return no;
        }

        // 2. Atualizar a altura do nó atual
        atualizarAltura(no);

        // 3. Obter o fator de balanceamento
        int fator = getFatorBalanceamento(no);

        // 4. Rebalancear a árvore (Os 4 casos)

        // Caso Esquerda-Esquerda (LL)
        if (fator > 1 && nome < no->esquerda->nome) {
            return rotacaoDireita(no);
        }

        // Caso Direita-Direita (RR)
        if (fator < -1 && nome > no->direita->nome) {
            return rotacaoEsquerda(no);
        }

        // Caso Esquerda-Direita (LR)
        if (fator > 1 && nome > no->esquerda->nome) {
            no->esquerda = rotacaoEsquerda(no->esquerda);
            return rotacaoDireita(no);
        }

        // Caso Direita-Esquerda (RL)
        if (fator < -1 && nome < no->direita->nome) {
            no->direita = rotacaoDireita(no->direita);
            return rotacaoEsquerda(no);
        }

        // Retorna o nó (potencialmente balanceado)
        return no;
    }

    // --- F2: Listagem (Recursiva - Em-Ordem) ---
    // Percurso Em-Ordem (LNR) resulta em listagem alfabética
    void _listarEmOrdem(NoArvore* no) {
        if (no == nullptr) {
            return;
        }
        _listarEmOrdem(no->esquerda);
        cout << "  - Nome: " << no->nome 
             << " (Sexo: " << no->sexo 
             << ", Idade: " << no->idade 
             << ", Peso: " << no->peso << ")" << endl;
        _listarEmOrdem(no->direita);
    }

    // --- F3: Remoção (Recursiva com Balanceamento) ---
    NoArvore* _remover(NoArvore* no, string nome) {
        // 1. Remoção padrão de BST
        if (no == nullptr) {
            cout << "Erro: Nome '" << nome << "' nao encontrado." << endl;
            return no;
        }

        if (nome < no->nome) {
            no->esquerda = _remover(no->esquerda, nome);
        } else if (nome > no->nome) {
            no->direita = _remover(no->direita, nome);
        } else {
            // Nó encontrado!
            // Caso 1: Nó sem filhos ou com um filho
            if (no->esquerda == nullptr || no->direita == nullptr) {
                NoArvore* temp = no->esquerda ? no->esquerda : no->direita;

                if (temp == nullptr) { // Sem filhos
                    temp = no;
                    no = nullptr;
                } else { // Um filho
                    *no = *temp; // Copia o conteúdo do filho
                }
                delete temp;
                cout << "Info: Nome '" << nome << "' removido." << endl;
            } else {
                // Caso 2: Nó com dois filhos
                // Pega o sucessor em-ordem (menor da sub-árvore direita)
                NoArvore* temp = encontrarMinimo(no->direita);

                // Copia os dados do sucessor para este nó
                no->nome = temp->nome;
                no->sexo = temp->sexo;
                no->idade = temp->idade;
                no->peso = temp->peso;

                // Remove o sucessor (que agora é duplicado)
                no->direita = _remover(no->direita, temp->nome);
            }
        }

        // Se a árvore ficou vazia (após remover o único nó)
        if (no == nullptr) {
            return no;
        }

        // 2. Atualizar a altura
        atualizarAltura(no);

        // 3. Obter o fator de balanceamento
        int fator = getFatorBalanceamento(no);

        // 4. Rebalancear (Os 4 casos, ligeiramente diferentes na lógica)

        // Caso Esquerda-Esquerda (LL)
        if (fator > 1 && getFatorBalanceamento(no->esquerda) >= 0) {
            return rotacaoDireita(no);
        }

        // Caso Esquerda-Direita (LR)
        if (fator > 1 && getFatorBalanceamento(no->esquerda) < 0) {
            no->esquerda = rotacaoEsquerda(no->esquerda);
            return rotacaoDireita(no);
        }

        // Caso Direita-Direita (RR)
        if (fator < -1 && getFatorBalanceamento(no->direita) <= 0) {
            return rotacaoEsquerda(no);
        }

        // Caso Direita-Esquerda (RL)
        if (fator < -1 && getFatorBalanceamento(no->direita) > 0) {
            no->direita = rotacaoDireita(no->direita);
            return rotacaoEsquerda(no);
        }

        return no;
    }

    // --- F4: Consulta (Recursiva) ---
    NoArvore* _consultar(NoArvore* no, string nome) {
        if (no == nullptr) {
            return nullptr; // Não encontrado
        }

        if (nome == no->nome) {
            return no; // Encontrado
        }

        if (nome < no->nome) {
            return _consultar(no->esquerda, nome);
        } else {
            return _consultar(no->direita, nome);
        }
    }

    // --- Destrutor Auxiliar ---
    void _destruir(NoArvore* no) {
        if (no != nullptr) {
            _destruir(no->esquerda);
            _destruir(no->direita);
            delete no;
        }
    }


public:
    // Construtor
    ArvoreAVL() : raiz(nullptr) {}

    // Destrutor
    ~ArvoreAVL() {
        _destruir(raiz);
    }

    // F1: Interface pública de Inserção
    void inserir(string nome, char sexo, int idade, double peso) {
        raiz = _inserir(raiz, nome, sexo, idade, peso);
    }

    // F2: Interface pública de Listagem (Em-Ordem)
    void listarEmOrdem() {
        if (raiz == nullptr) {
            cout << "A arvore esta vazia." << endl;
        } else {
            cout << "--- Listagem Em-Ordem (Alfabetica) ---" << endl;
            _listarEmOrdem(raiz);
            cout << "---------------------------------------" << endl;
        }
    }

    // F3: Interface pública de Remoção
    void remover(string nome) {
        raiz = _remover(raiz, nome);
    }

    // F4: Interface pública de Consulta
    void consultar(string nome) {
        NoArvore* resultado = _consultar(raiz, nome);
        if (resultado != nullptr) {
            cout << "--- Consulta Encontrada ---" << endl;
            cout << "  Nome: " << resultado->nome << endl;
            cout << "  Sexo: " << resultado->sexo << endl;
            cout << "  Idade: " << resultado->idade << endl;
            cout << "  Peso: " << resultado->peso << endl;
            cout << "---------------------------" << endl;
        } else {
            cout << "Info: Pessoa com nome '" << nome << "' nao encontrada." << endl;
        }
    }
};


// --- Funções Auxiliares para o Menu (Validação de Entrada) ---

// Limpa o buffer de entrada (cin) em caso de erro
void limparBufferEntrada() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Solicita uma string não vazia
string getString(const string& prompt) {
    string entrada;
    while (true) {
        cout << prompt;
        getline(cin, entrada);
        if (!entrada.empty()) {
            return entrada;
        }
        cout << "Entrada invalida. Tente novamente." << endl;
    }
}

// Solicita um char ('M' ou 'F')
char getChar(const string& prompt) {
    char entrada;
    while (true) {
        cout << prompt;
        cin >> entrada;
        entrada = toupper(entrada); // Converte para maiúscula
        if (entrada == 'M' || entrada == 'F') {
            limparBufferEntrada(); // Limpa o '\n' restante
            return entrada;
        }
        cout << "Entrada invalida. Digite 'M' ou 'F'." << endl;
        limparBufferEntrada();
    }
}

// Solicita um inteiro positivo
int getInt(const string& prompt) {
    int entrada;
    while (true) {
        cout << prompt;
        if (cin >> entrada && entrada > 0) {
            limparBufferEntrada();
            return entrada;
        }
        cout << "Entrada invalida. Digite um numero inteiro positivo." << endl;
        limparBufferEntrada();
    }
}

// Solicita um double positivo
double getDouble(const string& prompt) {
    double entrada;
    while (true) {
        cout << prompt;
        if (cin >> entrada && entrada > 0) {
            limparBufferEntrada();
            return entrada;
        }
        cout << "Entrada invalida. Digite um numero positivo (ex: 70.5)." << endl;
        limparBufferEntrada();
    }
}


// --- Função Principal (Menu de Opções) ---
int main() {
    ArvoreAVL avl;
    int opcao = -1;

    // Dados de exemplo para popular a árvore
    avl.inserir("Mario", 'M', 30, 80.5);
    avl.inserir("Ana", 'F', 25, 60.0);
    avl.inserir("Bruno", 'M', 40, 90.1);
    avl.inserir("Silvia", 'F', 22, 55.3);
    avl.inserir("Carlos", 'M', 50, 78.0);
    avl.inserir("Zelia", 'F', 45, 70.0); // Causa rotação
    
    cout << "Arvore AVL populada com dados iniciais." << endl;

    while (opcao != 0) {
        cout << "\n========== Menu Arvore AVL ==========" << endl;
        cout << "1. Inserir (F1)" << endl;
        cout << "2. Listar (F2 - Em-Ordem)" << endl;
        cout << "3. Remover (F3)" << endl;
        cout << "4. Consultar (F4)" << endl;
        cout << "0. Sair" << endl;
        cout << "=====================================" << endl;
        cout << "Escolha uma opcao: ";

        if (!(cin >> opcao)) {
            cout << "Opcao invalida. Digite um numero." << endl;
            limparBufferEntrada();
            continue;
        }
        limparBufferEntrada(); // Limpa o '\n' após o número

        switch (opcao) {
            case 1: { // Inserção
                cout << "\n--- Inserir Pessoa ---" << endl;
                string nome = getString("Nome: ");
                char sexo = getChar("Sexo (M/F): ");
                int idade = getInt("Idade: ");
                double peso = getDouble("Peso (kg): ");
                avl.inserir(nome, sexo, idade, peso);
                cout << "Info: " << nome << " inserido(a) com sucesso." << endl;
                break;
            }
            case 2: { // Listagem
                cout << "\n--- Listar Pessoas ---" << endl;
                avl.listarEmOrdem();
                break;
            }
            case 3: { // Remoção
                cout << "\n--- Remover Pessoa ---" << endl;
                string nome = getString("Nome para remover: ");
                avl.remover(nome);
                break;
            }
            case 4: { // Consulta
                cout << "\n--- Consultar Pessoa ---" << endl;
                string nome = getString("Nome para consultar: ");
                avl.consultar(nome);
                break;
            }
            case 0:
                cout << "Saindo do programa..." << endl;
                break;
            default:
                cout << "Opcao invalida. Tente novamente." << endl;
                break;
        }
    }

    return 0;
}