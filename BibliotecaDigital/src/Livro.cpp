#include "Livro.h"

Livro::Livro(int id, std::string isbn, std::string isbn13, std::string titulo,
             std::string autor, std::string editora, int anoPublicacao,
             std::string idioma, int numeroPaginas, double avaliacaoMedia,
             int quantidadeAvaliacoes)
    : id(id), isbn(std::move(isbn)), isbn13(std::move(isbn13)), titulo(std::move(titulo)),
      autor(std::move(autor)), editora(std::move(editora)), anoPublicacao(anoPublicacao),
      idioma(std::move(idioma)), numeroPaginas(numeroPaginas), avaliacaoMedia(avaliacaoMedia),
      quantidadeAvaliacoes(quantidadeAvaliacoes), emprestimos(0), pesquisas(0) {}

Livro::Livro(std::string isbn, std::string titulo, std::string autor, int anoPublicacao)
    : id(0), isbn(std::move(isbn)), isbn13(""), titulo(std::move(titulo)), 
    autor(std::move(autor)), editora(""), anoPublicacao(anoPublicacao), idioma(""), 
    numeroPaginas(0), avaliacaoMedia(0.0), quantidadeAvaliacoes(0), emprestimos(0), pesquisas(0) {}


int Livro::getId() const { return id; }
std::string Livro::getIsbn() const { return isbn; }
std::string Livro::getIsbn13() const { return isbn13; }
std::string Livro::getTitulo() const { return titulo; }
std::string Livro::getAutor() const { return autor; }
std::string Livro::getEditora() const { return editora; }
int Livro::getAnoPublicacao() const { return anoPublicacao; }
std::string Livro::getIdioma() const { return idioma; }
int Livro::getNumeroPaginas() const { return numeroPaginas; }
double Livro::getAvaliacaoMedia() const { return avaliacaoMedia; }
int Livro::getQuantidadeAvaliacoes() const { return quantidadeAvaliacoes; }
int Livro::getEmprestimos() const { return emprestimos; }
int Livro::getPesquisas() const { return pesquisas; }
int Livro::getTotalAcessos() const { return emprestimos + pesquisas; }

void Livro::setTitulo(const std::string& novoTitulo) { titulo = novoTitulo; }
void Livro::setAutor(const std::string& novoAutor) { autor = novoAutor; }
void Livro::setEditora(const std::string& novaEditora) { editora = novaEditora; }
void Livro::setAnoPublicacao(int novoAno) { anoPublicacao = novoAno; }
void Livro::setIdioma(const std::string& novoIdioma) { idioma = novoIdioma; }
void Livro::setNumeroPaginas(int novasPaginas) { numeroPaginas = novasPaginas; }
void Livro::setAvaliacaoMedia(double novaAvaliacao) { avaliacaoMedia = novaAvaliacao; }
void Livro::setQuantidadeAvaliacoes(int novaQuantidade) { quantidadeAvaliacoes = novaQuantidade; }

void Livro::incrementarEmprestimos() { emprestimos++; }
void Livro::incrementarPesquisas() { pesquisas++; }

