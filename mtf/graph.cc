#include "graph.h"

Pair::Pair(long i, long j): i(i), j(j) {}

Node::Node(long id) : id(id) {
}

Graph::Graph() {
}

void Graph::insert_edge(long a, long b) {
    if (a == b) {
        return;
    }
    
    if (a > b) {
        insert_edge(b, a);
    }

    if (nodes.count(a) == 0) {
        nodes[a] = new Node(a);
    }

    if (nodes.count(b) == 0) {
        nodes[b] = new Node(b);
    }

    Node* na = this->nodes[a];
    Node* nb = this->nodes[b];

    na->edges.insert(b);
    nb->edges.insert(a);

    this->edges.push_back(Pair(a, b));
}
