#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <string>
#include <vector>
#include "Livro.h"
#include "HashTable.h"   // Classe: HashTable
#include "AVL.h"         // Classe: AVL
#include "Heap.h"        // Classe: Heap
#include "Trie.h"        // Classe: Trie
#include "BloomFilter.h" // Classe: BloomFilter

class Biblioteca {
private:
    // Instanciação com a nomenclatura exata das classes do projeto
    HashTable tabelaHash;
    AVL arvoreAvl;
    Heap rankingHeap;
    Trie triePadrao;
    CompressedTrie trieComprimida;
    BloomFilter bloomFilter;

    // Vetor de rastreamento para amostragem e libertação de memória
    std::vector<Livro*> listaLivros;
    size_t totalLivros;
    void registrarPesquisa(Livro* livro);
    void registrarPesquisas(const std::vector<Livro*>& livros);

public:
    Biblioteca();
    ~Biblioteca();

    // Garantia estrita contra cópias acidentais (SOLID / Rule of Three)
    Biblioteca(const Biblioteca&) = delete;
    Biblioteca& operator=(const Biblioteca&) = delete;

    // Operações do ciclo de vida do acervo
    bool inserirLivro(Livro* novoLivro);
    bool removerLivro(const std::string& isbn);

    // Métodos de consulta e busca indexada

    Livro* buscarPorIsbn(const std::string& isbn);          // Com Bloom Filter
    Livro* buscarPorIsbnSemBloom(const std::string& isbn);   // Direto na Hash Table (Benchmark)
    Livro* buscarPorIsbnSemRegistrar(const std::string& isbn);
    Livro* buscarPorTitulo(const std::string& titulo);       // Via Árvore AVL
    Livro* buscarPorTituloSemRegistrar(const std::string& titulo);
    std::vector<Livro*> buscarPorPrefixo(const std::string& prefixo);           // Via Trie Padrão e registro de pesquisas
    std::vector<Livro*> buscarPorPrefixoSemRegistrar(const std::string& prefixo); // Via Trie Padrão, sem alterar estatísticas
    std::vector<Livro*> buscarPorPrefixoComprimido(const std::string& prefixo);  // Via Radix Tree e registro de pesquisas
    std::vector<Livro*> buscarPorPrefixoComprimidoSemRegistrar(const std::string& prefixo); // Via Radix Tree, sem alterar estatísticas


    // Métodos de suporte a Rankings e Gestão de Memória (R5)
    void atualizarRankingPreferencia();
    Livro* obterLivroMaisEmprestadoViaHeap();
    Livro* obterLivroMaisEmprestadoViaAvl();
    void expurgarLivrosObsoletos(size_t quantidadeParaRemover);

    // Métricas de monitoramento
    size_t getTotalLivrosCadastrados() const;
    const std::vector<Livro*>& getAmostraLivros() const;
    void exibirEstatisticasGlobais();
};

#endif