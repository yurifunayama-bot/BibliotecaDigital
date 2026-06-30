#include "Trie.h"
#include <iostream>
#include <algorithm>

// ============================================================================
// IMPLEMENTAÇÃO - TRIE PADRÃO
// ============================================================================

TrieNode::TrieNode() : isTerminal(false) {
    for (int i = 0; i < 256; ++i) {
        filhos[i] = nullptr;
    }
}

TrieNode::~TrieNode() {}

Trie::Trie() {
    raiz = new TrieNode();
}

Trie::~Trie() {
    destruir(raiz);
}

void Trie::destruir(TrieNode* no) {
    if (!no) return;
    for (int i = 0; i < 256; ++i) {
        if (no->filhos[i]) {
            destruir(no->filhos[i]);
        }
    }
    delete no;
}

void Trie::inserir(const std::string& termo, Livro* livro) {
    if (termo.empty()) return;
    
    TrieNode* atual = raiz;
    for (char c : termo) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (!atual->filhos[uc]) {
            atual->filhos[uc] = new TrieNode();
        }
        atual = atual->filhos[uc];
    }
    atual->isTerminal = true;
    atual->livros.push_back(livro);
}

std::vector<Livro*> Trie::buscarPorPrefixo(const std::string& prefixo) const {
    TrieNode* atual = raiz;
    for (char c : prefixo) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (!atual->filhos[uc]) {
            return {}; // Prefixo não encontrado
        }
        atual = atual->filhos[uc];
    }

    std::vector<Livro*> resultado;
    coletarTodosLivros(atual, resultado);
    return resultado;
}

void Trie::coletarTodosLivros(TrieNode* no, std::vector<Livro*>& resultado) const {
    if (!no) return;
    if (no->isTerminal) {
        for (Livro* l : no->livros) {
            resultado.push_back(l);
        }
    }
    for (int i = 0; i < 256; ++i) {
        if (no->filhos[i]) {
            coletarTodosLivros(no->filhos[i], resultado);
        }
    }
}

bool Trie::remover(const std::string& termo) {
    if (termo.empty()) return false;

    TrieNode* atual = raiz;
    for (char c : termo) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (!atual->filhos[uc]) return false;
        atual = atual->filhos[uc];
    }

    if (!atual->isTerminal) return false;
    atual->livros.clear();
    atual->isTerminal = false;
    return true;
}

bool Trie::remover(const std::string& termo, Livro* livro) {
    if (termo.empty() || livro == nullptr) return false;

    TrieNode* atual = raiz;
    for (char c : termo) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (!atual->filhos[uc]) return false;
        atual = atual->filhos[uc];
    }

    auto it = std::find(atual->livros.begin(), atual->livros.end(), livro);
    if (it == atual->livros.end()) return false;

    atual->livros.erase(it);
    if (atual->livros.empty()) {
        atual->isTerminal = false;
    }
    return true;
}

// ============================================================================
// IMPLEMENTAÇÃO - TRIE COMPRIMIDA (RADIX TREE)
// ============================================================================

CompressedTrieNode::CompressedTrieNode() : isTerminal(false) {}

CompressedTrieNode::~CompressedTrieNode() {}

CompressedTrie::CompressedTrie() {
    raiz = new CompressedTrieNode();
}

CompressedTrie::~CompressedTrie() {
    destruir(raiz);
}

void CompressedTrie::destruir(CompressedTrieNode* no) {
    if (!no) return;
    for (auto& aresta : no->filhos) {
        destruir(aresta.destino);
    }
    delete no;
}

void CompressedTrie::inserir(const std::string& termo, Livro* livro) {
    if (termo.empty()) return;

    CompressedTrieNode* atual = raiz;
    std::string restante = termo;

    while (!restante.empty()) {
        bool correspondenciaEncontrada = false;

        for (auto& aresta : atual->filhos) {
            size_t i = 0;
            // Encontra o prefixo comum máximo entre o restante do termo e o rótulo da aresta
            while (i < restante.length() && i < aresta.rotulo.length() && restante[i] == aresta.rotulo[i]) {
                i++;
            }

            if (i > 0) { // Existe correspondência parcial ou total
                correspondenciaEncontrada = true;
                std::string comum = restante.substr(0, i);
                std::string restoRotulo = aresta.rotulo.substr(i);
                std::string restoRestante = restante.substr(i);

                if (restoRotulo.empty()) {
                    // O rótulo da aresta foi totalmente consumido. Desce para o nó destino.
                    atual = aresta.destino;
                    restante = restoRestante;
                    break;
                } else {
                    // DIVISÃO DE ARESTA (SPLIT): Ocorre quando há divergência no meio do rótulo
                    CompressedTrieNode* noIntermediario = new CompressedTrieNode();
                    CompressedTrieNode* noAntigoDestino = aresta.destino;

                    // Ajusta a aresta original para apontar ao intermediário com o prefixo comum
                    aresta.rotulo = comum;
                    aresta.destino = noIntermediario;

                    // O nó intermediário adota o destino antigo com o restante do rótulo original
                    noIntermediario->filhos.push_back({restoRotulo, noAntigoDestino});

                    if (restoRestante.empty()) {
                        // O termo inserido termina exatamente no nó intermediário criado
                        noIntermediario->isTerminal = true;
                        noIntermediario->livros.push_back(livro);
                    } else {
                        // Cria uma nova folha para abrigar o restante do novo termo
                        CompressedTrieNode* noFolha = new CompressedTrieNode();
                        noFolha->isTerminal = true;
                        noFolha->livros.push_back(livro);
                        noIntermediario->filhos.push_back({restoRestante, noFolha});
                    }
                    return;
                }
            }
        }

        if (!correspondenciaEncontrada) {
            // Sem qualquer interseção de prefixo: cria ramificação direta a partir do nó atual
            CompressedTrieNode* noFolha = new CompressedTrieNode();
            noFolha->isTerminal = true;
            noFolha->livros.push_back(livro);
            atual->filhos.push_back({restante, noFolha});
            return;
        }
    }

    // Caso o laço termine perfeitamente em cima de um nó pré-existente
    atual->isTerminal = true;
    atual->livros.push_back(livro);
}

std::vector<Livro*> CompressedTrie::buscarPorPrefixo(const std::string& prefixo) const {
    if (prefixo.empty()) return {};

    CompressedTrieNode* atual = raiz;
    std::string restante = prefixo;

    while (!restante.empty()) {
        bool correspondenciaEncontrada = false;

        for (const auto& aresta : atual->filhos) {
            size_t i = 0;
            while (i < restante.length() && i < aresta.rotulo.length() && restante[i] == aresta.rotulo[i]) {
                i++;
            }

            if (i > 0) {
                correspondenciaEncontrada = true;
                if (i == aresta.rotulo.length()) {
                    // Consumiu toda a aresta, avança na árvore profunda
                    atual = aresta.destino;
                    restante = restante.substr(i);
                    break;
                } else if (i == restante.length()) {
                    // O prefixo de busca termina NO MEIO do rótulo desta aresta.
                    // Isso significa que todas as subárvores deste destino são válidas!
                    std::vector<Livro*> resultado;
                    coletarTodosLivros(aresta.destino, resultado);
                    return resultado;
                } else {
                    // Divergência parcial. Não há correspondência.
                    return {};
                }
            }
        }

        if (!correspondenciaEncontrada) return {};
    }

    std::vector<Livro*> resultado;
    coletarTodosLivros(atual, resultado);
    return resultado;
}

void CompressedTrie::coletarTodosLivros(CompressedTrieNode* no, std::vector<Livro*>& resultado) const {
    if (!no) return;
    if (no->isTerminal) {
        for (Livro* l : no->livros) {
            resultado.push_back(l);
        }
    }
    for (const auto& aresta : no->filhos) {
        coletarTodosLivros(aresta.destino, resultado);
    }
}

bool CompressedTrie::remover(const std::string& termo) {
    if (termo.empty()) return false;

    CompressedTrieNode* atual = raiz;
    std::string restante = termo;

    while (!restante.empty()) {
        bool encontrou = false;
        for (auto& aresta : atual->filhos) {
            if (restante.rfind(aresta.rotulo, 0) == 0) {
                atual = aresta.destino;
                restante = restante.substr(aresta.rotulo.size());
                encontrou = true;
                break;
            }
        }
        if (!encontrou) return false;
    }

    if (!atual->isTerminal) return false;
    atual->livros.clear();
    atual->isTerminal = false;
    return true;
}

bool CompressedTrie::remover(const std::string& termo, Livro* livro) {
    if (termo.empty() || livro == nullptr) return false;

    CompressedTrieNode* atual = raiz;
    std::string restante = termo;

    while (!restante.empty()) {
        bool encontrou = false;
        for (auto& aresta : atual->filhos) {
            if (restante.rfind(aresta.rotulo, 0) == 0) {
                atual = aresta.destino;
                restante = restante.substr(aresta.rotulo.size());
                encontrou = true;
                break;
            }
        }
        if (!encontrou) return false;
    }

    auto it = std::find(atual->livros.begin(), atual->livros.end(), livro);
    if (it == atual->livros.end()) return false;

    atual->livros.erase(it);
    if (atual->livros.empty()) {
        atual->isTerminal = false;
    }
    return true;
}