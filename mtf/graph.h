#ifndef GRAPH_H
#define GRAPH_H

#include <set>
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
    std::set<uint> edges;
};

class Graph {
public:
    Graph();
    void loadGraph(std::string file, int mode);

    uint get_vertex_id(long id, std::map<long, uint>& compressMap);
    void insert_edge(uint a, uint b);
    std::map<uint, Node *> nodes;
};

#endif
