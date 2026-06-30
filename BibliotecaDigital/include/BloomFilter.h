#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <string>
#include <vector>
#include <cstdint>
#include <cstddef>

class BloomFilter {
private:
    size_t tamanhoBits;
    size_t numHashFunctions;
    std::vector<uint8_t> tabelaBits; // Armazenamento e manipulação manual de bits

    // Funções fundamentais de Hash para a combinação de Kirsch-Mitzenmacher
    uint64_t fnv1a(const std::string& chave) const;
    uint64_t djb2(const std::string& chave) const;

    // Métodos auxiliares de manipulação de bits a nível de bitwise
    void setBit(size_t indice);
    bool getBit(size_t indice) const;

public:
    // Dimensionamento automático com base na capacidade esperada e taxa tolerável de falsos positivos
    BloomFilter(size_t capacidadePrevista, double taxaFalsoPositivo);
    ~BloomFilter() = default;

    // Proteção de ciclo de vida (Rule of Three)
    BloomFilter(const BloomFilter&) = delete;
    BloomFilter& operator=(const BloomFilter&) = delete;

    // Operações base do Filtro de Bloom
    void inserir(const std::string& isbn);
    bool provavelmenteExiste(const std::string& isbn) const;

    // Getters para telemetria de benchmarks e relatórios experimentais
    size_t getTamanhoBits() const;
    size_t getNumHashFunctions() const;
};

#endif