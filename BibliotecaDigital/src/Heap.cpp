#include "Heap.h"
#include <utility>

Heap::Heap() {}

void Heap::heapifyUp(size_t indice) {
    if (indice == 0) return;

    size_t pai = (indice - 1) / 2;
    
    // Critério de Max-Heap: Pai deve ter maior ou igual número de empréstimos
    if (dados[indice]->getTotalAcessos() > dados[pai]->getTotalAcessos()) {
        std::swap(dados[indice], dados[pai]);
        heapifyUp(pai); // Propaga recursivamente em direção à raiz
    }
}

void Heap::heapifyDown(size_t indice) {
    size_t maior = indice;
    size_t esquerdo = 2 * indice + 1;
    size_t direito = 2 * indice + 2;

    // Compara o nó atual com o filho esquerdo
    if (esquerdo < dados.size() && dados[esquerdo]->getTotalAcessos() > dados[maior]->getTotalAcessos()) {
        maior = esquerdo;
    }

    // Compara o resultado com o filho direito
    if (direito < dados.size() && dados[direito]->getTotalAcessos() > dados[maior]->getTotalAcessos()) {
        maior = direito;
    }

    // Se um dos filhos for maior, realiza a troca e continua a descida
    if (maior != indice) {
        std::swap(dados[indice], dados[maior]);
        heapifyDown(maior);
    }
}

void Heap::inserir(Livro* livro) {
    dados.push_back(livro);
    heapifyUp(dados.size() - 1);
}

Livro* Heap::extrairMaximo() {
    if (dados.empty()) return nullptr;

    Livro* maximo = dados[0];
    
    // Move o último elemento para a raiz e remove a duplicata no fim
    dados[0] = dados.back();
    dados.pop_back();

    // Rebalanceia o Heap a partir da raiz se a estrutura não estiver vazia
    if (!dados.empty()) {
        heapifyDown(0);
    }

    return maximo;
}

Livro* Heap::obterMaximo() const {
    if (dados.empty()) return nullptr;
    return dados[0];
}

bool Heap::estaVazio() const {
    return dados.empty();
}

size_t Heap::getTamanho() const {
    return dados.size();
}

void Heap::construirHeap(const std::vector<Livro*>& livros) {
    dados = livros;
    if (dados.empty()) return;
    
    // Algoritmo de Floyd para construção de Heap (Bottom-Up)
    // Começa do último nó que possui filhos até a raiz
    for (long long i = (static_cast<long long>(dados.size()) / 2) - 1; i >= 0; --i) {
        heapifyDown(static_cast<size_t>(i));
    }
}

void Heap::limpar() {
    dados.clear();
}

void Heap::remover(Livro* livro){
    for (size_t i = 0; i < dados.size(); i++)
    {
        if (dados[i] == livro)
        {
            dados[i] = dados.back();
            dados.pop_back();

            if (i < dados.size())
            {
                heapifyDown(i);
                heapifyUp(i);
            }

            return;
        }
    }
}

Livro* Heap::obterTopo() const {
    return obterMaximo();
}

void Heap::reconstruirHeap(const std::vector<Livro*>& livros) {
    construirHeap(livros);
}

Livro* Heap::extrairMinimoRelevancia(){
    if (dados.empty())
        return nullptr;

    size_t indiceMenor = 0;

    for (size_t i = 1; i < dados.size(); i++)
    {
        if (dados[i]->getTotalAcessos()
            < dados[indiceMenor]->getTotalAcessos())
        {
            indiceMenor = i;
        }
    }

    Livro* resultado = dados[indiceMenor];

    dados[indiceMenor] = dados.back();
    dados.pop_back();

    if (indiceMenor < dados.size())
    {
        heapifyDown(indiceMenor);
        heapifyUp(indiceMenor);
    }

    return resultado;
}