#include "Usuario.h"

Usuario::Usuario(int id, std::string nome) 
    : id(id), nome(std::move(nome)) {}

int Usuario::getId() const { return id; }
std::string Usuario::getNome() const { return nome; }
void Usuario::setNome(const std::string& novoNome) { nome = novoNome; }