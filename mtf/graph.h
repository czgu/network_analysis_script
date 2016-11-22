#ifndef GRAPH_H
#define GRAPH_H

#include <set>
#include <map>
#include <vector>

struct Pair {
    long i;
    long j;
    Pair(long i, long j);
};

struct Node {
    std::set<long> edges;
};

class Graph {
public:
    Graph();
    void insert_edge(long a, long b);
    std::map<long, Node *> nodes;
    std::vector<Pair> edges;
};

#endif
