#ifndef LIVRO_H
#define LIVRO_H

#include <string>

class Livro {
private:
    int id;
    std::string isbn;
    std::string isbn13;
    std::string titulo;
    std::string autor;
    std::string editora;
    int anoPublicacao;
    std::string idioma;
    int numeroPaginas;
    double avaliacaoMedia;
    int quantidadeAvaliacoes;
    int emprestimos;
    int pesquisas;

public:
        Livro(
        int id,
        std::string isbn,
        std::string isbn13,
        std::string titulo,
        std::string autor,
        std::string editora,
        int anoPublicacao,
        std::string idioma,
        int numeroPaginas,
        double avaliacaoMedia,
        int quantidadeAvaliacoes
    );

    Livro(
        std::string isbn,
        std::string titulo,
        std::string autor,
        int anoPublicacao
    );

    // Getters
    int getId() const;
    std::string getIsbn() const;
    std::string getIsbn13() const;
    std::string getTitulo() const;
    std::string getAutor() const;
    std::string getEditora() const;
    int getAnoPublicacao() const;
    std::string getIdioma() const;
    int getNumeroPaginas() const;
    double getAvaliacaoMedia() const;
    int getQuantidadeAvaliacoes() const;
    int getEmprestimos() const;
    int getPesquisas() const;
    int getTotalAcessos() const;

    // Setters e Modificadores
    void setTitulo(const std::string& novoTitulo);
    void setAutor(const std::string& novoAutor);
    void setEditora(const std::string& novaEditora);
    void setAnoPublicacao(int novoAno);
    void setIdioma(const std::string& novoIdioma);
    void setNumeroPaginas(int novasPaginas);
    void setAvaliacaoMedia(double novaAvaliacao);
    void setQuantidadeAvaliacoes(int novaQuantidade);
    
    void incrementarEmprestimos();
    void incrementarPesquisas();
};

#endif