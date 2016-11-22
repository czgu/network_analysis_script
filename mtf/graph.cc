#include "graph.h"

Pair::Pair(long i, long j): i(i), j(j) {}

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
        nodes[a] = new Node();
    }

    if (nodes.count(b) == 0) {
        nodes[b] = new Node();
    }

    Node* na = this->nodes[a];
    Node* nb = this->nodes[b];

    na->edges.insert(b);
    nb->edges.insert(a);

    this->edges.push_back(Pair(a, b));
}
