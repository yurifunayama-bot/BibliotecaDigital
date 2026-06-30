#ifndef EMPRESTIMO_H
#define EMPRESTIMO_H

#include <string>

class Emprestimo {
private:
    int idUsuario;
    std::string isbn;
    std::string dataEmprestimo;
    std::string dataPrevistaDevolucao;

public:
    Emprestimo(int idUsuario, std::string isbn, std::string dataEmprestimo, std::string dataPrevistaDevolucao);

    int getIdUsuario() const;
    std::string getIsbn() const;
    std::string getDataEmprestimo() const;
    std::string getDataPrevistaDevolucao() const;
};

#endif