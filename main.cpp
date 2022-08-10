#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

typedef struct
{
    int nr_linhas = 0;
    int nr_colunas = 0;
    string nm_matriz;
    bool is_iniciada = false;
    int **matriz;
    void liberar_memoria()
    {
        free(this->matriz);
    }
    /* void for_each(void(function)(int *, int, int, int **))
     {
         for (int indice = 0; indice < this->nr_linhas; indice++)
         {
             function(this->matriz[indice], this->nr_colunas, indice, this->matriz);
         }
     }*/
    void instanciar(int linhas, int colunas)
    {

        this->matriz = new int *[linhas];
        for (int i = 0; i < linhas; i++)
        {
            this->matriz[i] = new int[2];
        }
        for (int i = 0; i < linhas; i++)
        {
            for (int j = 0; j < colunas; j++)
            {
                this->matriz[i][j] = 0;
            }
        }

        this->nr_linhas = linhas;
        this->nr_colunas = colunas;
        this->is_iniciada = true;
    }
    void imprimir_matriz()
    {
        if (!this->is_iniciada)
        {
            cout << "Matriz " << this->nm_matriz << " ainda não instânciada" << endl;
        }
        else
        {
            cout << "Matriz de " << this->nm_matriz << ":" << endl;
            cout << right << setw(2) << right << setw(2) << "0" << right << setw(2) << "|";
            for (int i = 1; i <= this->nr_colunas; i++)
            {
                cout << right << setw(2) << i << right << setw(2);
            }
            cout << endl;
            for (int i = 0; i < this->nr_linhas; i++)
            {
                cout << right << setw(2) << i + 1 << right << setw(2) << "|";
                for (int j = 0; j < this->nr_colunas; j++)
                {
                    cout << right << setw(2) << this->matriz[i][j] << right << setw(2);
                }
                cout << endl;
            }
        }
    }
} Matriz;

typedef struct
{
    int vertice = 0;
    int aresta = 0;
    bool in_tudo_certo = true;
    string nm_grafo;
    string nm_localizacao_arquivo;
    Matriz matriz_ligacao;
    Matriz matriz_adj;
    Matriz matriz_inc;

    bool is_pseudog = false;
    bool is_multig = false;
    bool is_nulo = false;
    bool is_vazio = false;
    bool is_trivial = false;
    bool is_conexo = true;
    bool is_completo = true;

    void ler_grafo_de_arquivo(string nome, string nm_arquivo)
    {
        this->nm_grafo = nome;
        this->nm_localizacao_arquivo = nm_arquivo;
        this->matriz_ligacao.nm_matriz = "ligacao";
        this->matriz_adj.nm_matriz = "adjacencias";
        this->matriz_inc.nm_matriz = "incidencias";
        string vertice, aresta, linha_lida;

        cout << "Iniciando leitura do grafo do arquivo " << nm_arquivo << endl;
        ifstream arq(nm_arquivo, ios::in); // nm_arquivo é o caminho para o arquivo

        if (!arq)
        {
            cout << "Arquivo grafo.txt nao pode ser aberto" << endl;
            abort();
        }
        cout << "Lendo vertice e aresta do grafo" << endl;
        getline(arq, vertice);
        getline(arq, aresta);

        cout << "Vertice: " << vertice << " "
             << "Aresta: " << aresta << endl;

        this->aresta = stoi(aresta);
        this->vertice = stoi(vertice);

        this->matriz_ligacao.instanciar(this->aresta, 2);

        int linha = 0, coluna = 0;
        cout << "Lendo dados do grafo e gerando da matriz de ligação" << endl;
        // busca os vertices no arquivo e coloca em uma matriz numero-arestas x 2
        getline(arq, linha_lida);
        for (char &caracter : linha_lida)
        {
            if (linha > this->matriz_ligacao.nr_linhas)
            {
                cout << "Houve uma falha ao tentar traduzir o grafo." << endl;
                cout << "Erro: O grafo possui mais ligações do que o total declarado nas arestas" << endl;
                this->in_tudo_certo = false;
                return;
            }
            if (caracter != ',' && caracter != ';')
            {
                // Cria string temporaria para poder converter o valor do char lido para inteiro.
                string temp_string;
                temp_string.push_back(caracter);
                this->matriz_ligacao.matriz[linha][coluna] = stoi(temp_string);
            }
            else if (caracter != ';' && coluna < 2)
            {
                coluna++;
            }
            else
            {
                coluna = 0;
                linha++;
            }
        }

        this->gerar_matriz_adjacencias();
        this->gerar_matriz_incidencias();
        this->in_tudo_certo = true;
    }

    void gerar_matriz_adjacencias()
    {
        // cout << "Gerando matriz de adjacencias" << endl;
        this->matriz_adj.instanciar(this->vertice, this->vertice);
        // insere os valores da matriz de ligação na matriz de adjacências
        for (int i = 0; i < this->matriz_ligacao.nr_linhas; i++)
        {
            this->matriz_adj.matriz[this->matriz_ligacao.matriz[i][0] - 1][this->matriz_ligacao.matriz[i][1] - 1] = 1;
            this->matriz_adj.matriz[this->matriz_ligacao.matriz[i][1] - 1][this->matriz_ligacao.matriz[i][0] - 1] = 1;
        }
        cout << "Matriz de adjacencias gerada" << endl;
    }
    void gerar_matriz_incidencias()
    {
        // cout << "Gerando matriz de incidencias" << endl;
        this->matriz_inc.instanciar(this->aresta, this->vertice);
        // insere os valores da matriz de ligação na matriz de incidências
        for (int i = 0; i < this->matriz_ligacao.nr_linhas; i++)
        {
            this->matriz_inc.matriz[i][this->matriz_ligacao.matriz[i][0] - 1] = 1;
            this->matriz_inc.matriz[i][this->matriz_ligacao.matriz[i][1] - 1] = 1;
        }
        cout << "Matriz de incidencias gerada" << endl;
    }

    void imprime_grafo()
    {
        if (!this->in_tudo_certo)
        {
            cout << "Grafo com problemas." << endl;
        }
        cout << "Imprimindo Grafo: " << this->nm_grafo << endl;
        cout << "Vertices: " << this->vertice << endl;
        cout << "Arestas: " << this->aresta << endl;

        this->matriz_ligacao.imprimir_matriz();
        this->matriz_adj.imprimir_matriz();
        this->matriz_inc.imprimir_matriz();
    }
    void caract_laco()
    {
        int cont = 0;
        for (int i = 0; i < this->vertice; i++)
        {
            if (this->matriz_adj.matriz[i][i] != 0)
            {
                cont++;
            }
        }
        cout << "lacos encontrados: " << cont << endl;

        if (cont != 0)
        {

            if (cont == this->vertice)
            {
                cout << "E um grafo reflexivo " << endl;
            }
            is_pseudog = true;
        }
    }

    void caract_arParalela()
    {
        int a = 0, b = 0, cont = 0;

        for (int j = 0; j < this->matriz_ligacao.nr_linhas; j++)
        {
            a = this->matriz_ligacao.matriz[j][0];
            b = this->matriz_ligacao.matriz[j][1];
            for (int i = j + 1; i < this->matriz_ligacao.nr_linhas; i++)
            {
                if ((this->matriz_ligacao.matriz[i][0] == a && this->matriz_ligacao.matriz[i][1] == b) || (this->matriz_ligacao.matriz[i][0] == b && this->matriz_ligacao.matriz[i][1] == a))
                {
                    cont++;
                    cout << "arestas paralelas: " << a << " - " << b << endl;
                    this->is_multig = true;
                }
            }
        }
        cout << "numero de arestas paralelas: " << cont << endl;
    }
    void is_simples()
    {

        if ((!this->is_multig) || (!this->is_pseudog))
        {
            cout << "E um grafo simples" << endl;
        }
    }
    void vazio_nulo_trivial()
    {
        if (this->vertice == 0)
        {
            this->is_nulo = true;
        }
        else if (this->vertice == 1)
        {
            this->is_trivial = true;
        }
        if (this->aresta == 0)
        {
            this->is_vazio = true;
        }
    }
    void completo()
    {
        int mat = 0;
        mat = (this->vertice * (this->vertice - 1)) / 2;
        cout << "mat " << mat << endl;
        if ((!is_pseudog || !is_multig) && (this->vertice >= mat))
        {
        }
        for (int i = 0; i < this->matriz_adj.nr_linhas; i++)
        {
            for (int j = 0; j < this->matriz_adj.nr_colunas; j++)
            {
                if ((i != j) && (this->matriz_adj.matriz[i][j] != 1))
                {
                    this->is_completo = false;
                }
            }
        }

        if (is_pseudog || is_multig)
        {
        }
    }
    void conexividade()
    {
        int aux = 0;
        bool encontrei = false;
        int *vet = new int[this->vertice];
        for (int a = 0; a < this->vertice; a++)
        {
            vet[a] = a + 1;
        }
        while (aux < this->vertice)
        {
            for (int i = 0; i < this->matriz_ligacao.nr_linhas; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    if (this->matriz_ligacao.matriz[i][j] == vet[aux])
                    {
                        vet[aux] = 0;
                        aux++;
                        j = 0;
                        i = 0;
                    }
                }
            }
            aux++;
        }
        for (int i = 0; i < this->vertice; i++)
        {
            if (vet[i] != 0)
            {
                this->is_conexo = false;
                cout << "vertice desconexo: " << vet[i] << endl;
            }
        }
    }
    void caracteristicas_grafo()
    {
        this->caract_laco();
        this->caract_arParalela();
        this->is_simples();
        this->vazio_nulo_trivial();
        this->conexividade();
        this->completo();
    }
    void liberar_memoria()
    {
        this->matriz_adj.liberar_memoria();
        this->matriz_ligacao.liberar_memoria();
    }
} Grafo;

/*void iterador(int *itens, int length, int indice, int **matriz)
{
    for (int i = 0; i < length; i++)
    {
        cout << itens[i];
    }
    cout << endl;
}*/
int main(int argc, char *argv[])
{
    Grafo grafo;
    grafo.ler_grafo_de_arquivo("g1", "./grafo.txt");
    grafo.imprime_grafo();
    // grafo.matriz_ligacao.for_each(iterador);
    grafo.caracteristicas_grafo();
    return 0;
}