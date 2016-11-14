#ifndef MOTIF_H
#define MOTIF_H

#include <string>
#include <map>
#include <vector>
#include "graph.h"

struct MotifSampleResult {
    std::map<int, double> scores;
    int motifSize;

    void normalizeResult();
    void sum(const MotifSampleResult& r);
};

class MotifFinder {
public:
    MotifFinder();
    void parseMotif(std::string fName);
    void sample(int numSample, int numThread);
    void output(std::string fName);
private:
    Graph graph;
    std::vector<MotifSampleResult *> results;
};



#endif
