#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <vector>
#include <string>

typedef unsigned int uint;

struct Pair {
    uint i;
    uint j;
    Pair(uint i, uint j);
};

struct Node {
    std::vector<uint> edges;
};

class Graph {
public:
    Graph();
    void loadGraph(std::string file, bool isDirected);

    uint get_vertex_id(long id, std::map<long, uint>& compressMap);
    void insert_edge(uint a, uint b, bool isDirected);
    std::vector<Node *> nodes;
};

#endif
