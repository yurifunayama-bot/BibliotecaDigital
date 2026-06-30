#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include "Livro.h"

// Nó interno da lista encadeada para tratamento de colisões por encadeamento
struct HashNode {
    std::string chave;
    Livro* valor;
    HashNode* proximo;

    HashNode(std::string k, Livro* v) 
        : chave(std::move(k)), valor(v), proximo(nullptr) {}
};

class HashTable {
private:
    HashNode** tabela;
    size_t capacidade;
    size_t tamanho;
    size_t totalColisoes;

    // Função de espalhamento (Hash Function) baseada no algoritmo DJB2
    size_t calcularHash(const std::string& chave) const;
    
    // Aloca uma nova tabela maior e realoca todos os nós existentes (Rehash)
    void redimensionar();

public:
    HashTable(size_t capacidadeInicial = 10003); // Uso de número primo inicial
    ~HashTable();

    // Desabilitar cópia e atribuição para proteção de ponteiros duplos (Rule of Three/Five)
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    bool inserir(const std::string& chave, Livro* valor);
    Livro* buscar(const std::string& chave) const;
    bool remover(const std::string& chave);

    // Métodos de Telemetria e Métricas (Requisito do Benchmark)
    size_t getTamanho() const;
    size_t getCapacidade() const;
    size_t getTotalColisoes() const;
    double getFatorCarga() const;
};

#endif