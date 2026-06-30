#include "CSVReader.h"
#include <fstream>
#include <iostream>
#include <sstream>

// Função auxiliar: extrai o ano de uma data.
static int extrairAnoPublicacao(const std::string& data)
{
    if (data.empty())
        return 0;

    size_t pos = data.find_last_of('/');

    try
    {
        if (pos != std::string::npos)
            return std::stoi(data.substr(pos + 1));

        return std::stoi(data);
    }
    catch (...)
    {
        return 0;
    }
}
std::vector<std::string> CSVReader::tokenizarLinha(const std::string& linha) {
    std::vector<std::string> tokens;
    std::string tokenAtual;
    bool dentroDeAspas = false;

    for (size_t i = 0; i < linha.length(); ++i) {
        char c = linha[i];

        if (c == '"') {
            // Alterna o estado ao encontrar aspas duplas (trata vírgulas internas)
            dentroDeAspas = !dentroDeAspas;
        } else if (c == ',' && !dentroDeAspas) {
            // Fim de um campo fora das aspas
            tokens.push_back(tokenAtual);
            tokenAtual.clear();
        } else {
            tokenAtual += c;
        }
    }
    // Adiciona o último token da linha
    tokens.push_back(tokenAtual);
    return tokens;
}

std::vector<Livro*> CSVReader::carregarLivros(const std::string& caminhoArquivo) {
    std::vector<Livro*> acervo;
    std::ifstream arquivo(caminhoArquivo);

    if (!arquivo.is_open()) {
        std::cerr << "[ERRO] Não foi possível abrir o arquivo CSV: " << caminhoArquivo << std::endl;
        return acervo;
    }

    std::string linha;
    // Ignorar a linha de cabeçalho do dataset
    if (!std::getline(arquivo, linha)) {
        return acervo;
    }

    int linhasProcessadas = 0;
    int registrosCorrompidos = 0;



    while (std::getline(arquivo, linha)) {
        if (linha.empty()) continue;

        std::vector<std::string> campos = tokenizarLinha(linha);
        linhasProcessadas++;

        // O dataset deve conter exatamente os 11 campos mapeados
        if (campos.size() < 11) {
            registrosCorrompidos++;
            continue; // Restrição R16: Ignora linhas visivelmente corrompidas ou incompletas
        }

        try {
            // Conversão segura dos tipos primitivos (R16)
            int id = std::stoi(campos[0]);
            std::string titulo = campos[1];
            std::string autor = campos[2];
            double avaliacaoMedia = std::stod(campos[3]);
            std::string isbn = campos[4];
            std::string isbn13 = campos[5];
            std::string idioma = campos[6];
            int numeroPaginas = std::stoi(campos[7]);
            int quantidadeAvaliacoes = std::stoi(campos[8]);
            std::string dataPublicacao = campos[9];
            std::string editora = campos[10];
            
            int anoPublicacao = extrairAnoPublicacao(dataPublicacao);
            
            // Instanciação dinâmica do livro
            Livro* novoLivro = new Livro(id, isbn, isbn13, titulo, autor, editora,
                                         anoPublicacao, idioma, numeroPaginas,
                                         avaliacaoMedia, quantidadeAvaliacoes);
            acervo.push_back(novoLivro);

        } catch (const std::exception& e) {
            // Captura falhas de conversão de string para int/double (Dados corrompidos - R16)
            registrosCorrompidos++;
            continue;
        }
    }

    arquivo.close();
    std::cout << "[INFO] Carga concluída. Livros importados com sucesso: " << acervo.size() 
              << " | Registros descartados/corrompidos: " << registrosCorrompidos << std::endl;

    return acervo;
}