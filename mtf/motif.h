#ifndef MOTIF_H
#define MOTIF_H

#include <string>
#include <map>
#include <vector>
#include "graph.h"

struct MotifSampleResult {
    std::map<int, double> scores;
    unsigned int motifSize;

    void normalizeResult();
    void sum(const MotifSampleResult& r);
};

class MotifFinder {
public:
    MotifFinder();
    void parseMotif(std::string fName);
    void sample(int numSample, int numThread);
    void output(std::string fName);
    void run(std::string inFile, std::string outFile, int numThread, int numSample);
private:
    Graph graph;
    std::vector<MotifSampleResult *> results;
};

void findAllPossibleVertices(std::vector<long>& vertices, std::vector<long>& possibleVertices, Graph& graph, long vertex);
double findProbability(unsigned int motifSize, Graph& graph, int counted_vertices, std::vector<long>& vertices, std::vector<long>& edges, std::vector<long>& current_vertices);
int categorizeMotif(std::vector<int>& vertex_degrees, unsigned int motifSize);
void MotifSample(unsigned int motifSize, int numSample, MotifSampleResult** result, Graph* graph_ptr);


#endif
