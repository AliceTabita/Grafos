#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

typedef struct Vetor
{
    int length = 0;
    int max_len = 0;
    // comma separated integer
    string temp_str = "";

    int *vetor;

    void instanciar(int tamanho)
    {
        vetor = new int[tamanho];
        this->max_len = tamanho;
    }
    void append(int item)
    {
        if (this->length == this->max_len)
        {
            cout << "Vetor cheio" << endl;
            return;
        }
        this->vetor[this->length] = item;
        this->length++;
    }
    // Converte uma string de números sem separação em vetor [cada digito em uma célula do vetor]
    // is digit serve para verificar se é um numero

    void converter_string_vetor()
    {
        vetor = new int[this->temp_str.size()];
        int indice = 0;
        for (int i = 0; i < this->temp_str.size(); i++)
        {
            if (isdigit(this->temp_str[i]))
            {
                string temp;
                temp.push_back(this->temp_str[i]);
                this->vetor[indice] = stoi(temp);
                indice++;
            }
        }
        this->length = indice;
    }
    void imprime()
    {
        if (this->length == 0)
        {
            cout << "vetor vazio" << endl;
        }
        for (int i = 0; i < this->length; i++)
        {
            cout << this->vetor[i] << endl;
        }
    }
    bool existe(int number)
    {
        if (this->length == 0)
        {
            return false;
        }
        for (int i = 0; i < this->length; i++)
        {
            if (this->vetor[i] == number)
            {
                return true;
            }
        }
        return false;
    }

    bool not_in(int number)
    {
        return !this->existe(number);
    }
    bool in(int number)
    {
        return this->existe(number);
    }
} Vetor;

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

    void insert(int pos, Vetor vec)
    {
        for (int i = 0; i < vec.length; i++)
        {
            this->matriz[pos][i] = vec.vetor[i];
        }
    }
    void instanciar(int linhas, int colunas)
    {

        this->matriz = new int *[linhas];
        for (int i = 0; i < linhas; i++)
        {
            this->matriz[i] = new int[colunas];
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
    Matriz matriz_ligacao_ponderada;
    Matriz matriz_adj;
    Matriz matriz_adj_ponderada;
    Matriz matriz_arvore_minima;
    Matriz matriz_inc;
    Matriz matriz_caminhos;

    // caracteristicas
    Matriz grau_vertice;
    bool is_pseudog = false;
    bool is_multig = false;
    bool is_nulo = false;
    bool is_vazio = false;
    bool is_trivial = false;
    bool is_conexo = true;
    bool is_completo = true;
    bool is_simples = false;
    bool is_regular = true;

    void ler_grafo_de_arquivo(string nome, string nm_arquivo)
    {
        this->nm_grafo = nome;
        this->nm_localizacao_arquivo = nm_arquivo;
        this->matriz_ligacao.nm_matriz = "ligacao";
        this->matriz_ligacao_ponderada.nm_matriz = "ligacao ponderada";
        this->matriz_adj.nm_matriz = "adjacencias";
        this->matriz_inc.nm_matriz = "incidencias";
        this->matriz_adj_ponderada.nm_matriz = "adjacencias ponderada";
        this->matriz_arvore_minima.nm_matriz = "Arvore Minima";

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
        this->matriz_ligacao_ponderada.instanciar(this->aresta, 3);
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
                this->matriz_ligacao_ponderada.matriz[linha][coluna] = stoi(temp_string);
                if (coluna < 2)
                {
                    this->matriz_ligacao.matriz[linha][coluna] = stoi(temp_string);
                }
            }
            else if (caracter != ';' && coluna < 3)
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
        this->gerar_matriz_adjacencias_ponderada();
        this->gerar_arvore_minima();
        this->in_tudo_certo = true;
    }

    void gerar_matriz_adjacencias()
    {
        // cout << "Gerando matriz de adjacencias" << endl;
        this->matriz_adj.instanciar(this->vertice, this->vertice);

        // insere os valores da matriz de ligação na matriz de adjacências
        for (int i = 0; i < this->matriz_ligacao.nr_linhas; i++)
        {
            // a matriz adjacente no indice q esta sendo lido na matriz de ligação -1 recebe 1
            this->matriz_adj.matriz[this->matriz_ligacao.matriz[i][0] - 1][this->matriz_ligacao.matriz[i][1] - 1] = 1;
            this->matriz_adj.matriz[this->matriz_ligacao.matriz[i][1] - 1][this->matriz_ligacao.matriz[i][0] - 1] = 1;
        }
        cout << "Matriz de adjacencias gerada" << endl;
    }
    void gerar_matriz_adjacencias_ponderada()
    {
        // cout << "Gerando matriz de adjacencias" << endl;
        this->matriz_adj_ponderada.instanciar(this->vertice, this->vertice);

        // insere os valores da matriz de ligação na matriz de adjacências
        for (int i = 0; i < this->matriz_ligacao_ponderada.nr_linhas; i++)
        {
            // matriz ponderada
            // no indice lido na matriz de ligação -1 eu insiro o peso da aresta daquela ligação
            this->matriz_adj_ponderada.matriz[this->matriz_ligacao.matriz[i][0] - 1][this->matriz_ligacao.matriz[i][1] - 1] = this->matriz_ligacao_ponderada.matriz[i][2];
            this->matriz_adj_ponderada.matriz[this->matriz_ligacao.matriz[i][1] - 1][this->matriz_ligacao.matriz[i][0] - 1] = this->matriz_ligacao_ponderada.matriz[i][2];
        }
        cout << "Matriz de adjacencias ponderada gerada" << endl;
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
    void gerar_arvore_minima()
    {
        this->copiar_matriz(&this->matriz_arvore_minima, this->matriz_ligacao_ponderada);
        for (int i = 0; i < this->matriz_arvore_minima.nr_linhas; i++)
        {

            for (int j = i + 1; j < this->matriz_arvore_minima.nr_linhas; j++)
            {
                if (this->matriz_arvore_minima.matriz[j][2] < this->matriz_arvore_minima.matriz[i][2])
                {
                    int auxiliar_1 = this->matriz_arvore_minima.matriz[i][0];
                    int auxiliar_2 = this->matriz_arvore_minima.matriz[i][1];
                    int auxiliar_3 = this->matriz_arvore_minima.matriz[i][2];
                    this->matriz_arvore_minima.matriz[i][0] = this->matriz_arvore_minima.matriz[j][0];
                    this->matriz_arvore_minima.matriz[i][1] = this->matriz_arvore_minima.matriz[j][1];
                    this->matriz_arvore_minima.matriz[i][2] = this->matriz_arvore_minima.matriz[j][2];

                    this->matriz_arvore_minima.matriz[j][0] = auxiliar_1;
                    this->matriz_arvore_minima.matriz[j][1] = auxiliar_2;
                    this->matriz_arvore_minima.matriz[j][2] = auxiliar_3;
                }
            }
        }
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
        this->matriz_adj_ponderada.imprimir_matriz();
        this->matriz_inc.imprimir_matriz();
        this->matriz_ligacao_ponderada.imprimir_matriz();
        this->matriz_arvore_minima.imprimir_matriz();
    }

    void gerar_matriz_caminhos(int numero_iniciar_caminho, Vetor &executados)
    {
        int mat = 0;
        mat = (this->vertice * (this->vertice - 1)) / 2;
        this->matriz_caminhos.instanciar(mat / 2, this->vertice);
        // matriz_caminhos.nm_matriz = to_string(numero_iniciar_caminho);

        for (int i = 1; i < this->aresta; i++)
        {
            this->caminhoRecursivo(numero_iniciar_caminho, i, executados);
            cout << "---" << i << endl;
            executados.append(2);
        }

        this->matriz_caminhos.imprimir_matriz();
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
    void verifica_simples()
    {

        if ((!this->is_multig) && (!this->is_pseudog))
        {
            this->is_simples = true;
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
        // percorre a matriz de adjacencias verificando se todas as celulas (tirando as da diagonal principal) são iguais a 1
        // se sim quer dizer que tenho todas as ligações possiveis e ele e completo
        int mat = 0;
        mat = (this->vertice * (this->vertice - 1)) / 2;
        cout << "mat " << mat << endl;
        // não sabia se existindo aresta paralela e laço poderia ser considerado completo
        if ((!is_pseudog && !is_multig) && (this->vertice >= mat))
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
    }
    void conexividade()
    {
        // crio um vetor com as possibilidades de vértices e percorro a matriz de ligação verificando se o vertice existe
        // se existir na matriz de ligação eu removo do vetor e passo pro próximo vertice
        // no final eu tenho no vetor apenas os vértices desconexos
        if (!this->is_vazio)
        {
            int aux = 0;
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
    }
    void grau()
    {
        if (!this->is_vazio)
        {
            int aux = 0, cont = 0;
            this->grau_vertice.instanciar(this->vertice, 2);
            this->grau_vertice.nm_matriz = "Graus";
            for (int a = 0; a < this->vertice; a++)
            {
                grau_vertice.matriz[a][0] = a + 1;
            }

            while (aux < this->vertice)
            {
                for (int i = 0; i < this->matriz_ligacao.nr_linhas; i++)
                {
                    for (int j = 0; j < 2; j++)
                    {
                        if (this->matriz_ligacao.matriz[i][j] == this->grau_vertice.matriz[aux][0])
                        {
                            cont++;
                        }
                    }
                }
                // não lembro se os laços contam no grau do grafo
                // essa verificação existe pra caso seja laço remover da conta dos graus

                this->grau_vertice.matriz[aux][1] = cont;

                cont = 0;
                aux++;
            }
            this->grau_vertice.imprimir_matriz();
        }
    }
    void regular()
    {
        // verifica se algum grau e diferente do 1, se for já não é regular
        int aux = this->grau_vertice.matriz[0][1];
        for (int i = 1; i < this->vertice; i++)
        {
            if (this->grau_vertice.matriz[i][1] != aux)
            {
                this->is_regular = false;
            }
        }
    }

    void caracteristicas_grafo()
    {
        this->caract_laco();
        this->caract_arParalela();
        this->verifica_simples();
        this->vazio_nulo_trivial();
        this->conexividade();
        this->completo();
        this->grau();
        this->regular();
        this->exibir_caracteristicas();
    }
    void exibir_caracteristicas()
    {
        cout << "completo? " << (this->is_completo ? "sim" : "não") << endl;
        cout << "pseudografo? " << (this->is_pseudog ? "sim" : "não") << endl;
        cout << "multigrafo? " << (this->is_multig ? "sim" : "não") << endl;
        cout << "simples? " << (this->is_simples ? "sim" : "não") << endl;
        cout << "vazio? " << (this->is_vazio ? "sim" : "não") << endl;
        cout << "nulo? " << (this->is_nulo ? "sim" : "não") << endl;
        cout << "trivial? " << (this->is_trivial ? "sim" : "não") << endl;
        cout << "conexo? " << (this->is_conexo ? "sim" : "não") << endl;
        cout << "regular? " << (this->is_regular ? "sim" : "não") << endl;
    }

    void copiar_matriz(Matriz *copia, Matriz base)
    {
        copia->instanciar(base.nr_linhas, base.nr_colunas);
        for (int i = 0; i < copia->nr_linhas; i++)
        {
            for (int j = 0; j < copia->nr_colunas; j++)
            {
                copia->matriz[i][j] = base.matriz[i][j];
            }
        }
    }

    void liberar_memoria()
    {

        this->matriz_ligacao.liberar_memoria();
        this->matriz_ligacao_ponderada.liberar_memoria();
        this->matriz_adj.liberar_memoria();
        this->matriz_adj_ponderada.liberar_memoria();
        this->matriz_arvore_minima.liberar_memoria();
        this->matriz_inc.liberar_memoria();
    }

    Vetor caminhoRecursivo(int inicio, int fim, Vetor &executados)
    {
        // recebe como parametro um vetor de vertices executados (não podem ser usados nesse caminho)
        // converte e imprime a string encontrada no encontrar caminho em vetor
        Vetor vetor;
        string temp_str;
        temp_str = this->__encontrar_caminho(inicio, fim, executados);
        vetor.temp_str = temp_str;
        vetor.converter_string_vetor();
        vetor.imprime();
        return vetor;
    }
    string __encontrar_caminho(int inicio, int fim, Vetor &executados)
    {
        executados.append(inicio);
        for (int i = 0; i < this->matriz_adj.nr_linhas; i++)
        {
            if (executados.not_in(i + 1))
            {
                if (this->matriz_adj.matriz[inicio - 1][i] == 1 && this->matriz_adj.matriz[fim - 1][i] == 1)
                {
                    string result;
                    result.append(to_string(inicio));
                    result.append(",");
                    result.append(to_string(i + 1));
                    result.append(",");
                    result.append(to_string(fim));
                    return result;
                }
            }
        }
        for (int i = 0; i < this->matriz_ligacao.nr_linhas; i++)
        {
            int busca = 0;
            if (inicio == this->matriz_arvore_minima.matriz[i][0] && executados.not_in(this->matriz_arvore_minima.matriz[i][1]))
            {
                busca = this->matriz_arvore_minima.matriz[i][1];
            }
            if (inicio == this->matriz_arvore_minima.matriz[i][1] && executados.not_in(this->matriz_arvore_minima.matriz[i][0]))
            {
                busca = this->matriz_arvore_minima.matriz[i][0];
            }

            if (busca != 0)
            {
                string result = "";
                result.append(",");
                result.append(to_string(inicio));
                result.append(this->__encontrar_caminho(busca, fim, executados));
                return result;
            }
        }
        return " ";
    }
} Grafo;

int main(int argc, char *argv[])
{
    Grafo grafo;
    Vetor executados;
    grafo.ler_grafo_de_arquivo("g1", "./grafo2.txt");
    executados.instanciar(grafo.vertice);
    executados.append(4);
    executados.append(5);
    // grafo.caminhoRecursivo(1, 8, executados);
    //  grafo.gerar_matriz_caminhos(1, executados);
    grafo.imprime_grafo();
    grafo.caracteristicas_grafo();
    //  grafo.liberar_memoria();
    return 0;
}