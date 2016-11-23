#include "graph.h"

#include <fstream>
#include <sstream>
#include <iostream>


Pair::Pair(uint i, uint j): i(i), j(j) {}

Graph::Graph() {
}

void Graph::insert_edge(uint a, uint b) {
    if (a == b) {
        return;
    }

    if (a > b) {
        insert_edge(b, a);
    }

    // Assert Node should already exist
    Node* na = this->nodes[a];
    Node* nb = this->nodes[b];

    na->edges.push_back(b);
    nb->edges.push_back(a);
}

uint Graph::get_vertex_id(long id, std::map<long, uint>& compressMap) {
    if (compressMap.count(id) == 0) {
        uint vertex_id = nodes.size();
        compressMap[id] = vertex_id;
        nodes.push_back(new Node());
    }
    return compressMap[id];
}

void Graph::loadGraph(std::string file, int mode) {
    std::ifstream in(file);

    std::string line;
    long num_edges = 0;

    std::map<long, uint> compressMap;
    while (getline(in, line)) {
        std::stringstream ss(line);
        if (line[0] == '#') {
            continue;
        }

        long a,b;
        ss >> a >> b;

        uint aid = get_vertex_id(a, compressMap);
        uint bid = get_vertex_id(b, compressMap);

        this->insert_edge(aid, bid);

        num_edges ++;
    }

    std::cout << "Number of vertices: " << nodes.size() << std::endl;
    std::cout << "Number of edges: " << num_edges << std::endl;

    in.close();
}
