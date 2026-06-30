#ifndef USUARIO_H
#define USUARIO_H

#include <string>

class Usuario {
private:
    int id;
    std::string nome;

public:
    Usuario(int id, std::string nome);

    int getId() const;
    std::string getNome() const;
    void setNome(const std::string& novoNome);
};

#endif