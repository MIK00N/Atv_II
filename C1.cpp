#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// 1. Definição do Tipo
#define vertex int

// Classe Graph para representação de grafos usando matrizes de adjacência e peso
class Graph {
private:
    int V;       // Número de vértices
    int A;       // Número de arcos
    int **adj;   // Matriz de adjacência (1 se há arco, 0 caso contrário)
    int **dist;  // Matriz de distâncias/pesos
    int *grau;   // Grau de saída de cada vértice

    // Função privada para alocar e inicializar as matrizes
    void initializeMatrices(int V_val);

public:
    // Construtor: Inicializa a partir de um arquivo (obrigatorio "grafo.txt")
    Graph(const std::string& filename);
    
    // Destrutor: Libera a memória alocada dinamicamente
    ~Graph(); 

    // Métodos solicitados
    void insertArc(vertex v, vertex w, int weight);
    void removeArc(vertex v, vertex w);
    void listGraph(); 
    void displayAdjacencyMatrix(); 
    void displayDistanceMatrix(); 
    void displayVertexDegrees();

    // Getter para V (útil para o main)
    int getV() const { return V; }
};

// --- Implementação da Classe Graph ---

// Função auxiliar para alocar e inicializar as matrizes
void Graph::initializeMatrices(int V_val) {
    V = V_val;
    A = 0;
    
    // Alocação e inicialização
    adj = new int*[V];
    dist = new int*[V];
    grau = new int[V];

    for (int i = 0; i < V; ++i) {
        adj[i] = new int[V];
        dist[i] = new int[V];
        grau[i] = 0; // Inicializa o grau de saída

        for (int j = 0; j < V; ++j) {
            adj[i][j] = 0; 
            dist[i][j] = 0; 
        }
    }
}

// Construtor (Leitura do Arquivo)
Graph::Graph(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo " << filename << ". Certifique-se que o arquivo existe." << std::endl;
        V = 0; A = 0; adj = nullptr; dist = nullptr; grau = nullptr;
        return;
    }

    int V_file, A_file;
    // 1. Lê o número de vértices (V) e o número de arcos (A)
    if (!(file >> V_file >> A_file)) {
        std::cerr << "Erro: Arquivo com formato invalido (V e A nao encontrados)." << std::endl;
        V = 0; A = 0; adj = nullptr; dist = nullptr; grau = nullptr;
        return;
    }

    initializeMatrices(V_file); // Inicializa a estrutura do grafo

    vertex u, v;
    int weight;
    
    // 2. Loop para ler e inserir cada arco
    for (int i = 0; i < A_file; ++i) {
        if (file >> u >> v >> weight) {
            // Verifica se os vertices são validos e se o arco ainda nao existe
            if (u >= 0 && u < V && v >= 0 && v < V) {
                if (adj[u][v] == 0) {
                    adj[u][v] = 1;
                    dist[u][v] = weight;
                    grau[u]++; 
                    A++;      
                } else {
                    // Se o arco já existe no arquivo, apenas atualiza o peso.
                    dist[u][v] = weight; 
                }
            } else {
                 std::cerr << "Aviso: Vertices invalidos (" << u << ", " << v << ") encontrados no arquivo." << std::endl;
            }
        } else {
            std::cerr << "Aviso: Arquivo com menos arcos do que o esperado (" << A_file << ")." << std::endl;
            break; 
        }
    }

    std::cout << "--- Grafo carregado ---" << std::endl;
    std::cout << "Vertices: " << V << ", Arcos Iniciais: " << A << std::endl;
}

// Destrutor
Graph::~Graph() {
    if (adj != nullptr) {
        for (int i = 0; i < V; ++i) {
            delete[] adj[i];
            delete[] dist[i];
        }
        delete[] adj;
        delete[] dist;
        delete[] grau;
    }
}

// Inserção de Arco
void Graph::insertArc(vertex v, vertex w, int weight) {
    if (v < 0 || v >= V || w < 0 || w >= V) {
        std::cerr << "Erro: Vertice invalido para a insercao." << std::endl;
        return;
    }
    
    if (adj[v][w] == 0) {
        adj[v][w] = 1;
        grau[v]++;
        A++;
        std::cout << "Arco (" << v << ", " << w << ") INSERIDO com peso " << weight << "." << std::endl;
    } else {
        std::cout << "Arco (" << v << ", " << w << ") ja existia.";
    }
    
    // Atualiza o peso em ambos os casos (novo ou existente)
    dist[v][w] = weight; 
    if (adj[v][w] == 1) { // Só imprime a atualização se já existia
         std::cout << " Peso atualizado para " << weight << "." << std::endl;
    }
}

// Remoção de Arco
void Graph::removeArc(vertex v, vertex w) {
    if (v < 0 || v >= V || w < 0 || w >= V) {
        std::cerr << "Erro: Vertice invalido para a remocao." << std::endl;
        return;
    }
    
    if (adj[v][w] == 1) {
        adj[v][w] = 0;
        dist[v][w] = 0; 
        grau[v]--;
        A--;
        std::cout << "Arco (" << v << ", " << w << ") REMOVIDO." << std::endl;
    } else {
        std::cout << "Arco (" << v << ", " << w << ") NAO existe no grafo." << std::endl;
    }
}

// Listagem do Grafo (Lista os arcos)
void Graph::listGraph() {
    std::cout << "\n--- Listagem do Grafo (Arcos Atuais) ---" << std::endl;
    if (V == 0) {
        std::cout << "Grafo vazio." << std::endl;
        return;
    }

    std::cout << "Total de Vertices: " << V << ", Total de Arcos: " << A << std::endl;

    for (vertex u = 0; u < V; ++u) {
        for (vertex v = 0; v < V; ++v) {
            if (adj[u][v] == 1) {
                std::cout << "Arco: " << u << " -> " << v << " (Peso: " << dist[u][v] << ")" << std::endl;
            }
        }
    }
    std::cout << "----------------------------------------" << std::endl;
}

// Exibição da Matriz de Adjacência
void Graph::displayAdjacencyMatrix() {
    std::cout << "\n--- Matriz de Adjacencia (adj) ---" << std::endl;
    if (V == 0) return;

    // Cabecalho
    std::cout << "   ";
    for (int j = 0; j < V; ++j) {
        std::cout << j << " ";
    }
    std::cout << "\n  ";
    for (int j = 0; j < V * 2; ++j) std::cout << "-";
    std::cout << std::endl;
    
    // Linhas
    for (int i = 0; i < V; ++i) {
        std::cout << i << "| ";
        for (int j = 0; j < V; ++j) {
            std::cout << adj[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "----------------------------------" << std::endl;
}

// Exibição da Matriz de Distância (Pesos)
void Graph::displayDistanceMatrix() {
    std::cout << "\n--- Matriz de Distancia (dist) ---" << std::endl;
    if (V == 0) return;
    
    // Cabecalho
    std::cout << "   ";
    for (int j = 0; j < V; ++j) {
        std::cout << j << "   "; 
    }
    std::cout << "\n  ";
    for (int j = 0; j < V * 4; ++j) std::cout << "-";
    std::cout << std::endl;
    
    // Linhas
    for (int i = 0; i < V; ++i) {
        std::cout << i << "| ";
        for (int j = 0; j < V; ++j) {
            // Imprime o peso (distancia)
            // Usa setw ou formatacao manual para alinhamento
            std::cout.width(3);
            std::cout << dist[i][j] << " "; 
        }
        std::cout << std::endl;
    }
    std::cout << "------------------------------------" << std::endl;
}

// Exibição do Grau dos Vértices
void Graph::displayVertexDegrees() {
    std::cout << "\n--- Grau de Saida dos Vertices (grau) ---" << std::endl;
    if (V == 0) return;

    for (int i = 0; i < V; ++i) {
        std::cout << "Vertice " << i << ": Grau de Saida = " << grau[i] << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}

// --- Estrutura de Arquivo de Exemplo ---

/*
Para testar, crie um arquivo chamado "grafo.txt" no mesmo diretório do executável 
com o seguinte conteúdo:

4 5
0 1 10
0 2 5
1 3 2
2 1 1
2 3 9
*/

// --- Função main (Exemplo de Uso) ---

int main() {
    // Tenta carregar o grafo do arquivo
    Graph g("grafo.txt");

    if (g.getV() == 0) {
        std::cerr << "Nao foi possivel inicializar o grafo. Verifique o arquivo 'grafo.txt'." << std::endl;
        return 1; 
    }

    // 1. Exibição inicial
    std::cout << "\n#####################################################" << std::endl;
    std::cout << "## FASE 1: APOS LEITURA DO ARQUIVO ##################" << std::endl;
    std::cout << "#####################################################" << std::endl;
    g.listGraph();
    g.displayAdjacencyMatrix();
    g.displayDistanceMatrix();
    g.displayVertexDegrees();

    // 2. Teste de Inserção de Arco
    std::cout << "\n#####################################################" << std::endl;
    std::cout << "## FASE 2: INSERCAO E ATUALIZACAO ###################" << std::endl;
    std::cout << "#####################################################" << std::endl;
    g.insertArc(3, 0, 8);  // Novo arco (3 -> 0)
    g.insertArc(0, 1, 15); // Atualiza o peso de (0 -> 1)
    g.insertArc(2, 0, 4);  // Novo arco (2 -> 0)
    
    g.displayAdjacencyMatrix();
    g.displayDistanceMatrix();
    g.displayVertexDegrees();

    // 3. Teste de Remoção de Arco
    std::cout << "\n#####################################################" << std::endl;
    std::cout << "## FASE 3: REMOCAO ##################################" << std::endl;
    std::cout << "#####################################################" << std::endl;
    g.removeArc(2, 1); // Remover arco (2 -> 1)
    g.removeArc(1, 0); // Arco que não existe
    
    g.listGraph();
    g.displayAdjacencyMatrix();
    g.displayVertexDegrees();

    system("pause");
    return 0;
}