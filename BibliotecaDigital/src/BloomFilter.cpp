#include "BloomFilter.h"
#include <cmath>
#include <algorithm>

BloomFilter::BloomFilter(size_t capacidadePrevista, double taxaFalsoPositivo) {
    // Fórmulas Matemáticas de Otimização de Espaço de Bloom:
    // m = - (n * ln(p)) / (ln(2)^2)
    // k = (m / n) * ln(2)
    
    double ln2 = std::log(2.0);
    double m = - (static_cast<double>(capacidadePrevista) * std::log(taxaFalsoPositivo)) / (ln2 * ln2);
    double k = (m / static_cast<double>(capacidadePrevista)) * ln2;

    tamanhoBits = static_cast<size_t>(std::ceil(m));
    numHashFunctions = std::max(static_cast<size_t>(1), static_cast<size_t>(std::round(k)));

    // Determina quantos bytes inteiros são necessários para acomodar a quantidade de bits calculada
    size_t tamanhoBytes = (tamanhoBits + 7) / 8;
    tabelaBits.resize(tamanhoBytes, 0);
}

uint64_t BloomFilter::fnv1a(const std::string& chave) const {
    uint64_t hash = 14695981039346656037ULL; // FNV offset basis
    for (char c : chave) {
        hash ^= static_cast<uint64_t>(c);
        hash *= 1099511628211ULL; // FNV prime
    }
    return hash;
}

uint64_t BloomFilter::djb2(const std::string& chave) const {
    uint64_t hash = 5381;
    for (char c : chave) {
        hash = ((hash << 5) + hash) + static_cast<uint64_t>(c);
    }
    return hash;
}

void BloomFilter::setBit(size_t indice) {
    size_t bytePos = indice / 8;
    size_t bitPos = indice % 8;
    tabelaBits[bytePos] |= (1 << bitPos); // Operação Bitwise OR ativa o bit específico
}

bool BloomFilter::getBit(size_t indice) const {
    size_t bytePos = indice / 8;
    size_t bitPos = indice % 8;
    return (tabelaBits[bytePos] & (1 << bitPos)) != 0; // Operação Bitwise AND valida a existência
}

void BloomFilter::inserir(const std::string& isbn) {
    uint64_t h1 = fnv1a(isbn);
    uint64_t h2 = djb2(isbn);

    // Kirsch-Mitzenmacher: h_i(x) = (h1(x) + i * h2(x)) % m
    for (size_t i = 0; i < numHashFunctions; ++i) {
        size_t indiceCombinado = (h1 + i * h2) % tamanhoBits;
        setBit(indiceCombinado);
    }
}

bool BloomFilter::provavelmenteExiste(const std::string& isbn) const {
    uint64_t h1 = fnv1a(isbn);
    uint64_t h2 = djb2(isbn);

    for (size_t i = 0; i < numHashFunctions; ++i) {
        size_t indiceCombinado = (h1 + i * h2) % tamanhoBits;
        if (!getBit(indiceCombinado)) {
            // Se um único bit do padrão estiver desativado, o elemento GARANTIDAMENTE NÃO existe 
            return false;
        }
    }
    // Se todos os bits estiverem ativos, o elemento provavelmente existe (sujeito a falso positivo) [cite: 101]
    return true; 
}

size_t BloomFilter::getTamanhoBits() const {
    return tamanhoBits;
}

size_t BloomFilter::getNumHashFunctions() const {
    return numHashFunctions;
}