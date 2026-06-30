#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "Biblioteca.h"
#include <random>
#include <string>

class Simulador {
private:
    Biblioteca& biblioteca;
    std::mt19937 gerador;
    
    // Métodos internos para geração de dados sintéticos e realistas de teste
    std::string gerarIsbnAleatorio();
    std::string gerarTituloAleatorio();
    std::string gerarAutorAleatorio();

    // Executores internos de cenários stress-test
    void cenarioInsercaoContinuaR25();
    void cenarioConsultasMassivasR9();
    void cenarioEmprestimosEDevolvidos();
    void cenarioDescarteAutomaticoR5(size_t limiteMaximoItens);

public:
    explicit Simulador(Biblioteca& bib);
    ~Simulador() = default;

    // Bloqueio de cópias para segurança estrutural
    Simulador(const Simulador&) = delete;
    Simulador& operator=(const Simulador&) = delete;

    // Orquestrador principal da simulação por ciclos iterativos
    void executarSimulacao(size_t numCiclos, size_t limiteMemoriaItens);
};

#endif