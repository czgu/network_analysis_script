#include "motif.h"
#include "timer.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <cmath>
#include <thread>

using namespace std;

void MotifSampleResult::sum(const MotifSampleResult& r) {
    if (r.motifSize != this->motifSize) {
        // TODO: Assert this
        return;
    }

    for (auto const& pair : r.scores) {
        if (this->scores.count(pair.first) == 0) {
            this->scores[pair.first] = pair.second;
        } else {
            this->scores[pair.first] += pair.second;
        }
    }
}

void MotifSampleResult::normalizeResult() {
    double sum = 0;
    for(auto const& pair : this->scores) {
        sum += (pair.second / 100); // so that the result is in %
    }
    cout << "sum: " << sum << endl;
    for(auto const& pair : this->scores) {
        this->scores[pair.first] = ((pair.second) / sum);
        cout << pair.first << " score: " << this->scores[pair.first] << endl;
    }
}

MotifFinder::MotifFinder() {

}

void MotifFinder::load(string fName, int mode) {
    this->graph.loadGraph(fName, mode);
}

void findAllPossibleVertices(vector<long>& vertices, vector<long>& possibleVertices, Graph& graph, long vertex) {
    for (long neighbor : graph.nodes[vertex]->edges) {
        if (find(vertices.begin(), vertices.end(), neighbor) == vertices.end()) {
            possibleVertices.push_back(neighbor);
        }
    }
}

double findProbability(unsigned int motifSize, Graph& graph, int counted_vertices, vector<long>& vertices, vector<long>& edges, vector<long>& current_vertices) {
    if (current_vertices.size() >= motifSize) {
        return 1.0f;
    }

    double total_probability = 0.0f;

    for (unsigned int i = 0; i < motifSize; i++) {
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

int categorizeMotif(vector<int>& vertex_degrees, unsigned int motifSize) {
    sort(vertex_degrees.begin(), vertex_degrees.end());
    int hash = 0;
    for (unsigned int i = 0; i < motifSize; i++) {
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
            case 1113:
                id = 2;
                break;
            case 1223:
                id = 3;
                break;
            case 2222:
                id = 4;
                break;
            case 1122:
                id = 5;
                break;
            case 2233:
                id = 6;
                break;
            case 3333:
                id = 7;
                break;
            default:
                break;
        }
    }

    if (id == -1) {
        cout << "Motif: " << motifSize << ", Hash:" << hash << endl;
    }

    return id;
}

void MotifSample(unsigned int motifSize, int numSample, MotifSampleResult** result, Graph* graph_ptr) {
    MotifSampleResult* localSample = new MotifSampleResult();
    Graph& graph = *graph_ptr;


    localSample->motifSize = motifSize;
    double normalizationFactor = numSample * pow(10, motifSize);

    // cout << "Num sample:" << numSample << " " << normalizationFactor << endl;
    for (int _i = 0; _i < numSample; _i++) {
        // Pick a random node
        uint v1 = rand() % graph.nodes.size();
        uint v1degree = graph.nodes[v1]->edges.size();

        // If we have a bad pick for vertices, restart the process
        if (v1degree == 0) {
            _i --;
            continue;
        }
        // From that node, pick a random neighbor
        long v2 = graph.nodes[v1]->edges[rand() % v1degree];

        double edgeChance = (1.0f / v1degree);
        Pair edge(v1, v2);

        // Create a set of vertices and possible vertices
        vector<long> vertices;
        vector<long> possibleVertices;

        // Push them in this order so that they would not appear in possible vertices
        vertices.push_back(edge.j);
        findAllPossibleVertices(vertices, possibleVertices, graph, edge.i);

        vertices.push_back(edge.i);
        findAllPossibleVertices(vertices, possibleVertices, graph, edge.j);

        for (unsigned int i = 0; i < motifSize - 2; i++) {
            if (possibleVertices.size() == 0) {
                break;
            }

            unsigned int idx = rand() % possibleVertices.size();
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
        for (unsigned int i = 0; i < motifSize; i++) {
            vertex_degrees.push_back(0);
        }

        for (unsigned int i = 0; i < motifSize - 1; i++) {
            for (unsigned int j = i + 1; j < motifSize; j++) {
                auto& vi_edges = graph.nodes[vertices[i]]->edges;
                if (find(vi_edges.begin(), vi_edges.end(), vertices[j]) != vi_edges.end()) {
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

        // To prevent P to be too small, which makes 1/P smaller
        P *= (normalizationFactor * edgeChance);

        if (P == 0) {
            continue;
        }

        // Categorize the motif
        int motifId = categorizeMotif(vertex_degrees, motifSize);
        double W = 1 / P;


        if (localSample->scores.count(motifId) == 0) {
            localSample->scores[motifId] = W;
        } else {
            localSample->scores[motifId] += W;
        }
    }
    *result = localSample;
}


void MotifFinder::sample(int numSample, int numThread) {
    int samplePerThread = numSample / numThread;
    for (int i = 3; i <= 4; i++) {
        MotifSampleResult** results = new MotifSampleResult*[numThread];
        std::thread* threads = new std::thread[numThread];

        for (int j = 0; j < numThread; j++) {
            threads[j] = std::thread(MotifSample, i, samplePerThread, results + j, &this->graph);
            // MotifSample( i, samplePerThread, results + j, &this->graph);
        }

        // Map
        for (int j = 0; j < numThread; j++) {
            threads[j].join();
        }
        // Reduce
        MotifSampleResult* total_result = new MotifSampleResult();
        total_result->motifSize = i;
        for (int j = 0; j < numThread; j++) {
            total_result->sum(*results[j]);
        }


        this->results.push_back(total_result);
        // Clean up
        for (int j = 0; j < numThread; j++) {
            delete results[j];
            results[j] = NULL;
        }
        delete [] threads;
        delete [] results;

        threads = NULL;
        results = NULL;
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

void MotifFinder::run(string inFile, string outFile, int numThread, int numSample) {
    Timer timer;
    this->load(inFile, 0);
    cout << "Parsed. Time took: " << timer.updateAnchorTime() << endl;
    this->sample(numSample, numThread);
    cout << "Sampling completed. Time took: " << timer.updateAnchorTime() << endl;
    this->output(outFile);
    cout << "Output completed. Time took: " << timer.updateAnchorTime() << endl;
}

