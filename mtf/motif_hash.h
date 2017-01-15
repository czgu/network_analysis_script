#include <map>
#include <fstream>

#define DIRECTED_BIT (1 << 8)

typedef unsigned short ushort;



class MotifHash {
public:
    MotifHash();
    ~MotifHash();

    void setMotifType(ushort motifSize, bool directed);
    ushort identifyMotifType(std::vector<int> degrees)
private:
    void load();
    void save();

    std::map<ushort, std::map<int, ushort> > motifHashMap;
};
