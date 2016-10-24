#ifndef MOTIF_H
#define MOTIF_H

#include <string>
#include "graph.h"

class Motif {
};

class MotifFinder {
public:
    MotifFinder();
    void parseMotif(std::string fName);
    void sample(int numSample, int numThread);
    void output(std::string fName);
private:

};



#endif
