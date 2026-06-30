#include "Emprestimo.h"

Emprestimo::Emprestimo(int idUsuario, std::string isbn, std::string dataEmprestimo, std::string dataPrevistaDevolucao)
    : idUsuario(idUsuario), isbn(std::move(isbn)), dataEmprestimo(std::move(dataEmprestimo)), dataPrevistaDevolucao(std::move(dataPrevistaDevolucao)) {}

int Emprestimo::getIdUsuario() const { return idUsuario; }
std::string Emprestimo::getIsbn() const { return isbn; }
std::string Emprestimo::getDataEmprestimo() const { return dataEmprestimo; }
std::string Emprestimo::getDataPrevistaDevolucao() const { return dataPrevistaDevolucao; }