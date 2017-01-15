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
    // Calculate the sum of the scores
    for(auto const& pair : this->scores) {
        sum += pair.second;
    }
    // cout << "sum: " << sum << endl;
    for(auto const& pair : this->scores) {
        // so that the output score is percentages
        this->scores[pair.first] = ((pair.second) / sum) * 100;         

        // cout << pair.first << " score: " << this->scores[pair.first] << endl;
    }
}

MotifFinder::MotifFinder() {

}

void MotifFinder::load(string fName, bool isDirected) {
    this->graph.loadGraph(fName, isDirected);
}

void findAllPossibleVertices(vector<long>& vertices, vector<long>& possibleVertices, Graph& graph, long vertex) {
    for (long neighbor : graph.nodes[vertex]->edges) {
        if (find(vertices.begin(), vertices.end(), neighbor) == vertices.end()) {
            possibleVertices.push_back(neighbor);
        }
    }
}

double findProbability(unsigned int motifSize, Graph& graph, int counted_vertices, vector<long>& vertices, vector<long>& edges, vector<long>& current_vertices) {
    // We fill current_vertices until it has all vertices, while recursively calculate the probability of each possible edge selection
    if (current_vertices.size() >= motifSize) { // completed search
        return 1.0f;
    }

    double total_probability = 0.0f;

    // iterate through all ways to pick a vertex
    for (unsigned int i = 0; i < motifSize; i++) {
        // We check using counted_vertices as a bitmap to see
        // if the ith vertex of the subgraph is already included in current_vertices
        if ((counted_vertices & (1 << i)) == 0) {
            // stores how many edges can connect us from current_vertices subgraph
            // to the ith vertex
            int edge_count = 0;

            // new list of edges after we remove the ones that points to ith vertex
            vector<long> remaining_edges;

            for (long& v : edges) {
                if (v == vertices[i]) {
                    edge_count++;
                } else {
                    remaining_edges.push_back(v);
                }
            }

            // Currently cannot connect to the ith vertex
            if (edge_count == 0) {
                continue;
            }

            // the probability of adding the vertex to the graph
            double prob = ((double)edge_count) / edges.size();
            current_vertices.push_back(vertices[i]);

            // add all the neighbor of ith vertex to remaining edges
            findAllPossibleVertices(current_vertices, remaining_edges, graph, vertices[i]);

            // now recursively calculate the probability with ith vertex added
            prob *= findProbability(motifSize, graph, counted_vertices | (1 << i), vertices, remaining_edges, current_vertices);

            total_probability += prob;

            current_vertices.pop_back();
        }
    }

    return total_probability;
}

int categorizeMotif(vector<int>& vertex_degrees, unsigned int motifSize) {
    // order the degree of each vertex in increasing order
    sort(vertex_degrees.begin(), vertex_degrees.end());
    // now convert a list of degrees to a number
    // for example [1, 2, 2, 3] => 1223
    int hash = 0;
    for (unsigned int i = 0; i < motifSize; i++) {
        if (hash == 0) {
            hash = vertex_degrees[i];
        } else {
            hash = hash * 10 + vertex_degrees[i];
        }
    }

    // Now assign a id to each number, the id represent the unique motif graph
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

    // this is used so that the sum of probability does not exceed floating point limit
    double normalizationFactor = numSample * pow(10, motifSize);

    // cout << "Num sample:" << numSample << " " << normalizationFactor << endl;
    for (int _i = 0; _i < numSample; _i++) {
        // Pick a random vertex
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

        // Calculate the probability of the subgraph being selected
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

                    // add vertex i, j to the subgraph, and all their neighbors to the edge list
                    current_vertices.push_back(vertices[i]);
                    findAllPossibleVertices(current_vertices, current_edges, graph, vertices[j]);
                    current_vertices.push_back(vertices[j]);
                    findAllPossibleVertices(current_vertices, current_edges, graph, vertices[i]);

                    // The probability of the subgraph being selected is calculated here,
                    // we start with 2 randomly selected vertex of the subgraph and a list of their neighbors
                    // and we try to calculate the probability of generating the subgraph by choosing their neighbor vertices
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


void MotifFinder::sample(MotifSetting& setting) {
    int numSample = setting.numSample;
    int numThread = setting.numThread;

    int samplePerThread = numSample / numThread;
    for (int i = setting.motifSizeMin; i <= setting.motifSizeMax; i++) {
        MotifSampleResult** results = new MotifSampleResult*[numThread];
        std::thread* threads = new std::thread[numThread];

        // Sample on each thread
        for (int j = 0; j < numThread; j++) {
            threads[j] = std::thread(MotifSample, i, samplePerThread, results + j, &this->graph);
        }

        // Map (Run all subprocess of sampling)
        for (int j = 0; j < numThread; j++) {
            threads[j].join();
        }

        // Reduce (merge results)
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

void MotifFinder::run(MotifSetting& setting) {
    Timer timer;
    this->load(setting.inFile, setting.directed);
    cout << "Parsed. Time took: " << timer.updateAnchorTime() << endl;
    this->sample(setting);
    cout << "Sampling completed. Time took: " << timer.updateAnchorTime() << endl;
    this->output(setting.outFile);
    cout << "Output completed. Time took: " << timer.updateAnchorTime() << endl;
}

