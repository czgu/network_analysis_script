#include "motif.h"
#include <cstdlib>
#include <time.h>
#include <iostream>

using namespace std;

int main(int argc,  char** argv) {
    srand (time(NULL));
    if (argc < 3) {

    } else {
        MotifSetting setting;

        // default motif sampling values
        setting.inFile = argv[1];
        setting.outFile = argv[2];
        setting.numThread = 4;
        setting.numSample = 10000;
        setting.directed = false;

        setting.motifSizeMin = 3;
        setting.motifSizeMax = 4;

        if (argc > 3) {
            for (int i = 3; i < argc; i++) {
                char c = argv[i][0];
                int val = atoi(argv[i] + 1);

                if (c == 't') {
                    setting.numThread = val;
                } else if (c == 's') {
                    setting.numSample = val;
                } else if (c == 'd') {
                    setting.directed = val;
                } else if (c == 'm') {
                    setting.motifSizeMin = val / 10;
                    setting.motifSizeMax = val % 10;
                }
            }
        }
        cout << "in: " << setting.inFile << endl
             << "out: " << setting.outFile << endl
             << "num thread: " << setting.numThread << endl
             << "num sample: " << setting.numSample << endl
             << "directed: " << setting.directed << endl
             << "motif size: " << setting.motifSizeMin << '-' << setting.motifSizeMax << endl;

        MotifFinder mfinder;
        mfinder.run(setting);

    }

    return 0;
}
