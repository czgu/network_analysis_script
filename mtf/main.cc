#include "motif.h"
#include <cstdlib>
#include <time.h>
#include <iostream>

using namespace std;

int main(int argc,  char** argv) {
    if (argc < 5) {

    } else {
        srand (time(NULL));

        string fName(argv[1]);
        string fOutName(argv[2]);
        int numThread = atoi(argv[3]);
        int numSample = atoi(argv[4]);

        MotifFinder mfinder;
        mfinder.parseMotif(fName);
        mfinder.sample(numSample, numThread);
        mfinder.output(fOutName);
    }

    return 0;
}
