#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "Biblioteca.h"
#include <string>
#include <vector>

struct ResultadoBenchmark {
    std::string cenario;
    std::string estrutura;
    std::string operacao;
    size_t tamanhoDataset;
    size_t numeroOperacoes;
    double tempoTotalMs;
    double tempoMedioMs;
    size_t memoriaEstimadaBytes;
    size_t colisoes;
    std::string complexidadeTeorica;
    std::string observacao;
};
class Benchmark {
    private:
    Biblioteca& biblioteca;

    // Auxiliares para exportação de dados estruturados
    void salvarParaCSV(const std::string& nomeFicheiro, const std::string& cabecalho, const std::vector<std::string>& linhas);

    void salvarResultadosPadronizados(
    const std::vector<ResultadoBenchmark>& resultados,
    const std::string& caminhoArquivo
    );

    std::vector<Livro*> prepararAmostra(
    const std::vector<Livro*>& origem,
    size_t limite
    ) const;

    std::vector<std::string> gerarIsbnsInexistentes(
    size_t quantidade
    ) const;

    size_t estimarMemoriaHash(size_t n, size_t capacidade) const;
    size_t estimarMemoriaAVL(size_t n) const;
    size_t estimarMemoriaHeap(size_t n) const;
    size_t estimarMemoriaTriePadrao(const std::vector<Livro*>& amostra) const;
    size_t estimarMemoriaTrieComprimida(const std::vector<Livro*>& amostra) const;
    void garantirDiretorioOutput() const;   



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

    // Métodos de benchmark detalhados para cada operação do ciclo de vida
    void executarBenchmarkInsercao(const std::vector<Livro*>& amostraLivros, std::vector<ResultadoBenchmark>& resultados);
    void executarBenchmarkBusca(const std::vector<Livro*>& amostraLivros, std::vector<ResultadoBenchmark>& resultados);
    void executarBenchmarkRemocao(const std::vector<Livro*>& amostraLivros, std::vector<ResultadoBenchmark>& resultados);
    void executarBenchmarkMemoria(const std::vector<Livro*>& amostraLivros, std::vector<ResultadoBenchmark>& resultados);
    void executarBenchmarkEscalabilidade(const std::vector<Livro*>& amostraLivros, std::vector<ResultadoBenchmark>& resultados);
    void executarBenchmarkTempoMedioConsulta(const std::vector<Livro*>& amostraLivros, std::vector<ResultadoBenchmark>& resultados);
    void executarBenchmarkLatenciaMedia(const std::vector<Livro*>& amostraLivros, std::vector<ResultadoBenchmark>& resultados);

    // Orquestrador completo que executa todos os testes em lote
    void executarTodosOsBenchmarks(const std::vector<Livro*>& amostraLivros);
};  
#endif
