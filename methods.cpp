#include "methods.h"

#include <chrono>
#include <ctime>
#include <thread>
#include <string>

#include <vector>

#include <QCoreApplication>


//Hash table user

methods::hashTableUser2::hashTableUser2(int module)
{
    //Inicialização do divisor
    hashTableUser2::mood = module;

    //Inicialização da tabela com os N elementos
    hashTableUser2::table = std::vector<std::vector<std::pair<uint32_t, std::vector<userData2> > > >(module);
}

int methods::hashTableUser2::hash(int user_id)
{
    return user_id % mood;
}

void methods::hashTableUser2::insert(uint32_t user_id, userData2 userStruct)
{
    auto index = hashTableUser2::hash(user_id);

    int userIndex = -1;

    for(size_t i = 0; i < hashTableUser2::table.at(index).size(); i++)
    {
        if(hashTableUser2::table.at(index).at(i).first == user_id)
        {
            userIndex = i;
            break;
        }
    }

    if(userIndex == -1)
    {
        hashTableUser2::table.at(index).push_back(std::make_pair(user_id, std::vector<userData2>(1, userStruct)));

    } else {

        hashTableUser2::table.at(index).at(userIndex).second.push_back(userStruct);
    }
}

std::vector<methods::userData2> methods::hashTableUser2::search(uint32_t user_id)
{
    std::vector<userData2> out;

    auto index = hashTableUser2::hash(user_id);

    for(size_t i = 0; i < hashTableUser2::table.at(index).size(); i++)
    {
        if(hashTableUser2::table.at(index).at(i).first == user_id)
        {
            out = hashTableUser2::table.at(index).at(i).second;
            break;
        }
    }

    return out;
}

uint32_t methods::hashTableUser2::size()
{
    uint32_t size = 0;

    for(auto user : hashTableUser2::table)
    {
        size += user.size();
    }

    return size;
}

//Hash table player

methods::hashTablePlayer::hashTablePlayer( int module )
{
    //Inicializa divisor
    this->mood = module;

    //Inicializa vetor com tamanho do divisor informado
    hashTablePlayer::table = std::vector<std::vector<methods::playerData*> >(module);
}

int methods::hashTablePlayer::hash(uint32_t sofifa_id)
{
    return sofifa_id%mood;
}

void methods::hashTablePlayer::insert(methods::playerData* address) 
{
    //Calculo index
    auto index = hash(address->sofifa_id);

    //Inserção
    hashTablePlayer::table.at(index).push_back(address);
}

methods::playerData* methods::hashTablePlayer::search(uint32_t sofifa_id)
{
    methods::playerData* out = nullptr;
    auto index = hash(sofifa_id);

    //Verifica se há mais de um jogador no mesmo endeço
    if(hashTablePlayer::table.at(index).size() > 1)
    {
        //Percorre vetor procurando o jogador correto
        for(auto player : hashTablePlayer::table.at(index))
        {
            if(player->sofifa_id == sofifa_id)
            {
                out = player;
                break;
            }
        }

    } else if (hashTablePlayer::table.at(index).size() == 1) {
        
        auto player = hashTablePlayer::table.at(index).at(0);

        if(player->sofifa_id == sofifa_id) out = player;
        
    }

    return out;
}

std::vector<methods::playerData*> methods::hashTablePlayer::getAll()
{
    std::vector<methods::playerData*> out;

    for(auto tableplayer : this->table)
    {
        for(auto player : tableplayer)
        {
            out.push_back(player);
        }
    }

    return out;
}

//Arvore trie Tags

methods::trieTreeTags::trieTreeTags()
{
    //Inicializção da raiz
    trieTreeTags::root = new trieTreeTags::trieNode;
    trieTreeTags::root->caracter = 0;
    trieTreeTags::root->terminator = false;
}

methods::trieTreeTags::trieNode* methods::trieTreeTags::internalSearch(std::string tag)
{
    auto pointer = this->root;

    for(auto character : tag)
    {
        pointer = pointer->childs.at((size_t)(unsigned char)character);

        if(pointer == nullptr) break;
    }

    if(pointer)
        return pointer->terminator ? pointer : nullptr;
    else
        return nullptr;
}

void methods::trieTreeTags::internalInsert(std::string tag, methods::playerData* player) 
{
    auto pointer = this->root;
    auto pastPointer = pointer; //backup do ponteiro inicial

    for(auto character : tag)
    {
        //Verifica existencia do nodo filho
        pointer = pointer->childs.at((size_t)(unsigned char)character);

        //Adicionar novo nodo
        if(pointer == nullptr)
        {
            auto node = new trieTreeTags::trieNode;
            node->caracter = character;
            node->terminator = false;
            pastPointer->childs.at((size_t)(unsigned char)character) = node;
            pointer = node;
        }

        pastPointer = pointer;               
    }

    //Define trminador
    pointer->terminator = true;
    pointer->player.push_back(player);

    return;
}

void methods::trieTreeTags::insert(methods::playerData* player, std::vector<std::string> vecTags)
{
    for(auto tag : vecTags)
    {
        //Pesquisa palavra na arvore
        auto node = trieTreeTags::internalSearch(tag);

        //Se node for nulo, deve-se inserir nova palavra na arvore
        if(node == nullptr)
        {
            //std::cout << "inserir novo." << std::endl;
            trieTreeTags::internalInsert(tag, player);

        } else { //Se não, a palavra ja existe

            if(!(methods::stdVectorFind(player, node->player))) //se não existe, adiciona
                node->player.push_back(player);  
        }
    }
}

std::vector<methods::playerData*> methods::trieTreeTags::search(std::string tag)
{
    //Pesquisa palavra na aravore
    auto node = trieTreeTags::internalSearch(tag);

    //Se a variavel node estiver definida, retorna o vetor de player do nodo
    //Se não retorna um vetor vazio
    return node ? node->player : std::vector<methods::playerData*>(0);
}

//Arvore trie Nomes

methods::trieTreeFullName::trieTreeFullName()
{
    //Incializa raiz
    trieTreeFullName::root = new trieTreeFullName::trieNode;
    trieTreeFullName::root->caracter = 0;
    trieTreeFullName::root->terminator = false;
}

methods::trieTreeFullName::trieNode* methods::trieTreeFullName::internalSearch(std::string tag)
{
    auto pointer = this->root;

    for(auto character : tag)
    {
        pointer = pointer->childs.at((size_t)(unsigned char)character);

        if(pointer == nullptr) break;
    }

    if(pointer)
        return pointer->terminator ? pointer : nullptr;
    else
        return nullptr;
}

void methods::trieTreeFullName::internalInsert(std::string tag, methods::playerData* player) 
{
    auto pointer = this->root;
    auto pastPointer = pointer; //backup do ponteiro inicial

    for(auto character : tag)
    {
        //Verifica existencia do nodo filho
        pointer = pointer->childs.at((size_t)(unsigned char)character);

        //Adicionar novo nodo
        if(pointer == nullptr)
        {
            auto node = new trieTreeFullName::trieNode;
            node->caracter = character;
            node->terminator = false;
            pastPointer->childs.at((size_t)(unsigned char)character) = node;
            pointer = node;
        }

        pastPointer = pointer;               
    }

    //Define trminador
    pointer->terminator = true;
    pointer->player.push_back(player);

    return;
}

void methods::trieTreeFullName::getChild(methods::trieTreeFullName::trieNode* pointer, std::vector<methods::playerData*> &out)
{
    if(pointer->terminator)
        out.insert(out.end(), pointer->player.begin(), pointer->player.end());

    for(auto child : pointer->childs)
        if(child) trieTreeFullName::getChild(child, out);
}

std::vector<methods::playerData*> methods::trieTreeFullName::search(std::string toSearch)
{
    std::vector<methods::playerData*> out;

    auto pointer = this->root;
    auto pastPointer = pointer;

    for(auto character : toSearch)
    {
        pointer = pointer->childs.at((size_t)(unsigned char)character);

        if(pointer == nullptr) break;

        pastPointer = pointer;
    }

    if(pointer)
        trieTreeFullName::getChild(pointer, out);
        
    return out;
}

void methods::trieTreeFullName::insert(methods::playerData* player, std::vector<std::string> vecNames)
{
    for(auto names : vecNames)
    {
        //Pesquisa palavra na arvore
        auto node = trieTreeFullName::internalSearch(names);

        //Se node for nulo, deve-se inserir nova palavra na arvore
        if(node == nullptr)
        {
            //std::cout << "inserir novo." << std::endl;
            trieTreeFullName::internalInsert(names, player);

        } else { //Se não, a palavra ja existe

            if(!(methods::stdVectorFind(player, node->player))) //se não existe, adiciona
                node->player.push_back(player);
        }
    }
}

//Funções de carregamento dos arquivos

uint8_t methods::LoadAndProcessPlayers(std::string path, methods::dataSet &data) //Carrega e processa os dados do arquivo -> memória
{
    //Abre arquivo com dados dos jogadores
    std::fstream inputFile;
    inputFile.open(std::string(std::string(path + "/players_21.csv")), std::ios::in);

    //Verifica se houve algum erro ao tentar abrir o arquivo
    if(inputFile.fail())  return 5;

    //Buffer para captura arquivo
    std::string buffer;

    //le primeira linha e verifica se é o esperado
    std::getline(inputFile, buffer);

    if(buffer != "sofifa_id,player_url,short_name,long_name,age,dob,height_cm,weight_kg,nationality,club_name,league_name,league_rank,overall,potential,value_eur,wage_eur,player_positions,preferred_foot,international_reputation,weak_foot,skill_moves,work_rate,body_type,real_face,release_clause_eur,player_tags,team_position,team_jersey_number,loaned_from,joined,contract_valid_until,nation_position,nation_jersey_number,pace,shooting,passing,dribbling,defending,physic,gk_diving,gk_handling,gk_kicking,gk_reflexes,gk_speed,gk_positioning,player_traits,attacking_crossing,attacking_finishing,attacking_heading_accuracy,attacking_short_passing,attacking_volleys,skill_dribbling,skill_curve,skill_fk_accuracy,skill_long_passing,skill_ball_control,movement_acceleration,movement_sprint_speed,movement_agility,movement_reactions,movement_balance,power_shot_power,power_jumping,power_stamina,power_strength,power_long_shots,mentality_aggression,mentality_interceptions,mentality_positioning,mentality_vision,mentality_penalties,mentality_composure,defending_marking,defending_standing_tackle,defending_sliding_tackle,goalkeeping_diving,goalkeeping_handling,goalkeeping_kicking,goalkeeping_positioning,goalkeeping_reflexes,ls,st,rs,lw,lf,cf,rf,rw,lam,cam,ram,lm,lcm,cm,rcm,rm,lwb,ldm,cdm,rdm,rwb,lb,lcb,cb,rcb,rb")
        return 6;

    //LOG
    std::wcout << methods::wcurrentTime() << " Iniciando leitura do arquivo \"players_21.csv\"." << std::endl;

    //Laço para leitura do arquivo
    while(std::getline(inputFile, buffer)) //coloca conteúdo da linha na variável buffer | para quando inputFile for EOF
    {
        //Remove virgulas que separam as posições
        buffer = methods::removeCharacterInDelimiter(buffer, ',', '\"'); //quando encontra aspas duplas, remove "," (para resolver bug das posições)

        //Mapeamento da string em vetor
        auto bufferMap = methods::stringMap(buffer, ',');

        //Novo endereço com dados de um jogador
        methods::playerData *newPlayer = new methods::playerData;

        //Atribuição dos dados
        newPlayer->sofifa_id = std::stoi(bufferMap[0]);
        newPlayer->short_name = bufferMap[2];
        newPlayer->long_name = bufferMap[3];
        newPlayer->age = std::stoi(bufferMap[4]);
        newPlayer->height_cm = std::stoi(bufferMap[6]);
        newPlayer->weight_kg = std::stoi(bufferMap[7]);
        newPlayer->nationality = bufferMap[8];
        newPlayer->club_name = bufferMap[9];
        newPlayer->overall = std::stoi(bufferMap[12]);
        newPlayer->positions = bufferMap[16];
        newPlayer->rating = 0;
        newPlayer->total_votes = 0;

        //Insere na hash table com base no ID
        data.htPlayers.insert(newPlayer);

        // Vetor com base na ordem de captura do arquivo que
        // já vem ordado por overall. Inserção ao inicio
        data.vecOverhall.insert(data.vecOverhall.begin(), newPlayer);

        //Insere na arvore Trie os jogadores com base em seus nomes
        //Note: Adiciona na arvore cada palavra do nome e o nome completo
        //para que a pesquisa possa ser realizada com espaçamento.

        //Nome completo
        auto splitPositions = methods::stringMap(newPlayer->long_name, ' ');
        splitPositions.push_back(newPlayer->long_name);
        data.ttNames.insert(newPlayer, splitPositions);

        //Short Name
        splitPositions = methods::stringMap(newPlayer->short_name, ' ');
        splitPositions.push_back(newPlayer->short_name);
        data.ttNames.insert(newPlayer, splitPositions);

        //Insere na tabela hash com base nas posições
        splitPositions = methods::stringMap(newPlayer->positions, ' ');
        data.ttPosition.insert(newPlayer, splitPositions);

    }

    //Fecha arquivo
    inputFile.close();

    //LOG
    std::wcout << methods::wcurrentTime() << " Fim da leitura do arquivo \"players_21.csv\"." << std::endl;

    return 0;
}

uint8_t methods::LoadAndProcessRating(std::string path, methods::dataSet &data)
{
    //Abertura do arquivo rating.csv
    std::fstream inputFile;
    inputFile.open(std::string(std::string(path + "/rating.csv")), std::ios::in);

    //Verificação de falha na aberturaa
    if(inputFile.fail()) return 7;

    //Buffer para receber dados
    std::string buffer;

    //le primeira linha e verifica se é o esperado
    std::getline(inputFile, buffer);

    //Verifica se primeira linha é o esperado
    if(buffer != "user_id,sofifa_id,rating") return 8;

    //LOG
    std::cout << methods::currentTime() << " Iniciando a leitura do arquivo \"rating.csv\"." << std::endl;

    //Primeira leitura
    std::getline(inputFile, buffer);
    auto bufferMap = methods::ratingMap(buffer);

    //Variaveis de controle
    uint32_t sofifa_idPresent = bufferMap[1];
    uint32_t sofifa_idPast = sofifa_idPresent;
    uint32_t ratingCount = 1;
    double rating = (double)bufferMap[2]/10;

    //Adiciona a hash os dados do primeiro usuário
    methods::userData2 user;
    user.rating = rating;
    user.sofifa_id = data.htPlayers.search(sofifa_idPresent);
    data.htUser.insert(bufferMap[0], user);

    //Laço para demais leituras
    while(std::getline(inputFile, buffer))
    {
        //Mapeamento da string em vetor
        bufferMap = methods::ratingMap(buffer);

        //Atualiza id do jogador da linha atual
        sofifa_idPresent = bufferMap[1];

        //Salva dados do usuário
        user.rating = (double)bufferMap[2]/10;
        user.sofifa_id = data.htPlayers.search(sofifa_idPresent);
        data.htUser.insert(bufferMap[0], user);

        //
        if(sofifa_idPast != sofifa_idPresent)
        {
            //Adicionar
            auto player = data.htPlayers.search(sofifa_idPast);
            player->rating = rating/ratingCount;
            player->total_votes = ratingCount;
            data.htUser.insert(bufferMap[0], user);

            rating = (double)bufferMap[2]/10;
            ratingCount = 1;

            // Responder eventos QT
            // remove "Não está respondendo" da janelo de loading
            QCoreApplication::processEvents();

        } else {

            rating += ((double)bufferMap[2]/10);  //Soma avaliação
            ratingCount++;                       //Incremento contador de avaliações
        }

        sofifa_idPast = sofifa_idPresent;


    }

    //Salva dados do ultimo jogador
    auto player = data.htPlayers.search(sofifa_idPast);
    player->rating = rating/ratingCount;
    player->total_votes = ratingCount;

    //Fecha arquivo
    inputFile.close();

    //LOG
    std::cout << methods::currentTime() << " Fim da leitura do arquivo \"rating.csv\"." << std::endl;

    return 0;
}

uint8_t methods::LoadAndProcessTags(std::string path, methods::dataSet &data)
{
    //Abertura do arquivo tags.csv
    std::fstream inputFile;
    inputFile.open(std::string(std::string(path + "/tags.csv")), std::ios::in);

    //Verificação de falha na abertura
    if(inputFile.fail()) return 7;

    //Buffer para receber dados
    std::string buffer;

    //le primeira linha e verifica se é o esperado
    std::getline(inputFile, buffer);

    //Verifica se primeira linha é o esperado
    if(buffer != "user_id,sofifa_id,tag") return 8;

    //LOG
    std::cout << methods::currentTime() << " Iniciando a leitura do arquivo \"tags.csv\"." << std::endl;

    //Primeira leitura
    std::getline(inputFile, buffer);
    auto bufferMap = methods::tagsMap(buffer);

    //Variaveis de controle
    uint32_t sofifa_idPresent = bufferMap.first;
    uint32_t sofifa_idPast = sofifa_idPresent;
    std::vector<std::string> tagsVec;
    tagsVec.push_back(bufferMap.second);

    //Laço para demais leituras
    while(std::getline(inputFile, buffer))
    {
        //Mapeamento da string em vetor
        bufferMap = methods::tagsMap(buffer);

        //Atualiza id do jogador da linha atual
        sofifa_idPresent = bufferMap.first;

        //Verifica se a leitura atual é a mesma que anterior
        if(sofifa_idPast != sofifa_idPresent)
        {
            //Verifica se há tags associadas
            if(tagsVec.size())
            {
                //Pega o endereço com a estrutura dos dados do jogador
                auto player = data.htPlayers.search(sofifa_idPast);

                //Se o ID for encontrado
                if(player)
                {
                    //Insere na trie de tags a lista de tags associadas a um jogador
                    data.ttTags.insert(player, tagsVec);
                    tagsVec.clear();
                }
            }
        }

        //Se houver tag, adiciona ao vetor
        if(bufferMap.second.length())
        {
            tagsVec.push_back(bufferMap.second);
        }

        sofifa_idPast = sofifa_idPresent;
    }

    //Ultimo caso
    data.ttTags.insert(data.htPlayers.search(sofifa_idPast), tagsVec);

    //LOG
    std::cout << methods::currentTime() << " Fim da leitura do arquivo \"tags.csv\"." << std::endl;

    return 0;

}

//Funções auxiliares

std::vector<std::string> methods::stringMap(std::string str, char spacer)
{
    std::vector<std::string> out;

    size_t start = 0, end = 0;

    //Percorre toda string
    while(end != std::string::npos)
    {
        //Encontra posição da proxima virgula
        end = str.find(spacer, end+1);

        out.push_back(str.substr(start, end - start));

        start = end+1;
    }

    return out;
}

std::string methods::currentTime(std::string format)
{
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    char output[11];
    strftime(output, 11, format.c_str(), timeinfo);

    return std::string(output);
}

std::wstring methods::wcurrentTime(std::string format)
{
    int size = format.size()+1;
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    char output[size];
    strftime(output, size, format.c_str(), timeinfo);

    //Conversão de char[] para std::wstring
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &output[0], size, NULL, 0);
    std::wstring wstrTo( size_needed, 0 );
    MultiByteToWideChar(CP_UTF8, 0, &output[0], size, &wstrTo[0], size_needed);

    return wstrTo;
}

std::string methods::removeCharacterInDelimiter(std::string str, char toRemove, char delim, size_t firstDelim) //função para remover vírgulas dentro das aspas duplas
{
    //Encontra o primeiro delimitador
    firstDelim = str.find(delim, firstDelim);

    //Remove caractere a ser removido apos o primeiro limitador
    if(firstDelim != std::string::npos)
    {
        //Remove primeiro delimitador
        str.erase(firstDelim, 1);

        //Enquanto não encontra proximo limitador, remove caractere informado
        while(str.at(firstDelim) != delim)
        {
            if(str.at(firstDelim) == toRemove) str.erase(firstDelim, 1);
            firstDelim++;
        }

        //Remove ultimo delimitador
        str.erase(firstDelim, 1);
        
        //Continua o processo no resto da string - de forma recursiva
        str = methods::removeCharacterInDelimiter(str, toRemove, delim, firstDelim);
    }

    return str;
}

void methods::playerInfo(uint32_t sofifa_id, methods::dataSet &data)
{
    auto player = data.htPlayers.search(sofifa_id);

    std::cout << "ID Sofifa: " << player->sofifa_id << std::endl
              << "Vulgo: " << player->short_name << std::endl
              << "Nome: " << player->long_name << std::endl
              << "Idade: " << player->age << std::endl
              << "Altura(cm): " << player->height_cm << std::endl
              << "Peso(kg): " << player->weight_kg << std::endl
              << "Nacionalidade: " << player->nationality << std::endl
              << "Clube: " << player->club_name << std::endl
              << "Pontuacao geral: " << player->overall << std::endl
              << "Posicoes: " << player->positions << std::endl
              << "Avaliacao dos usuarios: " << player->rating << std::endl
              << "Total de avaliacoes: " << player->total_votes << std::endl;
}

std::vector<uint32_t> methods::ratingMap(std::string str)
{
    std::vector<uint32_t> out(3, 0);

    uint16_t index = 0;

    //user_id
    while(str.at(index) != ',')
    {
        out[0] *= 10;
        out[0] += str.at(index)-'0';
        index++;
    }

    index++;

    //sofifa_id
    while(str.at(index) != ',')
    {
        out[1] *= 10;
        out[1] += str.at(index)-'0';
        index++;
    }

    //rating
    out[2] = (str.at(str.size()-3)-'0')*10;
    out[2] += str.at(str.size()-1)-'0';

    return out;
}

std::pair<int, std::string> methods::tagsMap(std::string str)
{
    int sofifa_id = 0;
    std::string tag;

    //Ignora user_id
    auto index = str.find(',');

    while(str.at(++index) != ',')
    {
        sofifa_id *= 10;
        sofifa_id += str.at(index)-'0';
    }

    return std::make_pair(sofifa_id, str.substr(index+1));

}

bool methods::stdVectorFind(methods::playerData* toFind, std::vector<methods::playerData*> &vec) //função de pesquisa
{
    //Itera pelo votor atras do elmento a ser procurado
    for(auto i : vec) if(i == toFind) return true;

    return false;
}

uint16_t methods::digitOfFloat(float in, uint8_t digit)
{
    uint32_t number = in*pow(10, digit-1);

    return number? std::to_string(number).at(digit-1)-'0' : 0;
}

void methods::radixRating(std::vector<methods::playerData*> &inVec, uint8_t digits)
{
    int size = inVec.size();

    std::vector<methods::playerData*> auxVec(size);

    for(int digit = digits-1; digit >= 0; digit--) //Radix sort
    {
        //Counting sort

        //tamanho do vetor digitos == 10 (digitos de 0~9)
        std::vector<int> count(11, 0);

        for(int i = 0; i < size; i++)   //Incremento
            count.at(methods::digitOfFloat(inVec.at(i)->rating, digit+1)+1)++;

        for(int i = 0; i < 9; i++) //Ajuste
            count.at(i+1) += count.at(i);

        for(int i = 0; i < size; i++) //Distribuição
            auxVec.at(count.at(methods::digitOfFloat(inVec.at(i)->rating, digit+1))++) = inVec.at(i);

        inVec = auxVec;   //Copia
    }
}

uint16_t methods::digitOfInt(uint32_t in, uint8_t digit)
{
    return in? std::to_string(in).at(digit-1)-'0' : 0;
}

void methods::radixOverall(std::vector<methods::playerData*> &inVec, uint8_t digits)
{
    int size = inVec.size();

    std::vector<methods::playerData*> auxVec(size);

    for(int digit = digits-1; digit >= 0; digit--) //Radix sort
    {
        //Counting sort

        //tamanho do vetor digitos == 10 (digitos de 0~9)
        std::vector<int> count(11, 0);

        for(int i = 0; i < size; i++)   //Incremento
            count.at(methods::digitOfInt(inVec.at(i)->overall, digit+1)+1)++;

        for(int i = 0; i < 9; i++) //Ajuste
            count.at(i+1) += count.at(i);

        for(int i = 0; i < size; i++) //Distribuição
            auxVec.at(count.at(methods::digitOfInt(inVec.at(i)->overall, digit+1))++) = inVec.at(i);

        inVec = auxVec;   //Copia
    }
}

std::pair<std::vector<methods::playerData*>, std::vector<float> > methods::parsingUserData(std::vector<userData2> &vec)
{
    std::vector<methods::playerData*> playersOut;
    std::vector<float> ratingOut;

    for(auto element : vec)
    {
        playersOut.push_back(element.sofifa_id);
        ratingOut.push_back(element.rating);
    }

    return std::make_pair(playersOut, ratingOut);
}

void methods::radixRating(std::vector<methods::userData2> &inVec, uint8_t digits)
{
    int size = inVec.size();

    std::vector<methods::userData2> auxVec(size);

    for(int digit = digits-1; digit >= 0; digit--) //Radix sort
    {
        //Counting sort

        //tamanho do vetor digitos == 10 (digitos de 0~9)
        std::vector<int> count(11, 0);

        for(int i = 0; i < size; i++)   //Incremento
            count.at(methods::digitOfFloat(inVec.at(i).rating, digit+1)+1)++;

        for(int i = 0; i < 9; i++) //Ajuste
            count.at(i+1) += count.at(i);

        for(int i = 0; i < size; i++) //Distribuição
            auxVec.at(count.at(methods::digitOfFloat(inVec.at(i).rating, digit+1))++) = inVec.at(i);

        inVec = auxVec;   //Copia
    }
}

std::vector<methods::playerData*> methods::tagIntersection(std::vector<methods::playerData*> &vec1, std::vector<methods::playerData*> &vec2)
{
    std::vector<methods::playerData*> out;

    for(auto player : vec1)
        if(methods::stdVectorFind(player, vec2))
            out.push_back(player);

    return out;
}

std::vector<std::string> methods::parsingTags(std::string strIn)
{
    size_t start = 0;
    size_t end = 0;

    std::vector<std::string> out;

    while((start = strIn.find('\'', end)) != std::string::npos)
    {
        end = strIn.find('\'', start+1);

        out.push_back(strIn.substr(start+1, end-start-1));
        end++;
    }

    return out;
}

size_t methods::countChar(std::string strIn, char toCount)
{
    size_t ocurrences = 0;

    for(auto character : strIn) if(character == toCount) ocurrences++;

    return ocurrences;
}

void methods::removeSequenceDuplicated(std::vector<methods::playerData*> &vec)
{
    if(!vec.size()) return;

    for(size_t index = 0; index < vec.size()-1; index++)
    {
        if(vec.at(index) == vec.at(index + 1))
        {
            vec.erase(vec.begin()+index);
            index--;
        }
    }
}

void methods::invertVector(std::vector<methods::playerData*> &vec)
{
    //Metade do tamanho do vetor
    size_t halfSize = vec.size()/2;

    for(size_t i = 0, j = vec.size()-1; i < halfSize; i++, j--)
        std::swap(vec.at(i), vec.at(j));
}
