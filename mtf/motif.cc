#include "motif.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include  <random>
#include  <cmath>

using namespace std;

void MotifSampleResult::normalizeResult() {
    double sum = 0;
    for(auto const& pair : this->scores) {
        sum += (pair.second / 100); // so that the result is in %
    }
    cout << sum << endl;
    for(auto const& pair : this->scores) {
        this->scores[pair.first] = ((pair.second) / sum);
        cout << "score: " << this->scores[pair.first] << endl;
    }
}

MotifFinder::MotifFinder() {

}

void MotifFinder::parseMotif(string fName) {
    ifstream in(fName);
 
    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        if (line[0] == '#') {
            continue;
        }

        long a,b;
        ss >> a >> b;

        graph.insert_edge(a, b);
    }

    in.close();
}

void findAllPossibleVertices(vector<long>& vertices, vector<long>& possibleVertices, Graph& graph, long vertex) {
    for (long neighbor : graph.nodes[vertex]->edges) {
        if (find(vertices.begin(), vertices.end(), neighbor) == vertices.end()) {
            possibleVertices.push_back(neighbor);
        }
    }
}

double findProbability(int motifSize, Graph& graph, int counted_vertices, vector<long>& vertices, vector<long>& edges, vector<long>& current_vertices) {
    if (current_vertices.size() >= motifSize) {
        return 1.0f;
    }

    double total_probability = 0.0f;

    for (int i = 0; i < motifSize; i++) {
        if ((counted_vertices & (1 << i)) == 0) {
            int edge_count = 0;

            vector<long> remaining_edges;

            for (long& v : edges) {
                if (v == vertices[i]) {
                    edge_count++;
                } else {
                    remaining_edges.push_back(v);
                }
            }

            if (edge_count == 0) {
                // Currently cannot connect
                continue;
            }

            double prob = ((double)edge_count) / edges.size();
            current_vertices.push_back(vertices[i]);

            findAllPossibleVertices(current_vertices, remaining_edges, graph, vertices[i]);

            prob *= findProbability(motifSize, graph, counted_vertices | (1 << i), vertices, remaining_edges, current_vertices);

            total_probability += prob;

            current_vertices.pop_back();
        }
    }

    return total_probability;
}

int categorizeMotif(vector<int>& vertex_degrees, int motifSize) {
    sort(vertex_degrees.begin(), vertex_degrees.end());
    int hash = 0;
    for (int i = 0; i < motifSize; i++) {
        if (hash == 0) {
            hash = vertex_degrees[i];
        } else {
            hash = hash * 10 + vertex_degrees[i];
        }
    }

    int id = -1;
    if (motifSize == 3) {
        if (hash == 112) {
            return 0;
        } else if (hash == 222) {
            return 1;
        }
    } else if (motifSize == 4) {
        switch (hash) {
            case 1112:
                id = 2;
                break;
            case 1113:
                id = 3;
                break;
            case 1223:
                id = 4;
                break;
            case 2222:
                id = 5;
                break;
            case 1122:
                id = 6;
                break;
            case 2233:
                id = 7;
                break;
            case 3333:
                id = 8;
                break;
            default:
                break;
        }
    }

    return id;
}

void MotifSample(int motifSize, int numSample, MotifSampleResult* & result, Graph& graph) {
    result = new MotifSampleResult();
    result->motifSize = motifSize;
    double normalizationFactor = numSample * pow(10, motifSize);

    cout << "Num sample:" << numSample << " " << normalizationFactor << endl;
    for (int _i = 0; _i < numSample; _i++) {
        // Pick a random edge
        int idx = rand() % graph.edges.size();
        Pair edge = graph.edges[idx];

        // Create a set of vertices and possible vertices
        vector<long> vertices;
        vector<long> possibleVertices;

        // Push them in this order so that they would not appear in possible vertices
        vertices.push_back(edge.j);
        findAllPossibleVertices(vertices, possibleVertices, graph, edge.i);

        vertices.push_back(edge.i);
        findAllPossibleVertices(vertices, possibleVertices, graph, edge.j);

        for (int i = 0; i < motifSize - 2; i++) {
            if (possibleVertices.size() == 0) {
                break;
            }

            idx = rand() % possibleVertices.size();
            long vertex = possibleVertices[idx];

            findAllPossibleVertices(vertices, possibleVertices, graph, vertex);
            vertices.push_back(vertex);

            // Remove "vertex"
            possibleVertices.erase(remove(possibleVertices.begin(), possibleVertices.end(), vertex));
        }

        if (vertices.size() != motifSize) {
            continue;
        }

        // Calculate the probability
        double P = 0;
        vector<int> vertex_degrees;
        for (int i = 0; i < motifSize; i++) {
            vertex_degrees.push_back(0);
        }

        for (int i = 0; i < motifSize - 1; i++) {
            for (int j = i + 1; j < motifSize; j++) {
                if (graph.nodes[vertices[i]]->edges.count(vertices[j]) != 0) {
                    // Add 1 degree to vertices i and j
                    vertex_degrees[i] ++;
                    vertex_degrees[j] ++;


                    int counted_vertices = (1 << i) | (1 << j);
                    vector<long> current_vertices;
                    vector<long> current_edges;

                    current_vertices.push_back(vertices[i]);
                    findAllPossibleVertices(current_vertices, current_edges, graph, vertices[j]);
                    current_vertices.push_back(vertices[j]);
                    findAllPossibleVertices(current_vertices, current_edges, graph, vertices[i]);

                    P += findProbability(motifSize, graph, counted_vertices, vertices, current_edges, current_vertices);
                }
            }
        }
        P /= (graph.edges.size() / normalizationFactor);

        if (P == 0) {
            continue;
        }

        // Categorize the motif
        int motifId = categorizeMotif(vertex_degrees, motifSize);
        double W = 1 / P;


        if (result->scores.count(motifId) == 0) {
            result->scores[motifId] = W;
        } else {
            result->scores[motifId] += W;
        }
    }
}


void MotifFinder::sample(int numSample, int numThread) {
    for (int i = 3; i <= 4; i++) {
        MotifSampleResult* result = NULL;
        MotifSample(i, numSample, result, this->graph);
        this->results.push_back(result);
    }

    // Normalize
    for (MotifSampleResult* result : results) {
        result->normalizeResult();
    }
}

void MotifFinder::output(string fName) {
    ofstream out(fName);
    for (MotifSampleResult* result : results) {
        out << "Motif Size:" << result->motifSize << endl;
        for(auto const& pair : result->scores) {
            out << pair.first << " " << pair.second << endl;
        }
    }
    out.close();
}
