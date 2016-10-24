#include "motif.h"

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

MotifFinder::MotifFinder() {

}

void MotifFinder::parseMotif(string fName) {
    ifstream in(fName);
 
    string line;
    while (getline(in, line)) {

    }

    in.close();
}

void MotifFinder::sample(int numSample, int numThread) {
    
}

void MotifFinder::output(string fName) {
    ofstream out;
    out.open(fName);
    
    out.close();
}
