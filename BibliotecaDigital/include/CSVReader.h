#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include <vector>
#include "Livro.h"

class CSVReader {
private:
    // Método auxiliar para quebrar uma linha do CSV respeitando aspas duplas (R16)
    static std::vector<std::string> tokenizarLinha(const std::string& linha);

public:
    // Carrega o dataset e retorna um vetor de ponteiros alocados dinamicamente
    static std::vector<Livro*> carregarLivros(const std::string& caminhoArquivo);
};

#endif