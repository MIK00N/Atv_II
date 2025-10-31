#include <iostream>
#include <fstream>      // Para leitura de arquivos (ifstream)
#include <vector>
#include <string>
#include <sstream>      // Para processar linhas (stringstream)
#include <iomanip>      // Para formatar a matriz (setw)
#include <limits>       // Para limpar o buffer de entrada (numeric_limits)

using namespace std;

/**
 * @brief Classe para representar um Grafo Direcionado (Digrafo)
 * usando uma Matriz de Adjacência.
 */
class Grafo {
private:
    int numVertices;
    vector<vector<int>> matrizAdjacencia;

    /**
     * @brief Verifica se um vértice 'v' está dentro dos limites válidos.
     */
    bool ehValido(int v) {
        return (v >= 0 && v < numVertices);
    }

public:
    /**
     * @brief Construtor padrão. Cria um grafo vazio.
     */
    Grafo() : numVertices(0) {}

    /**
     * @brief Carrega o grafo a partir de um arquivo .txt.
     * @param nomeArquivo O caminho para o arquivo.
     * @return true se o carregamento for bem-sucedido, false caso contrário.
     */
    bool carregarDeArquivo(const string& nomeArquivo) {
        ifstream arquivo(nomeArquivo);
        if (!arquivo.is_open()) {
            cerr << "Erro: Nao foi possivel abrir o arquivo " << nomeArquivo << endl;
            return false;
        }

        string linha;
        int V, E;

        // 1. Ler a primeira linha (V E)
        if (!getline(arquivo, linha)) {
            cerr << "Erro: Arquivo vazio." << endl;
            arquivo.close();
            return false;
        }

        stringstream ss(linha);
        if (!(ss >> V >> E)) {
            cerr << "Erro: Formato invalido da primeira linha (deve ser 'V E')." << endl;
            arquivo.close();
            return false;
        }

        // 2. Inicializar (ou resetar) a estrutura do grafo
        this->numVertices = V;
        // Cria uma matriz V x V preenchida com 0s
        matrizAdjacencia.assign(V, vector<int>(V, 0));

        // 3. Ler os Arcos (E linhas)
        int arcosLidos = 0;
        while (getline(arquivo, linha) && arcosLidos < E) {
            stringstream ss_arco(linha);
            int u, v;
            if (ss_arco >> u >> v) {
                // Usa a função de inserção (que já faz a validação)
                inserirArco(u, v);
                arcosLidos++;
            }
        }

        arquivo.close();
        cout << "Info: Grafo carregado com sucesso. " << V << " vertices e " << arcosLidos << " arcos." << endl;
        return true;
    }

    /**
     * @brief F1: Insere um arco direcionado (u -> v) no grafo.
     */
    void inserirArco(int u, int v) {
        if (ehValido(u) && ehValido(v)) {
            if (matrizAdjacencia[u][v] == 0) {
                matrizAdjacencia[u][v] = 1;
            }
            // Se já for 1, o arco já existe.
        } else {
            cerr << "Erro (Inserir): Vertice " << u << " ou " << v << " invalido." << endl;
        }
    }

    /**
     * @brief F2: Remove um arco direcionado (u -> v) do grafo.
     */
    void removerArco(int u, int v) {
        if (ehValido(u) && ehValido(v)) {
            if (matrizAdjacencia[u][v] == 1) {
                matrizAdjacencia[u][v] = 0;
            }
            // Se já for 0, o arco já não existe.
        } else {
            cerr << "Erro (Remover): Vertice " << u << " ou " << v << " invalido." << endl;
        }
    }

    /**
     * @brief F3: Lista o grafo no formato de Lista de Adjacência.
     */
    void listarGrafo() {
        if (numVertices == 0) {
            cout << "Grafo vazio." << endl;
            return;
        }
        cout << "\n--- Listagem do Grafo (Formato Lista de Adjacencia) ---" << endl;
        for (int i = 0; i < numVertices; ++i) {
            cout << i << " -> ";
            bool temVizinho = false;
            for (int j = 0; j < numVertices; ++j) {
                if (matrizAdjacencia[i][j] == 1) {
                    if (temVizinho) {
                        cout << ", ";
                    }
                    cout << j;
                    temVizinho = true;
                }
            }
            if (!temVizinho) {
                cout << "(nenhum)";
            }
            cout << endl;
        }
        cout << "--------------------------------------------------------" << endl;
    }

    /**
     * @brief F4: Exibe a Matriz de Adjacência.
     */
    void exibirMatrizAdjacencia() {
        if (numVertices == 0) {
            cout << "Grafo vazio." << endl;
            return;
        }
        cout << "\n--- Matriz de Adjacencia ---" << endl;
        
        // Cabeçalho (colunas)
        cout << "   |";
        for (int i = 0; i < numVertices; ++i) {
            cout << setw(3) << i;
        }
        cout << "\n---+";
        for (int i = 0; i < numVertices; ++i) {
            cout << "---";
        }
        cout << endl;

        // Linhas
        for (int i = 0; i < numVertices; ++i) {
            cout << setw(3) << i << "|";
            for (int j = 0; j < numVertices; ++j) {
                cout << setw(3) << matrizAdjacencia[i][j];
            }
            cout << endl;
        }
        cout << "------------------------------" << endl;
    }

    /**
     * @brief F5: Exibe os graus de entrada (In) e saída (Out) de cada vértice.
     */
    void exibirGrausVertices() {
        if (numVertices == 0) {
            cout << "Grafo vazio." << endl;
            return;
        }
        cout << "\n--- Graus dos Vertices ---" << endl;
        for (int i = 0; i < numVertices; ++i) {
            int grauSaida = 0;
            int grauEntrada = 0;

            for (int j = 0; j < numVertices; ++j) {
                // Grau de Saída: Arcos que SAEM de 'i' (linha 'i')
                grauSaida += matrizAdjacencia[i][j];
                
                // Grau de Entrada: Arcos que CHEGAM em 'i' (coluna 'i')
                grauEntrada += matrizAdjacencia[j][i];
            }
            
            cout << "Vertice " << setw(2) << i << ": "
                 << "Grau de Saida (Out): " << grauSaida
                 << " | Grau de Entrada (In): " << grauEntrada
                 << endl;
        }
        cout << "----------------------------" << endl;
    }
};


// --- Funções Auxiliares para o Menu ---

void exibirMenu() {
    cout << "\n========== Menu de Gerenciamento de Grafo ==========" << endl;
    cout << "1. Carregar Grafo do Arquivo (grafo.txt)" << endl;
    cout << "2. Inserir Arco (u, v)" << endl;
    cout << "3. Remover Arco (u, v)" << endl;
    cout << "4. Listar Grafo (Formato Lista de Adjacencia)" << endl;
    cout << "5. Exibir Matriz de Adjacencia" << endl;
    cout << "6. Exibir Graus dos Vertices" << endl;
    cout << "0. Sair" << endl;
    cout << "==================================================" << endl;
    cout << "Escolha uma opcao: ";
}

/**
 * @brief Lê um inteiro da entrada padrão com segurança.
 */
int lerInteiro(const string& prompt) {
    int val;
    cout << prompt;
    while (!(cin >> val)) {
        cout << "Entrada invalida. Digite um numero inteiro: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    // Limpa o buffer DEPOIS de uma leitura bem-sucedida
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    return val;
}


// --- Função Principal ---

int main() {
    Grafo meuGrafo;
    int opcao;
    bool grafoCarregado = false; // Controla se o grafo foi carregado

    do {
        exibirMenu();
        
        // Lê a opção do menu
        while (!(cin >> opcao)) {
            cout << "Opcao invalida. Digite um numero: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpa o buffer

        // Verifica se o grafo foi carregado antes de operações (exceto 1 e 0)
        if (!grafoCarregado && opcao != 1 && opcao != 0) {
            cout << "Erro: Por favor, carregue um grafo primeiro (Opcao 1)." << endl;
            continue;
        }

        switch (opcao) {
            case 1: { // Carregar Grafo
                string nomeArquivo = "grafo.txt";
                cout << "Tentando carregar '" << nomeArquivo << "'..." << endl;
                if (meuGrafo.carregarDeArquivo(nomeArquivo)) {
                    grafoCarregado = true;
                } else {
                    cout << "Falha ao carregar o grafo." << endl;
                }
                break;
            }
            case 2: { // Inserir Arco
                int u = lerInteiro("Digite o vertice de origem (u): ");
                int v = lerInteiro("Digite o vertice de destino (v): ");
                meuGrafo.inserirArco(u, v);
                cout << "Arco (" << u << ", " << v << ") inserido (se valido)." << endl;
                break;
            }
            case 3: { // Remover Arco
                int u = lerInteiro("Digite o vertice de origem (u): ");
                int v = lerInteiro("Digite o vertice de destino (v): ");
                meuGrafo.removerArco(u, v);
                cout << "Arco (" << u << ", " << v << ") removido (se valido)." << endl;
                break;
            }
            case 4: // Listar Grafo
                meuGrafo.listarGrafo();
                break;
            case 5: // Exibir Matriz
                meuGrafo.exibirMatrizAdjacencia();
                break;
            case 6: // Exibir Graus
                meuGrafo.exibirGrausVertices();
                break;
            case 0: // Sair
                cout << "Saindo do programa..." << endl;
                break;
            default:
                cout << "Opcao invalida. Tente novamente." << endl;
                break;
        }
    } while (opcao != 0);

    return 0;
}