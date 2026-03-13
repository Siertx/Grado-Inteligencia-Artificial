#include <iostream>
#include <list>

#include <memory>
#include <vector>

template <typename T>
class RCSTree {
private:
    struct Node {
        T e;
        Node *parent;
        std::shared_ptr<Node> lc;
        std::shared_ptr<Node> rc;
    };
};

template <typename T>
class LinkedTree {
    struct Node {
        T elemento;
        Node* parent;
        std::vector<std::shared_ptr<Node>> children;
    };
    std::shared_ptr<Node> root = nullptr;

public:
    Node* getRoot() {return root.get();}
    bool isRoot(Node* node) {return node == root.get();}
    static bool isLeaf(Node* node){return node->children.empty();}
    static bool isInternal(Node* node){return !isLeaf(node);}
    T getElement(Node* node) {return node->e;}
    Node* add(T e, Node* parent = nullptr) {
        if (parent == nullptr && root.get() == nullptr) {
            std::cout<<"Error, arbol ya tiene raiz";
        }
        auto aux = std::make_shared<Node>(); //creacion del nodo y asignacion de valores
        aux->elemento = e;
        aux->parent = parent;
        if (parent == nullptr) {
            root = aux;
        } else {
            parent->children.push_back(aux);
        }
        return aux.get();
    }

    void remove(Node* node) {
        if (node) { // if node!=nullptr
            if (isRoot(node)) {
                root = nullptr;
            } else {
                node->parent->children.erase(node);
            }
        }
    }

    void attach(Node* node, LinkedTree<T> t) {
        auto r = t.getRoot();
        t.root = nullptr;
        node->parent->children.push_back(r);
    }

    void preorderTree(T tree) {
        preorderNode(tree.getRoot());
    }
    void preorderNode(Node* n) {
        if (n) {
            visit(n); // print, push ....
            for (const auto& c:n->children) {
                preorderNode(c);
            }
        }
    }

    void parentNoGranpa(T tree, std::list<Node*> l) {
        parentNoGranpaNode(tree.getRoot(), l);
    }

    void parentNoGranpaNode(Node* n, std::list<Node*> l) {
        if (n) {
            visit(n, l);
            for (const auto& c : n->children) {
                parentNoGranpaNode(c);
            }
        }
    }
    void visit(Node* n, std::list<Node*> l) {
        if (isParent(n) && !isGranpa(n)) {
            l.push(n);
        }
    }

    void isParent(Node* n) {
        return !n->children.empty();
    }
    bool isGranpa(Node* n) {
        for (const auto& c: n->children) {
            if (isParent(c)) {
                return true;
            }
        }
        return false;
    }

    void tioSinHijo(T tree, std::list<Node*> l) {
        tioSinHijosNode(tree.getRoot(), l);
    }
    void tioSinHijosNode(Node* n, std::list<Node*> l) {
        visitamos(n, l);
        for (const auto& c: n->children) {
            tioSinHijosNode(c, l);
        }
    }
    void visitamos(Node* n, std::list<Node*> l) {
        if (esTio(n) && !tieneHijos(n)) {
            l.push(n);
        }
    }
    bool esTio(Node* n) {
        if (n->parent) {
            for (const auto& c:  n->parent.children) {
                if (c.get() != n && !n->children.empty()) return true;
            }
        }
        return false;
    }
    bool tieneHijos(Node* n) {
        return !n->children.empty();
    }

    class ChildrenIterato {
    public:
        ChildrenIterato(Node* node): node(node){}

        ChildrenIterato& operator++() {
            if (node) node = node->rs.get();
            return (*this);
        }

        auto operator++(int) {
            auto tmp = *this;
            operator++();
            return tmp;
        }

        Node operator*() {
            return *node;
        }

        auto operator!=(const ChildrenIterato& other)const {
            return node != other.node;
        }

    private:
        Node* node;
    };

    class ChildrenIterable {
    public:
        ChildrenIterable(Node* node) {
            if (node) it(node->lc);
            else it(nullptr);
        }

        auto begin() {
            if (node) return ChildrenIterato{node->lc};
            else ChildrenIterato{nullptr};
        }
        auto end(){return nullptr;}

        ChildrenIterable getChildren(Node* node) {
            return ChildrenIterable(node);
        }

    private:
        ChildrenIterato it{nullptr};
        Node* node;
    };


};

int main() {
    LinkedTree<int> t1;

    auto r = t1.add(3);
    auto h1 = t1.add(4, r);
    auto h2 = t1.add(5, r);

    auto n = t1.getRoot();
    t1.isRoot(n);
    LinkedTree<int>::isLeaf(n);
}