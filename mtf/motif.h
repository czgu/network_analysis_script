#ifndef MOTIF_H
#define MOTIF_H

#include <string>
#include <map>
#include <vector>
#include "graph.h"

// This is to store the result of multiple motif sample results
// Can be merged with multiple results that are computed in parallel
struct MotifSampleResult {
    // motif id : % distribution pairs
    std::map<int, double> scores;
    unsigned int motifSize;

    // normalize distribution of scores so that they sum to 1
    void normalizeResult();

    // update the motif sample result by combining anothor motif result
    void sum(const MotifSampleResult& r);
};

// Struct to store all sett
struct MotifSetting {
    std::string inFile;
    std::string outFile;
    int numThread;
    int numSample;
    bool directed;
    int motifSizeMin;
    int motifSizeMax;
};

class MotifFinder {
public:
    MotifFinder();

    // Load the graph with the given motif file
    void load(std::string fName, bool isDirected);

    // Run sampling on the graph, with the given setting. Can be multithreaded.
    void sample(MotifSetting& setting);

    // Output the results to file
    void output(std::string fName);

    // load graph, sample, and output
    void run(MotifSetting& setting);
private:
    Graph graph;
    std::vector<MotifSampleResult *> results;
};

// Given a vertex in a graph, find all of its neighbors that is not already in vertices and add them to possibleVertices
void findAllPossibleVertices(std::vector<long>& vertices, std::vector<long>& possibleVertices, Graph& graph, long vertex);

// Given a sub graph, calculate the probability that all vertices of the subgraph will be selected by picking edges randomly
double findProbability(unsigned int motifSize, Graph& graph, int counted_vertices, std::vector<long>& vertices, std::vector<long>& edges, std::vector<long>& current_vertices);

// Assign a motif id to the subgraph
int categorizeMotif(std::vector<int>& vertex_degrees, unsigned int motifSize);

// Perform the sampling algorithm on a single thread
void MotifSample(unsigned int motifSize, int numSample, MotifSampleResult** result, Graph* graph_ptr);


#endif
