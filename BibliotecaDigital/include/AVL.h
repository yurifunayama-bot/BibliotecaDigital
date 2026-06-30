#ifndef AVL_H
#define AVL_H

#include <string>
#include <vector>
#include "Livro.h"

// Nó estrutural da Árvore AVL
struct AVLNode {
    std::string titulo;
    std::vector<Livro*> livros; // Vetor de ponteiros para tratar títulos duplicados de forma limpa
    AVLNode* esquerdo;
    AVLNode* direito;
    int altura;

    AVLNode(std::string t, Livro* l)
        : titulo(std::move(t)), esquerdo(nullptr), direito(nullptr), altura(1) {
        livros.push_back(l);
    }
};

class AVL {
private:
    AVLNode* raiz;

    // Métodos utilitários e recursivos privados
    int getAltura(AVLNode* no) const;
    int getFatorBalanceamento(AVLNode* no) const;
    
    AVLNode* rotacaoDireita(AVLNode* y);
    AVLNode* rotacaoEsquerda(AVLNode* x);
    
    AVLNode* inserir(AVLNode* no, const std::string& titulo, Livro* livro);
    AVLNode* noComValorMinimo(AVLNode* no) const;
    AVLNode* removerNoFisico(AVLNode* no, const std::string& titulo);
    AVLNode* removerLivroDaArvore(AVLNode* no, const std::string& titulo, Livro* livro, bool& removido);
    
    void destruirArvore(AVLNode* no);

public:
    AVL();
    ~AVL();

    // Impedir cópias inseguras de ponteiros (Rule of Three)
    AVL(const AVL&) = delete;
    AVL& operator=(const AVL&) = delete;

    // Operações principais expostas ao sistema
    void inserir(const std::string& titulo, Livro* livro);
    std::vector<Livro*> buscar(const std::string& titulo) const;
    bool remover(const std::string& titulo);
    // Getter da raiz para fins de travessia e testes analíticos
    AVLNode* getRaiz() const;

    bool remover(const std::string& titulo, Livro* livro);
    int getAltura() const;
    Livro* encontrarMaiorScoreEmprestimo() const;
    void procurarMaior(AVLNode* no, Livro*& melhor) const;
};

#endif