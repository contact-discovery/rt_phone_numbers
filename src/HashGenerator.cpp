//
// Created by baschdl on 22.06.19.
//

#include "ChainWalkContext.h"
#include <iostream>



int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage:\nhashgen hashAlg hashCount specfile\nhashAlg: hash algorithm to be used\nhashCount: Number of hashes to produce\nspecFile:  file with number specifications\n           (must not contain any spaces)" << '\n';
        return 0;
    }
	string hashAlg = argv[1];
    string sSpecFile = argv[3];
    uint64 hashCount = atoi(argv[2]);

    CChainWalkContext::Setup(1, 1000, sSpecFile, hashAlg);

    CChainWalkContext cwc;
//    cwc.Dump();
    for (uint64_t i = 0; i < hashCount; i++) {
        cwc.GenerateRandomIndex();
        cwc.IndexToPlain();
        cwc.PlainToHash();
        cout << cwc.GetHash() << '\n';
    }
}
