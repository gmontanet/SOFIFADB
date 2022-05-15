#ifndef METHODS_H
#define METHODS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <windows.h>

//Métodos a serem usados no trabalho

namespace methods
{
    //Dados dos jogadores
    struct playerData
    {
        std::string
        short_name,     //Nome abreviado do jogador
        long_name,      //Nome completo do jogador
        nationality,    //Nacionalidade do Jogador
        club_name,      //Clube de atuação
        positions;      //Posições de atuação do jogador

        uint16_t
        overall,        //Pontuação geral do jogador
        age,            //Idade do jogador
        height_cm,      //Altura do jogador
        weight_kg;      //Peso do jogador

        uint32_t
        total_votes,    //Quantidade de votos recebidas
        sofifa_id;      //ID fifa

        float
        rating;         //Média dos votos dos usuários
    };

	//Struct com avaliações dos usuários
    struct userData2
    {
        methods::playerData* sofifa_id;
        float rating;
    };

	//Hash table pra armazenar dados dos usuários
    class hashTableUser2
    {
		//Divisor para cálculo de hash
        int mood;
		
		/* Estrutura de armazenamento:
         * Vetor de 2 dimensões (hash table de encadeamento fechado)
         * com elemento par de dados uint32_t que representa um user_id
         * e um vetor do tipo userData2 que representa um avaliação de um 
         * usuário a um jogador.
         */
        std::vector<std::vector<std::pair<uint32_t, std::vector<userData2> > > > table; 

		//Cálculo do hash
        int hash(int user_id);

    public:

        //constructor
        hashTableUser2(int module);

        //destructor
        ~hashTableUser2(){}

        /* Inserção: Verifica se o user_id já está na hashtable,
         * se já estiver, adiciona a nota e o endereço do jogador 
         * à lista de jogadores votados pelo usuário. Caso contrário
         * adiciona um novo user_id à hashtable e vincula a nota e
         * o endereço do jogador (struct userData2) ao user_id cadastrado.
         * O tratamento de colisões é feito através de endereçamento fechado
         */
        void insert(uint32_t user_id, userData2 userStruct);

        //Pesquisa na hashtable o user_id informado
        std::vector<userData2> search(uint32_t user_id);

        //Método auxiliar utlizados para testes
        //Retorna a quantidade de elementos na trie
        uint32_t size();
    };

    //hashtable pora armazena dos ID dos jogadores
    class hashTablePlayer
    {
        //Armazena dados
        std::vector<std::vector<methods::playerData*> > table;
        
        //Divisor para cálculo de hash
        int mood;

        //Cálculo do hash
        int hash(uint32_t sofifa_id);

    public:

        //Constructor
        hashTablePlayer( int module );

        //Destructor
        ~hashTablePlayer(){}

        //Recebe endereço e transforma sofifa id em hash
        void insert(methods::playerData* address);

        //Pesquisa
        methods::playerData* search(uint32_t sofifa_id);

        //Retorna uma lista com todos jogadores na hash
        std::vector<methods::playerData*> getAll();
    };

    //Verifica se o tipo playerData informado está presente em um vetor
    bool stdVectorFind(methods::playerData* toFind, std::vector<methods::playerData*> &vec);

    //Árvore Trie para as Tags
    class trieTreeTags
    {
        //Estrutura de nodo
        struct trieNode
        {
            char caracter;
            bool terminator;

            //Vetor para armazenamento de ponteiros para player
            //(utilizado apenas em nodos terminadores de string)
            std::vector<methods::playerData*> player;

            //ponteiro para filhos
            std::vector<trieTreeTags::trieNode*> childs{256, nullptr};
        };

        //Raiz da árvore
        trieNode* root;

        //Pesquisa padrão de árvore trie
        //Se a palavra for encontrada retorna o endereço do nodo terminador
        //Se não, retorna null
        trieNode* internalSearch(std::string tag);
        
        //Insere nova tag na árvore trie
        void internalInsert(std::string tag, methods::playerData* player);
    
    public:

        //Construtor
        trieTreeTags();

        //Destructor
        ~trieTreeTags() {};

        //Função de inserção, recebe um endereço de um plyer e um vetor de strings
        //Insere na trie todas as palavras do vetor associados ao jogador informado
        void insert(methods::playerData* player, std::vector<std::string> vecTags);

        //Pesquisa por uma tag na trie
        std::vector<methods::playerData*> search(std::string tag);
    };

    //Trie Tree Positions: estritamente igual à Trie Tree Tags
    //Classe filha da Trie Tree Tags
    class trieTreePosition : public methods::trieTreeTags {};

    // Trie Tree para Nomes
    class trieTreeFullName
    {
        //Estrutura de nodo
        struct trieNode
        {
            char caracter;
            bool terminator;

            //Vetor para armazenamento de ponteiros para player
            //(utilizado apenas em nodos terminadores de string)
            std::vector<methods::playerData*> player;

            //ponteiro para filhos
            std::vector<trieTreeFullName::trieNode*> childs{256, nullptr};
        };

        //Raiz da arvore
        trieNode* root;

        //Pesquisa padrão de árvore trie
        //Se a palavra for encontrada retorna o endereço do nodo terminador
        //Se não, retorna null
        trieNode* internalSearch(std::string tag);
        
        //insere nova tag na árvore trie
        void internalInsert(std::string tag, methods::playerData* player);

        //Função específica dessa Trie Tree de nomes para pesquisa por prefixos
        //Recebe um nodo e retorna (por referencia) um vetor de jogadores que estão "abaixo" do nodo informado
        void getChild(trieTreeFullName::trieNode* pointer, std::vector<methods::playerData*> &out);
    
    public:

        //Construtor
        trieTreeFullName();

        //Destructor
        ~trieTreeFullName() {};

        //Pesquisa
        std::vector<methods::playerData*> search(std::string toSearch);
    
        //Inserção: Insere diversos nomes na trie associados à um mesmo jogador
        void insert(methods::playerData* player, std::vector<std::string> vecNames);
    };

    //Armazenará os dados necessários
    struct dataSet
    {
        // Guarda estrutura com dados dos jogadores
        // em uma hashtable com base no sofifa_id.
        // 8191 foi um dos primos escolhidos para essa hashtable
        // por ter sido um dos mais performáticos
        methods::hashTablePlayer htPlayers{8191};

        // Guarda estrutura com dados das votações do
        // usuário em uma hashtable com base no user_id.
        // 131071 foi um dos primos escolhidos para essa hashtable
        // por ter sido um dos mais performáticos
        methods::hashTableUser2 htUser{131071};

        // PESQUISA SOBRE NOME DO JOGADOR
        // Guarda estrutura com dados dos jogadores
        // em uma árvore Trie com base no nome dos jogadores.
        methods::trieTreeFullName ttNames;

        // PESQUISA SOBRE TAGS
        // Guarda estrutura com dados dos jogadores
        // em uma árvore Trie com base nas tags.
        methods::trieTreeTags ttTags;

        // PESQUISA DE MELHORES JOGADORES DE DETERMINADA POSIÇÃO
        // Guarda estrutura com dados dos jogadores
        // em uma árvore Trie com base nas posições.
        methods::trieTreePosition ttPosition;

        // VETOR AUXILIAR PARA EXIBIR TODOS OS JOGADORES
        // Vetor capturada na ordem de leitura do arquivo player_21.csv (ordenado por overhall por padrão)
        std::vector<methods::playerData*> vecOverhall;

        // VETOR AUXILIAR PARA EXIBIR TODOS OS JOGADORES
        // Vetor ordenado com base no rating
        std::vector<methods::playerData*> vecRating;

    };

    //Lê arquivo com dados dos jogadores e adiciona as informações no DataSet
    uint8_t LoadAndProcessPlayers(std::string path, methods::dataSet &);
    uint8_t LoadAndProcessRating(std::string path, methods::dataSet &);
    uint8_t LoadAndProcessTags(std::string path, methods::dataSet &);

    //Metodos auxiliares:

    //Print da hora atual
    std::string currentTime(std::string format = "[%H:%M:%S]");
    std::wstring wcurrentTime(std::string format = "[%H:%M:%S]");

    //Mapeia uma string em vetor (mapeamento generico com base em um separador)
    std::vector<std::string> stringMap(std::string str, char spacer = ' ');

    //Mapeamento especifico para rating.csv no intuito de otimizar performance
    std::vector<uint32_t> ratingMap(std::string str);

    //Mapeamento especifico para o arquivo tags.csv no intuito de otimizr performance
    std::pair<int, std::string> tagsMap(std::string str);

    //Remove caracteres enrte um delimitador de uma string
    std::string removeCharacterInDelimiter(std::string str, char toRemove = ',', char delim = '\"', size_t firstDelim = 0);

    //Print em console das informações do jogador
    void playerInfo(uint32_t sofifa_id, methods::dataSet &);

    //Retorna um digito especifico de um float
    uint16_t digitOfFloat(float in, uint8_t digit);

    //Radix voltado para ordernar por rating
    void radixRating(std::vector<methods::playerData*> &inVec, uint8_t digits = 7);

    //Retorna um digito especifico de um int
    uint16_t digitOfInt(uint32_t in, uint8_t digit);

    //Radix voltado para ordenar o rating do usuário
    void radixRating(std::vector<methods::userData2> &inVec, uint8_t digits = 2);

    //Radix voltado para ordernar por overall
    void radixOverall(std::vector<methods::playerData*> &inVec, uint8_t digits = 2);

    //Decompõe a estrutura userData2 em 2 vetores
    std::pair<std::vector<methods::playerData*>, std::vector<float> > parsingUserData(std::vector<userData2>&);

    //Verifica elementos comum em dois vetores
    std::vector<methods::playerData*> tagIntersection(std::vector<methods::playerData*> &, std::vector<methods::playerData*> &);

    //Decompõe um conjunto de palavras separadas por apostofre
    std::vector<std::string> parsingTags(std::string);

    //Conta quantidade de vezes que um caractere especifico ocorre em uma string
    size_t countChar(std::string strIn, char toCount);

    //Verifica se o mesmo jogador está presente na lista mais de uma vez
    //Note: o vetor deve estar previamente ordenado
    void removeSequenceDuplicated(std::vector<methods::playerData*> &vec);

    //Inverte o vetor (obsoluta) : Solução: Inserção no vetor invertido
    void invertVector(std::vector<methods::playerData*> &vec);
};

#endif // METHODS_H

