#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "Biblioteca.h"
#include <string>
#include <vector>

class Benchmark {
private:
    Biblioteca& biblioteca;

    // Auxiliares para exportação de dados estruturados
    void salvarParaCSV(const std::string& nomeFicheiro, const std::string& cabecalho, const std::vector<std::string>& linhas);

public:
    explicit Benchmark(Biblioteca& bib);
    ~Benchmark() = default;

    // Bloqueio de cópia para assegurar integridade dos testes
    Benchmark(const Benchmark&) = delete;
    Benchmark& operator=(const Benchmark&) = delete;

    // Comparações experimentais metodologicamente compatíveis
    void executarBenchmarkTextual(const std::vector<std::string>& titulosAlvo);
    void executarBenchmarkTrie(const std::vector<std::string>& prefixosAlvo);
    void executarBenchmarkExistencia(const std::vector<std::string>& isbnsTeste);
    void executarBenchmarkRanking();

    // Orquestrador completo que executa todos os testes em lote
    void executarTodosOsBenchmarks(const std::vector<Livro*>& amostraLivros);
};

#endif
