#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include "Livro.h"

// ============================================================================
// 1. TRIE PADRÃO (STANDARD TRIE)
// ============================================================================
struct TrieNode {
    // Matriz de apontadores para os 256 caracteres ASCII (Explicita o overhead de memória)
    TrieNode* filhos[256];
    std::vector<Livro*> livros;
    bool isTerminal;

    TrieNode();
    ~TrieNode();
};

class Trie {
private:
    TrieNode* raiz;

    void coletarTodosLivros(TrieNode* no, std::vector<Livro*>& resultado) const;
    void destruir(TrieNode* no);

public:
    Trie();
    ~Trie();

    // Impedir cópia por segurança de ponteiros
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;

    void inserir(const std::string& termo, Livro* livro);
    bool remover(const std::string& termo);
    bool remover(const std::string& termo, Livro* livro);
    std::vector<Livro*> buscarPorPrefixo(const std::string& prefixo) const;
};

// ============================================================================
// 2. TRIE COMPRIMIDA (RADIX TREE / PATRICIA TRIE)
// ============================================================================
struct CompressedTrieNode {
    struct Aresta {
        std::string rotulo; // Segmento de string compactado na aresta
        CompressedTrieNode* destino;
    };

    std::vector<Aresta> filhos; // Armazenamento dinâmico otimiza espaço drasticamente
    std::vector<Livro*> livros;
    bool isTerminal;

    CompressedTrieNode();
    ~CompressedTrieNode();
};

class CompressedTrie {
private:
    CompressedTrieNode* raiz;

    void coletarTodosLivros(CompressedTrieNode* no, std::vector<Livro*>& resultado) const;
    void destruir(CompressedTrieNode* no);

public:
    CompressedTrie();
    ~CompressedTrie();

    // Impedir cópia por segurança de ponteiros
    CompressedTrie(const CompressedTrie&) = delete;
    CompressedTrie& operator=(const CompressedTrie&) = delete;

    void inserir(const std::string& termo, Livro* livro);
    bool remover(const std::string& termo);
    bool remover(const std::string& termo, Livro* livro);
    std::vector<Livro*> buscarPorPrefixo(const std::string& prefixo) const;
};

#endif