#include "Simulador.h"
#include <iostream>
#include <chrono>

Simulador::Simulador(Biblioteca& bib) 
    : biblioteca(bib), gerador(std::random_device{}()) {}

std::string Simulador::gerarIsbnAleatorio() {
    std::uniform_int_distribution<long long> dist(1000000000000LL, 9999999999999LL);
    return std::to_string(dist(gerador));
}

std::string Simulador::gerarTituloAleatorio() {
    static const std::vector<std::string> prefixos = {"O Segredo do", "A Historia de", "Guia Avançado de", "Introdução ao", "Crónicas de"};
    static const std::vector<std::string> nucleos = {"C++ moderno", "Algoritmos", "Sistemas Inteligentes", "Estruturas de Dados", "Universo"};
    
    std::uniform_int_distribution<size_t> distPrefix(0, prefixos.size() - 1);
    std::uniform_int_distribution<size_t> distNucl(0, nucleos.size() - 1);
    
    return prefixos[distPrefix(gerador)] + " " + nucleos[distNucl(gerador)];
}

std::string Simulador::gerarAutorAleatorio() {
    static const std::vector<std::string> nomes = {"Bjarne Stroustrup", "Donald Knuth", "Alan Turing", "Grace Hopper", "Linus Torvalds"};
    std::uniform_int_distribution<size_t> dist(0, nomes.size() - 1);
    return nomes[dist(gerador)];
}

void Simulador::cenarioInsercaoContinuaR25() {
    // R25: Entrada contínua de novos livros no acervo global
    std::string isbn = gerarIsbnAleatorio();
    std::string titulo = gerarTituloAleatorio();
    std::string autor = gerarAutorAleatorio();
    int ano = 2000 + (gerador() % 27);

    auto* novoLivro = new Livro(isbn, titulo, autor, ano);

    if (!biblioteca.inserirLivro(novoLivro)) {
    delete novoLivro;
    }
}

void Simulador::cenarioConsultasMassivasR9() {
    // R9: Grande volume de consultas simultâneas simuladas por amostragem
    // Sorteia o tipo de busca para estressar diferentes estruturas de dados
    int tipoBusca = gerador() % 3;

    if (tipoBusca == 0) {
        // Busca por ISBN (Passa primeiro pelo Bloom Filter antes da Hash Table)
        std::string isbnFicticio = gerarIsbnAleatorio();
        biblioteca.buscarPorIsbn(isbnFicticio); 
    } 
    else if (tipoBusca == 1) {
        // Busca por Título Exato via Árvore AVL
        std::string tituloFicticio = gerarTituloAleatorio();
        biblioteca.buscarPorTitulo(tituloFicticio);
    } 
    else {
        // Busca Inteligente por Prefixo via Árvore Trie
        static const std::vector<std::string> prefixosBusca = {"O", "A", "Guia", "Int", "Cró"};
        std::uniform_int_distribution<size_t> dist(0, prefixosBusca.size() - 1);
        biblioteca.buscarPorPrefixo(prefixosBusca[dist(gerador)]);
    }
}

void Simulador::cenarioEmprestimosEDevolvidos() {
    // Altera dinamicamente os scores de relevância dos livros para o ranking
    // Nota: Como o simulador opera em lote, ele requisita chaves aleatórias existentes
    // Para simplificar a simulação pura, faremos o tracking via amostragem da hash ou dados inseridos
    std::string isbnAlvo = gerarIsbnAleatorio();
    
    Livro* livro = biblioteca.buscarPorIsbnSemBloom(isbnAlvo);
    if (livro) {
        if (gerador() % 2 == 0) {
            livro->incrementarEmprestimos();
        } else {
            // Simula uma devolução ou uma leitura interna
            livro->incrementarPesquisas();
        }
        // Notifica a biblioteca para atualizar a Fila de Prioridade (Heap) do ranking
        biblioteca.atualizarRankingPreferencia();
    }
}

void Simulador::cenarioDescarteAutomaticoR5(size_t limiteMaximoItens) {
    // R5: Descartar automaticamente livros menos utilizados se estourar o teto físico de memória (R1)
    if (biblioteca.getTotalLivrosCadastrados() > limiteMaximoItens) {
        std::cout << "[SIMULADOR] Limite de armazenamento atingido (" << limiteMaximoItens 
                  << " livros). Disparando mecanismo de descarte R5...\n";
        
        // Remove os k livros menos relevantes do fundo do ranking de empréstimos/pesquisas
        size_t quantidadeParaRemover = biblioteca.getTotalLivrosCadastrados() - limiteMaximoItens + 10;
        biblioteca.expurgarLivrosObsoletos(quantidadeParaRemover);
    }
}

void Simulador::executarSimulacao(size_t numCiclos, size_t limiteMemoriaItens) {
    std::cout << "==================================================\n";
    std::cout << "  INICIANDO MOTOR DE SIMULAÇÃO DINÂMICA DE CARGA  \n";
    std::cout << "==================================================\n";

    auto tempoInicio = std::chrono::high_resolution_clock::now();

    for (size_t ciclo = 1; ciclo <= numCiclos; ++ciclo) {
        // 1. Simula entrada contínua de stock (R25)
        size_t novasInsercoes = 1 + (gerador() % 5);
        for (size_t i = 0; i < novasInsercoes; ++i) {
            cenarioInsercaoContinuaR25();
        }

        // 2. Simula rajadas de pesquisas simultâneas de utilizadores (R9)
        size_t volumeConsultas = 5 + (gerador() % 15);
        for (size_t i = 0; i < volumeConsultas; ++i) {
            cenarioConsultasMassivasR9();
        }

        // 3. Simula flutuação de atividade de empréstimo
        cenarioEmprestimosEDevolvidos();

        // 4. Aplica a política estrita de descarte de memória (R5 / R1)
        cenarioDescarteAutomaticoR5(limiteMemoriaItens);

        // Feedback visual a cada 10% de progresso
        if (ciclo % (numCiclos / 10 + 1) == 0 || ciclo == numCiclos) {
            std::cout << ">> Progresso da Simulação: " << (ciclo * 100) / numCiclos 
                      << "% | Livros em Memória: " << biblioteca.getTotalLivrosCadastrados() << "\n";
        }
    }

    auto tempoFim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = tempoFim - tempoInicio;

    std::cout << "==================================================\n";
    std::cout << "  SIMULAÇÃO CONCLUÍDA COM SUCESSO!\n";
    std::cout << "  Tempo de execução total: " << duracao.count() << " segundos.\n";
    std::cout << "==================================================\n\n";
}