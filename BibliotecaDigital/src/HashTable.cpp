#include "HashTable.h"
#include <iostream>

size_t HashTable::calcularHash(const std::string& chave) const {
    // Algoritmo DJB2: Altamente eficiente para strings com excelente distribuição
    unsigned long hash = 5381;
    for (char c : chave) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % capacidade;
}

HashTable::HashTable(size_t capacidadeInicial) 
    : capacidade(capacidadeInicial), tamanho(0), totalColisoes(0) {
    // Aloca o array de ponteiros de nós e inicializa todos com nullptr
    tabela = new HashNode*[capacidade]();
}

HashTable::~HashTable() {
    for (size_t i = 0; i < capacidade; ++i) {
        HashNode* atual = tabela[i];
        while (atual != nullptr) {
            HashNode* temporario = atual;
            atual = atual->proximo;
            // ATENÇÃO: Deleta apenas o nó estrutural da Hash. 
            // O objeto Livro* NÃO é deletado aqui, pois seu ciclo de vida é gerido pela classe Biblioteca.
            delete temporario; 
        }
    }
    delete[] tabela;
}

void HashTable::redimensionar() {
    size_t antigaCapacidade = capacidade;
    HashNode** antigaTabela = tabela;

    // Expande a capacidade para manter o fator de carga sob controle
    capacidade = antigaCapacidade * 2 + 1;
    tabela = new HashNode*[capacidade]();
    
    // Reseta contadores temporariamente para o recálculo na reinserção
    tamanho = 0;
    totalColisoes = 0;

    for (size_t i = 0; i < antigaCapacidade; ++i) {
        HashNode* atual = antigaTabela[i];
        while (atual != nullptr) {
            HashNode* proximoNode = atual->proximo;
            
            // Recalcula o índice na nova tabela expandida
            size_t novoIndice = calcularHash(atual->chave);
            
            if (tabela[novoIndice] != nullptr) {
                totalColisoes++;
            }
            
            // Reassocia o nó diretamente na nova tabela sem dar 'new' (Otimização de memória)
            atual->proximo = tabela[novoIndice];
            tabela[novoIndice] = atual;
            tamanho++;

            atual = proximoNode;
        }
    }
    delete[] antigaTabela;
}

bool HashTable::inserir(const std::string& chave, Livro* valor) {
    // Se o fator de carga ultrapassar 0.75, realiza o Rehash preventivo
    if (getFatorCarga() > 0.75) {
        redimensionar();
    }

    size_t indice = calcularHash(chave);
    HashNode* atual = tabela[indice];

    // Varre a lista do bucket para checar se a chave já existe (evita duplicatas de ISBN)
    while (atual != nullptr) {
        if (atual->chave == chave) {
            atual->valor = valor; // Atualiza o ponteiro se já existir
            return true;
        }
        atual = atual->proximo;
    }

    // Se o topo do bucket não for nulo, detectamos uma colisão analítica
    if (tabela[indice] != nullptr) {
        totalColisoes++;
    }

    // Insere o novo nó no início da lista encadeada (Operação O(1))
    HashNode* novoNo = new HashNode(chave, valor);
    novoNo->proximo = tabela[indice];
    tabela[indice] = novoNo;
    tamanho++;
    return true;
}

Livro* HashTable::buscar(const std::string& chave) const {
    size_t indice = calcularHash(chave);
    HashNode* atual = tabela[indice];

    while (atual != nullptr) {
        if (atual->chave == chave) {
            return atual->valor;
        }
        atual = atual->proximo;
    }
    return nullptr; // Caso o livro com esse ISBN não exista
}

bool HashTable::remover(const std::string& chave) {
    size_t indice = calcularHash(chave);
    HashNode* atual = tabela[indice];
    HashNode* anterior = nullptr;

    while (atual != nullptr) {
        if (atual->chave == chave) {
            if (anterior == nullptr) {
                // O elemento a ser removido está no topo do bucket
                tabela[indice] = atual->proximo;
            } else {
                // Remove o elemento costurando o ponteiro do anterior com o próximo
                anterior->proximo = atual->proximo;
            }
            delete atual;
            tamanho--;
            return true;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    return false; // Não encontrado para remoção
}

size_t HashTable::getTamanho() const { return tamanho; }
size_t HashTable::getCapacidade() const { return capacidade; }
size_t HashTable::getTotalColisoes() const { return totalColisoes; }
double HashTable::getFatorCarga() const { return static_cast<double>(tamanho) / capacidade; }