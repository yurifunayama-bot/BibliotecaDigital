#include "Biblioteca.h"
#include <iostream>
#include <algorithm>
#include <cctype>

static bool isbnValido(const std::string& isbn) {
    if (isbn.size() != 10 && isbn.size() != 13) {
        return false;
    }

    for (size_t i = 0; i < isbn.size(); ++i) {
        char c = isbn[i];

        if (std::isdigit(static_cast<unsigned char>(c))) {
            continue;
        }

        // ISBN-10 pode terminar com X ou x
        if (isbn.size() == 10 && i == 9 && (c == 'X' || c == 'x')) {
            continue;
        }

        return false;
    }

    return true;
}
static std::string normalizarTexto(std::string texto) {
    std::transform(
        texto.begin(),
        texto.end(),
        texto.begin(),
        [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        }
    );

    return texto;
}
// Inicializa o Bloom Filter com suporte a 150k registros e 1% (0.01) de margem de erro tolerável
Biblioteca::Biblioteca() 
    : tabelaHash(), arvoreAvl(), rankingHeap(), triePadrao(), trieComprimida(), bloomFilter(150000, 0.01), totalLivros(0) {}

Biblioteca::~Biblioteca() {
    // Gerenciamento Manual de Memória: Libera os objetos Livro alocados dinamicamente
    for (Livro* livro : listaLivros) {
        delete livro;
    }
    listaLivros.clear();
}

bool Biblioteca::inserirLivro(Livro* novoLivro) {
    if (!novoLivro) return false;

    if (!isbnValido(novoLivro->getIsbn())) {
        return false; // ISBN inválido, rejeita a inserção
    }

    if (tabelaHash.buscar(novoLivro->getIsbn()) != nullptr) {
        return false; // Evita duplicatas de ISBN na Hash Table
    }

    // 1. Alimenta o Filtro de Bloom (Barreira probabilística de portaria)
    bloomFilter.inserir(novoLivro->getIsbn());

    // 2. Indexa nas estruturas de busca principais
    tabelaHash.inserir(novoLivro->getIsbn(), novoLivro);

    std::string tituloNormalizado = normalizarTexto(novoLivro->getTitulo());

    arvoreAvl.inserir(tituloNormalizado, novoLivro);
    
    // 3. Indexa nas duas variações de árvore de prefixos (Trie)
    triePadrao.inserir(tituloNormalizado, novoLivro);
    trieComprimida.inserir(tituloNormalizado, novoLivro);

    // 4. Indexa na Fila de Prioridade de Ranking (Max-Heap)
    rankingHeap.inserir(novoLivro);

    // 5. Registra no vetor de amostragem global
    listaLivros.push_back(novoLivro);
    totalLivros++;

    return true; // Inserção bem-sucedida
}

bool Biblioteca::removerLivro(const std::string& isbn) {
    // Primeiro localiza o ponteiro do livro via Hash Table para saber o título
    Livro* livro = tabelaHash.buscar(isbn);
    if (!livro) return false; // Livro não existe

    std::string titulo = normalizarTexto(livro->getTitulo());

    // Remove o livro das estruturas indexadas
    tabelaHash.remover(isbn);
    arvoreAvl.remover(titulo, livro);
    triePadrao.remover(titulo, livro);
    trieComprimida.remover(titulo, livro);
    rankingHeap.remover(livro);

    // Remove do vetor de amostragem de ponteiros
    auto it = std::find(listaLivros.begin(), listaLivros.end(), livro);
    if (it != listaLivros.end()) {
        listaLivros.erase(it);
    }

    // Nota teórica de Engenharia: Filtros de Bloom padrão não suportam remoção de bits 
    // sem corromper outros dados. O objeto livro sairá da Hash, portanto, se o Bloom 
    // der um falso positivo no futuro, a Hash Table retornará nullptr com segurança.

    delete livro; // Liberação da memória real do objeto
    totalLivros--;
    return true;
}

void Biblioteca::registrarPesquisa(Livro* livro) {
    if (!livro) {
    return;
    }

    livro->incrementarPesquisas();
    atualizarRankingPreferencia();
}  

void Biblioteca::registrarPesquisas(const std::vector<Livro*>& livros) {
    bool houveAlteracao = false;

    for (Livro* livro : livros) {
        if (livro) {
            livro->incrementarPesquisas();
            houveAlteracao = true;
        }
    }

    if (houveAlteracao) {
        atualizarRankingPreferencia();
    }
}

Livro* Biblioteca::buscarPorIsbn(const std::string& isbn) {
    // Rápido curto-circuito: Se o Bloom Filter garantir que NÃO existe, pula a Hash Table
    if (!bloomFilter.provavelmenteExiste(isbn)) {
        return nullptr; 
    }
    Livro* livro = tabelaHash.buscar(isbn);
    registrarPesquisa(livro);
    return livro;
}
Livro* Biblioteca::buscarPorIsbnSemRegistrar(const std::string& isbn) {
    // Versão de consulta usada pelo benchmark: preserva métricas e ranking.
    if (!bloomFilter.provavelmenteExiste(isbn)) {
        return nullptr;
    }

    return tabelaHash.buscar(isbn);
}

Livro* Biblioteca::buscarPorIsbnSemBloom(const std::string& isbn) {
    // Ignora a portaria do Bloom Filter para fins de comparação de performance no benchmark
    return tabelaHash.buscar(isbn);
}

Livro* Biblioteca::buscarPorTitulo(const std::string& titulo) {
    std::string tituloNormalizado = normalizarTexto(titulo);

    std::vector<Livro*> resultados = arvoreAvl.buscar(tituloNormalizado);
    if (resultados.empty()) {
        return nullptr;
    }

    registrarPesquisa(resultados.front());
    return resultados.front();
}

Livro* Biblioteca::buscarPorTituloSemRegistrar(const std::string& titulo) {
    std::string tituloNormalizado = normalizarTexto(titulo);

    std::vector<Livro*> resultados = arvoreAvl.buscar(tituloNormalizado);
    if (resultados.empty()) {
        return nullptr;
    }

    return resultados.front();
}

std::vector<Livro*> Biblioteca::buscarPorPrefixo(const std::string& prefixo) {
    std::vector<Livro*> resultados = buscarPorPrefixoSemRegistrar(prefixo);

    registrarPesquisas(resultados);

    return resultados;
}

std::vector<Livro*> Biblioteca::buscarPorPrefixoSemRegistrar(const std::string& prefixo) {
    std::string prefixoNormalizado = normalizarTexto(prefixo);

    return triePadrao.buscarPorPrefixo(prefixoNormalizado);
}

std::vector<Livro*> Biblioteca::buscarPorPrefixoComprimido(const std::string& prefixo) {
    std::vector<Livro*> resultados = buscarPorPrefixoComprimidoSemRegistrar(prefixo);

    registrarPesquisas(resultados);

    return resultados;
}

std::vector<Livro*> Biblioteca::buscarPorPrefixoComprimidoSemRegistrar(const std::string& prefixo) {
    std::string prefixoNormalizado = normalizarTexto(prefixo);

    return trieComprimida.buscarPorPrefixo(prefixoNormalizado);
}

void Biblioteca::atualizarRankingPreferencia() {
    // Reconstrói o Max-Heap para reorganizar os nós com base nos scores atualizados de empréstimo/pesquisa
    rankingHeap.reconstruirHeap(listaLivros);
}

Livro* Biblioteca::obterLivroMaisEmprestadoViaHeap() {
    return rankingHeap.obterTopo(); // Retorna o elemento de maior prioridade em O(1)
}

Livro* Biblioteca::obterLivroMaisEmprestadoViaAvl() {
    // Em uma árvore AVL padrão ordenada por título, encontrar o maior score exige travessia O(N).
    // Este método demonstra por que o Max-Heap é a escolha correta de engenharia para rankings.
    return arvoreAvl.encontrarMaiorScoreEmprestimo(); 
}

void Biblioteca::expurgarLivrosObsoletos(size_t quantidadeParaRemover) {
    // Restrição R5: Descarte automático se estourar o teto físico de armazenamento em memória.
    // Estratégia: Remove os livros menos utilizados localizados na base do Heap ou coletando os menores scores.
    std::cout << "[POLITICA MEMORIA R5] Iniciando expurgo de " << quantidadeParaRemover << " livros obsoletos...\n";
    
    for (size_t i = 0; i < quantidadeParaRemover; ++i) {
        // Obtém o livro com menor relevância do sistema através do Heap adaptado
        Livro* obsoleto = rankingHeap.extrairMinimoRelevancia();
        if (obsoleto) {
            std::string isbnAlvo = obsoleto->getIsbn();
            // Desvincula e apaga de todas as outras estruturas
            std::string tituloAlvo = normalizarTexto(obsoleto->getTitulo());
            tabelaHash.remover(isbnAlvo);
            arvoreAvl.remover(tituloAlvo, obsoleto);
            triePadrao.remover(tituloAlvo, obsoleto);
            trieComprimida.remover(tituloAlvo, obsoleto);

            auto it = std::find(listaLivros.begin(), listaLivros.end(), obsoleto);
            if (it != listaLivros.end()) {
                listaLivros.erase(it);
            }

            delete obsoleto;
            totalLivros--;
        } else {
            break;
        }
    }
}

size_t Biblioteca::getTotalLivrosCadastrados() const {
    return totalLivros;
}

const std::vector<Livro*>& Biblioteca::getAmostraLivros() const {
    return listaLivros;
}

void Biblioteca::exibirEstatisticasGlobais() {
    std::cout << " -> Total de Livros Indexados: " << totalLivros << "\n";
    std::cout << " -> Fatores de Otimização Operacional:\n";
    std::cout << "    | Colisões Atuais na HashTable: " << tabelaHash.getTotalColisoes() << "\n";
    std::cout << "    | Altura da Árvore AVL de Títulos: " << arvoreAvl.getAltura() << "\n";
    std::cout << "    | Bits Alocados no Bloom Filter: " << bloomFilter.getTamanhoBits() << " bits\n";
    std::cout << "    | Funções de Hash Ativas (Bloom): " << bloomFilter.getNumHashFunctions() << "\n";
    
    Livro* top = obterLivroMaisEmprestadoViaHeap();
    if (top) {
        std::cout << "    | Livro Líder do Ranking: '" << top->getTitulo() << "' (" << top->getTotalAcessos() << " interações)\n";
    }
}