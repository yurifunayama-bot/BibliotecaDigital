#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "Biblioteca.h"
#include "CSVReader.h"
#include "Simulador.h"
#include "Benchmark.h"

// Função auxiliar para limpar o buffer do std::cin e evitar loops infinitos em inputs inválidos
void limparBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Exibe o menu interativo estipulado nos requisitos de interface (Páginas 10 e 11)
void exibirMenu() {
    std::cout << "\n==================================================\n";
    std::cout << "    SISTEMA DE BIBLIOTECA DIGITAL INTELIGENTE     \n";
    std::cout << "==================================================\n";
    std::cout << "1  Inserir livro\n";
    std::cout << "2  Remover livro\n";
    std::cout << "3  Buscar ISBN\n";
    std::cout << "4  Buscar titulo\n"; 
    std::cout << "5  Buscar prefixo\n";
    std::cout << "6  Estatisticas\n";
    std::cout << "7  Simulacao\n";
    std::cout << "8  Benchmark\n";
    std::cout << "9  Exportar resultados\n";
    std::cout << "0  Sair\n";
    std::cout << "==================================================\n";
    std::cout << "Escolha uma opcao: ";
}

int main() {
    Biblioteca biblioteca;
    
    std::cout << "[SISTEMA] Inicializando base de dados...\n";
    std::cout << "[SISTEMA] Tentando carregar o arquivo 'books.csv' (Minimo 10.000 livros)...\n";
    
    // R4: Chamada do leitor de arquivos CSV customizado
   std::vector<Livro*> livrosCarregados = CSVReader::carregarLivros("books.csv");
    if (livrosCarregados.empty()) {
    livrosCarregados = CSVReader::carregarLivros("data/books.csv");
    }

    size_t livrosRejeitados = 0;

    for (Livro* livro : livrosCarregados) {
    if (!biblioteca.inserirLivro(livro)) {
        delete livro;
        ++livrosRejeitados;
    }
    }

    if (!livrosCarregados.empty()) {
        std::cout << "[SUCESSO] Carga inicial concluida. Livros em memoria: " 
                << biblioteca.getTotalLivrosCadastrados() << "\n";

    if (livrosRejeitados > 0) {
        std::cout << "[AVISO] Livros rejeitados na carga inicial: "
                  << livrosRejeitados << "\n";
    }
    } else {
        std::cout << "[AVISO] Nao foi possivel carregar 'books.csv'. O sistema iniciara vazio.\n";
    }

    int opcao = -1;
    while (opcao != 0) {
        exibirMenu();
        if (!(std::cin >> opcao)) {
            std::cout << "[ERRO] Entrada invalida! Digite um nimero.\n";
            limparBuffer();
            continue;
        }
        limparBuffer(); // Limpa o caractere de nova linha residual

        switch (opcao) {
            case 1: { // Inserir Livro
                std::string isbn, titulo, autor, editora;
                int ano;
                
                std::cout << "Digite o ISBN (10 ou 13 digitos): ";
                std::getline(std::cin, isbn);
                std::cout << "Digite o Titulo: ";
                std::getline(std::cin, titulo);
                std::cout << "Digite o Autor: ";
                std::getline(std::cin, autor);
                std::cout << "Digite a Editora: ";
                std::getline(std::cin, editora);
                std::cout << "Digite o Ano de Publicacao: ";
                while (!(std::cin >> ano)) {
                    std::cout << "Ano invalida. Digite novamente: ";
                    limparBuffer();
                }
                limparBuffer();
                
                // Criação controlada da entidade na memória dinâmica
                auto* novoLivro = new Livro(isbn, titulo, autor, ano);
                novoLivro->setEditora(editora);
                if (biblioteca.inserirLivro(novoLivro)) {
                std::cout << "[SUCESSO] Livro indexado com sucesso em todas as estruturas!\n";
                }
                else {
                    delete novoLivro;   // Evita vazamento de memória
                std::cout << "[ERRO] ISBN invalido ou ja cadastrado.\n\n";
                }
                break;
            }
            case 2: { // Remover Livro
                std::string isbn;
                std::cout << "Digite o ISBN do livro a remover: ";
                std::getline(std::cin, isbn);
                
                if (biblioteca.removerLivro(isbn)) {
                    std::cout << "[SUCESSO] Livro removido do acervo.\n";
                } else {
                    std::cout << "[ERRO] Livro nao encontrado para remocao.\n";
                }
                break;
            }
            case 3: { // Buscar por ISBN (Passa por Bloom + Hash Table)
                std::string isbn;
                std::cout << "Digite o ISBN para busca: ";
                std::getline(std::cin, isbn);
                
                Livro* resultado = biblioteca.buscarPorIsbn(isbn);
                if (resultado) {
                    std::cout << "\n[LIVRO ENCONTRADO (Hash Table)]\n";
                    std::cout << "Titulo: " << resultado->getTitulo() << "\n";
                    std::cout << "Autor:  " << resultado->getAutor() << "\n";
                    std::cout << "Ano:    " << resultado->getAnoPublicacao() << "\n";
                } else {
                    std::cout << "[AVISO] Livro nao existe no acervo (Bloqueado na portaria ou inexistente).\n";
                }
                break;
            }
            case 4: { // Buscar por Título Exato (Árvore AVL)
                std::string titulo;
                std::cout << "Digite o Titulo exato para busca: ";
                std::getline(std::cin, titulo);
                
                Livro* resultado = biblioteca.buscarPorTitulo(titulo);
                if (resultado) {
                    std::cout << "\n[LIVRO ENCONTRADO (arvore AVL)]\n";
                    std::cout << "ISBN:   " << resultado->getIsbn() << "\n";
                    std::cout << "Autor:  " << resultado->getAutor() << "\n";
                } else {
                    std::cout << "[AVISO] Titulo nao localizado na arvore AVL.\n";
                }
                break;
            }
            case 5: { // Buscar por Prefixo Textual (Trie / Radix Tree)
                std::string prefixo;
                std::cout << "Digite o prefixo de busca (ex: prog): ";
                std::getline(std::cin, prefixo);
                
                std::vector<Livro*> resultados = biblioteca.buscarPorPrefixo(prefixo);
                if (!resultados.empty()) {
                    std::cout << "\n[RESULTADOS ENCONTRADOS VIA TRIE (" << resultados.size() << ")]\n";
                    // Limita a exibição às primeiras 10 correspondências para não sobrecarregar o terminal
                    size_t limite = std::min(resultados.size(), size_t(10));
                    for (size_t i = 0; i < limite; ++i) {
                        std::cout << " - " << resultados[i]->getTitulo() << " (por " << resultados[i]->getAutor() << ")\n";
                    }
                    if (resultados.size() > 10) std::cout << " ... e mais " << (resultados.size() - 10) << " livros.\n";
                } else {
                    std::cout << "[AVISO] Nenhum titulo inicia com o prefixo informado.\n";
                }
                break;
            }
            case 6: { // Estatísticas do Acervo
                std::cout << "\n==================================================\n";
                std::cout << "         METRICAS E ESTATISTICAS DO ACERVO       \n";
                std::cout << "==================================================\n";
                biblioteca.exibirEstatisticasGlobais();
                break;
            }
            case 7: { // Simulador Automático de Carga / Stress-Test
                Simulador simulador(biblioteca);
                size_t ciclos = 500;
                size_t limiteFisico = 105000; // Gatilho de corte de memória (R5/R1)
                
                std::cout << "[SIMULADOR] Executando " << ciclos << " ciclos operacionais continuos...\n";
                simulador.executarSimulacao(ciclos, limiteFisico);
                break;
            }
            case 8: { // Executar Subsistema de Benchmark
                Benchmark benchmark(biblioteca);
                std::cout << "[BENCHMARK] Recolhendo amostras e testando as 5 estruturas estruturais...\n";
                benchmark.executarTodosOsBenchmarks(biblioteca.getAmostraLivros());
                break;
            }
            case 9: { // Exportar resultados CSV explicitamente
                Benchmark benchmark(biblioteca);
                std::cout << "[EXPORTAR] Forcando gravacao e escrita dos logs em disco...\n";
                benchmark.executarTodosOsBenchmarks(biblioteca.getAmostraLivros());
                std::cout << "[SUCESSO] Todos os arquivos .csv estao prontos no diretorio de execucao.\n";
                break;
            }
            case 0:
                std::cout << "[SISTEMA] Encerrando atividades. Libertando todas as estruturas de dados...\n";
                break;
            default:
                std::cout << "[ERRO] Opcao invalida! Tente novamente.\n";
                break;
        }
    }

    return 0;
}
