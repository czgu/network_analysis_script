#include "motif_hash.h"

MotifHash::MotifHash() {
    this->load();
}

MotifHash::~MotifHash() {
    this->save();
}


