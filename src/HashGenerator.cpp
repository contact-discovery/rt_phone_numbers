//
// Created by baschdl on 22.06.19.
//

#include "ChainWalkContext.h"
#include <iostream>



int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage:\nhashgen hashCount specfile\nhashCount: Number of hashes to produce\nspecFile:  file with number specifications\n           (must not contain any spaces)" << '\n';
        return 0;
    }
    string sSpecFile = argv[2];
    uint64 hashCount = atoi(argv[1]);

    CChainWalkContext::Setup(1, 1000, sSpecFile, "sha1");

    CChainWalkContext cwc;
//    cwc.Dump();
    for (uint64_t i = 0; i < hashCount; i++) {
        cwc.GenerateRandomIndex();
        cwc.IndexToPlain();
        cwc.PlainToHash();
        cout << cwc.GetHash() << '\n';
    }
}
