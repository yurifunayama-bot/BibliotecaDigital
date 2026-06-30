#include "AVL.h"
#include <algorithm>

AVL::AVL() : raiz(nullptr) {}

AVL::~AVL() {
    destruirArvore(raiz);
}

void AVL::destruirArvore(AVLNode* no) {
    if (no) {
        destruirArvore(no->esquerdo);
        destruirArvore(no->direito);
        // Deleta apenas o nó estrutural da árvore. Os ponteiros contidos em no->livros 
        // são gerenciados centralizadamente pela classe Biblioteca.
        delete no;
    }
}

int AVL::getAltura(AVLNode* no) const {
    return no ? no->altura : 0;
}

int AVL::getFatorBalanceamento(AVLNode* no) const {
    if (!no) return 0;
    return getAltura(no->esquerdo) - getAltura(no->direito);
}

AVLNode* AVL::rotacaoDireita(AVLNode* y) {
    AVLNode* x = y->esquerdo;
    AVLNode* T2 = x->direito;

    // Executa a rotação física dos ponteiros
    x->direito = y;
    y->esquerdo = T2;

    // Atualiza as alturas dos nós modificados (a ordem importa: base para o topo)
    y->altura = std::max(getAltura(y->esquerdo), getAltura(y->direito)) + 1;
    x->altura = std::max(getAltura(x->esquerdo), getAltura(x->direito)) + 1;

    return x; // Nova raiz da subárvore
}

AVLNode* AVL::rotacaoEsquerda(AVLNode* x) {
    AVLNode* y = x->direito;
    AVLNode* T2 = y->esquerdo;

    // Executa a rotação física dos ponteiros
    y->esquerdo = x;
    x->direito = T2;

    // Atualiza as alturas
    x->altura = std::max(getAltura(x->esquerdo), getAltura(x->direito)) + 1;
    y->altura = std::max(getAltura(y->esquerdo), getAltura(y->direito)) + 1;

    return y; // Nova raiz da subárvore
}

void AVL::inserir(const std::string& titulo, Livro* livro) {
    raiz = inserir(raiz, titulo, livro);
}

AVLNode* AVL::inserir(AVLNode* no, const std::string& titulo, Livro* livro) {
    // 1. Inserção BST Clássica
    if (!no) {
        return new AVLNode(titulo, livro);
    }

    if (titulo < no->titulo) {
        no->esquerdo = inserir(no->esquerdo, titulo, livro);
    } else if (titulo > no->titulo) {
        no->direito = inserir(no->direito, titulo, livro);
    } else {
        // Título já existe indexado: evita duplicação de nós na árvore e insere no vetor
        no->livros.push_back(livro);
        return no;
    }

    // 2. Atualização da altura do nó ancestral
    no->altura = 1 + std::max(getAltura(no->esquerdo), getAltura(no->direito));

    // 3. Checagem do Fator de Balanceamento para detectar desequilíbrio
    int balanceamento = getFatorBalanceamento(no);

    // Caso Esquerda-Esquerda (Sinal positivo puro)
    if (balanceamento > 1 && titulo < no->esquerdo->titulo) {
        return rotacaoDireita(no);
    }

    // Caso Direita-Direita (Sinal negativo puro)
    if (balanceamento < -1 && titulo > no->direito->titulo) {
        return  rotacaoEsquerda(no);
    }

    // Caso Esquerda-Direita
    if (balanceamento > 1 && titulo > no->esquerdo->titulo) {
        no->esquerdo =  rotacaoEsquerda(no->esquerdo);
        return  rotacaoDireita(no);
    }

    // Caso Direita-Esquerda
    if (balanceamento < -1 && titulo < no->direito->titulo) {
        no->direito =  rotacaoDireita(no->direito);
        return  rotacaoEsquerda(no);
    }

    return no;
}

AVLNode* AVL::noComValorMinimo(AVLNode* no) const {
    AVLNode* atual = no;
    while (atual->esquerdo != nullptr) {
        atual = atual->esquerdo;
    }
    return atual;
}

AVLNode* AVL::removerNoFisico(AVLNode* no, const std::string& titulo) {
    if (!no) return no;

    if (titulo < no->titulo) {
        no->esquerdo = removerNoFisico(no->esquerdo, titulo);
    } else if (titulo > no->titulo) {
        no->direito = removerNoFisico(no->direito, titulo);
    } else {
        // Encontrou o nó a ser deletado fisicamente da árvore
        if (!no->esquerdo || !no->direito) {
            AVLNode* temp = no->esquerdo ? no->esquerdo : no->direito;
            if (!temp) {
                // Caso sem filhos
                temp = no;
                no = nullptr;
                delete temp;
            } else {
                // Caso com um filho: substitui o nó atual pelo filho válido
                AVLNode* antigoNo = no;
                no = temp;
                delete antigoNo;
            }
        } else {
            // Caso com dois filhos: obtém o sucessor em-ordem (mínimo da subárvore direita)
            AVLNode* temp = noComValorMinimo(no->direito);
            no->titulo = temp->titulo;
            no->livros = temp->livros; // Copia a lista de referências
            no->direito = removerNoFisico(no->direito, temp->titulo); // Deleta o sucessor
        }
    }

    if (!no) return no;

    // Recalcula altura e rebalanceia o nó após a deleção física
    no->altura = 1 + std::max(getAltura(no->esquerdo), getAltura(no->direito));
    int balanceamento = getFatorBalanceamento(no);

    if (balanceamento > 1 && getFatorBalanceamento(no->esquerdo) >= 0)
        return  rotacaoDireita(no);

    if (balanceamento > 1 && getFatorBalanceamento(no->esquerdo) < 0) {
        no->esquerdo =  rotacaoEsquerda(no->esquerdo);
        return  rotacaoDireita(no);
    }

    if (balanceamento < -1 && getFatorBalanceamento(no->direito) <= 0)
        return  rotacaoEsquerda(no);

    if (balanceamento < -1 && getFatorBalanceamento(no->direito) > 0) {
        no->direito =  rotacaoDireita(no->direito);
        return  rotacaoEsquerda(no);
    }

    return no;
}

bool AVL::remover(const std::string& titulo, Livro* livro) {
    bool removido = false;
    raiz = removerLivroDaArvore(raiz, titulo, livro, removido);
    return removido;
}

AVLNode* AVL::removerLivroDaArvore(AVLNode* no, const std::string& titulo, Livro* livro, bool& removido) {
    if (!no) return no;

    if (titulo < no->titulo) {
        no->esquerdo = removerLivroDaArvore(no->esquerdo, titulo, livro, removido);
    } else if (titulo > no->titulo) {
        no->direito = removerLivroDaArvore(no->direito, titulo, livro, removido);
    } else {
        // Encontrou o nó do título. Varre para remover o livro específico de dentro do vetor
        auto it = no->livros.end();
        for (auto i = no->livros.begin(); i != no->livros.end(); ++i) {
            if (*i == livro) {
                it = i;
                break;
            }
        }
        if (it != no->livros.end()) {
            no->livros.erase(it);
            removido = true;
        }

        // Se o vetor de duplicatas esvaziou completamente, o nó estrutural perde o sentido e deve ser removido
        if (no->livros.empty()) {
            return removerNoFisico(no, titulo);
        }
        return no; // Se ainda restarem livros com esse título, o nó permanece intacto e equilibrado
    }

    // Rebalanceamento propagado pelas chamadas recursivas que alteraram subárvores abaixo
    no->altura = 1 + std::max(getAltura(no->esquerdo), getAltura(no->direito));
    int balanceamento = getFatorBalanceamento(no);

    if (balanceamento > 1 && getFatorBalanceamento(no->esquerdo) >= 0)
        return  rotacaoDireita(no);

    if (balanceamento > 1 && getFatorBalanceamento(no->esquerdo) < 0) {
        no->esquerdo =  rotacaoEsquerda(no->esquerdo);
        return  rotacaoDireita(no);
    }

    if (balanceamento < -1 && getFatorBalanceamento(no->direito) <= 0)
        return  rotacaoEsquerda(no);

    if (balanceamento < -1 && getFatorBalanceamento(no->direito) > 0) {
        no->direito =  rotacaoDireita(no->direito);
        return  rotacaoEsquerda(no);
    }

    return no;
}

std::vector<Livro*> AVL::buscar(const std::string& titulo) const {
    AVLNode* atual = raiz;
    while (atual) {
        if (titulo < atual->titulo) {
            atual = atual->esquerdo;
        } else if (titulo > atual->titulo) {
            atual = atual->direito;
        } else {
            return atual->livros;
        }
    }
    return {}; // Retorna um vetor vazio caso o título não exista no acervo
}

AVLNode* AVL::getRaiz() const {
    return raiz;
}

bool AVL::remover(const std::string& titulo){
    auto livros = buscar(titulo);

    if (livros.empty())
        return false;

    // Remove o primeiro livro encontrado
    return remover(titulo, livros.front());
}

int AVL::getAltura() const{
    return getAltura(raiz);
}

void AVL::procurarMaior( AVLNode* no, Livro*& melhor) const {
    if (no == nullptr)
        return;

    procurarMaior(no->esquerdo, melhor);

    for (Livro* livro : no->livros)
    {
        if (melhor == nullptr ||
            livro->getTotalAcessos() >
            melhor->getTotalAcessos())
        {
            melhor = livro;
        }
    }

    procurarMaior(no->direito, melhor);
}

Livro* AVL::encontrarMaiorScoreEmprestimo() const {
    Livro* melhor = nullptr;

    procurarMaior(raiz, melhor);

    return melhor;
}