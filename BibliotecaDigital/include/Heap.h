#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include "Livro.h"

class Heap {
private:
    std::vector<Livro*> dados; // Vetor subjacente para representação da árvore binária

    // Métodos auxiliares de reestruturação do Heap
    void heapifyUp(size_t indice);
    void heapifyDown(size_t indice);

public:
    Heap();
    ~Heap() = default;

    // Desativar cópia para proteção de referências
    Heap(const Heap&) = delete;
    Heap& operator=(const Heap&) = delete;

    // Operações fundamentais de uma Fila de Prioridade
    void inserir(Livro* livro);
    Livro* extrairMaximo();
    Livro* obterMaximo() const;
    bool estaVazio() const;
    size_t getTamanho() const;

    // Construção eficiente Bottom-Up em O(N) para processar grandes volumes
    void construirHeap(const std::vector<Livro*>& livros);
    
    // Reseta a estrutura interna
    void limpar();

    void remover(Livro* livro);

    void reconstruirHeap(const std::vector<Livro*>& livros);

    Livro* obterTopo() const;

    Livro* extrairMinimoRelevancia();
};

#endif