#include "motif.h"
#include <cstdlib>

using namespace std;

int main(int argc,  char** argv) {
    if (argc < 5) {

    } else {
        string fName(argv[1]);
        string fOutName(argv[2]);
        int numThread = atoi(argv[3]);
        int numSample = atoi(argv[4]);
        MotifFinder mfinder;
        mfinder.parseMotif(fName);
        mfinder.sample(numThread, numSample);
        mfinder.output(fOutName);
    }

    return 0;
}
