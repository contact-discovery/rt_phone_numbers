/*
   RainbowCrack - a general propose implementation of Philippe Oechslin's faster time-memory trade-off technique.

   Copyright (C) Zhu Shuanglei <shuanglei@hotmail.com>
*/
#ifdef _WIN32
	#pragma warning(disable : 4786)
#endif

#ifdef _WIN32
	#include <windows.h>
    #include <direct.h>
    #define mkdir(dir, mode) _mkdir(dir)
#else
	#include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
#endif
#include <time.h>

#include <cstring>
#include <algorithm>
#include "ChainWalkContext.h"
#include "PrefixParser.h"
#include <iostream>
#include <mutex>
//#include <dlib/threads.h>
#include "omp.h"

void Usage()
{
	Logo();

	printf("usage: rtgen hash_algorithm \\\n");
	printf("             rainbow_table_index \\\n");
	printf("             rainbow_chain_length rainbow_chain_count \\\n");
    printf("             number_spec_file\n");
    printf("             out_folder\n");
	printf("\n");

	CHashRoutine hr;
	printf("hash_algorithm:       available: %s\n", hr.GetAllHashRoutineName().c_str());
	printf("rainbow_table_index:  index of the rainbow table\n");
	printf("rainbow_chain_length: length of the rainbow chain\n");
	printf("rainbow_chain_count:  count of the rainbow chain to generate\n");
	printf("number_spec_file:     file containing phone number specifications\n");
	printf("                      (must not contain any spaces)\n");
	printf("-out_folder:          folder to put rainbow tables in\n");
    printf("                      (must not contain any spaces)\n");
}

bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}



int main(int argc, char* argv[])
{

	if (argc != 7)
	{
		Usage();
		return 0;
	}

	string sHashRoutineName  = argv[1];
	int nRainbowTableIndex   = atoi(argv[2]);

	int nRainbowChainLen     = atoi(argv[3]);
	int nRainbowChainCount   = atoi(argv[4]);
	string sSpecFile         = argv[5];
    string sOutFolder        = argv[6];

    if (!hasEnding(sOutFolder, "/")) {
        sOutFolder.append("/");
    }

	// nRainbowChainCount check
	if (nRainbowChainCount >= 134217728)
	{
		printf("this will generate a table larger than 2GB, which is not supported\n");
		printf("please use a smaller rainbow_chain_count(less than 134217728)\n");
		return 0;
	}

    mkdir(sOutFolder.c_str(), 0770);

	// Setup CChainWalkContext
    cout << "Setting up... " << '\n';
    if (!CChainWalkContext::Setup(nRainbowTableIndex, nRainbowChainLen, sSpecFile, sHashRoutineName)) {
		printf("hash routine %s not supported\n", sHashRoutineName.c_str());
		return 0;
	}
    cout << "Done" << '\n';

	// Low priority
#ifdef _WIN32
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_IDLE);
#else
	nice(19);
#endif
	// FileName
	char szFileName[256];
	snprintf(szFileName, 255, "%s%s_%d_%dx%d_%s.rt", sOutFolder.c_str(),
	                                           sHashRoutineName.c_str(),
											   nRainbowTableIndex,
											   nRainbowChainLen,
											   nRainbowChainCount,
											   sSpecFile.c_str());

	cout << "Saving to: " << szFileName << '\n';

	// Open file
	fclose(fopen(szFileName, "a"));
	FILE* file = fopen(szFileName, "r+b");
	if (file == NULL)
	{
		printf("failed to create %s\n", szFileName);
		return 0;
	}

	// Check existing chains
	unsigned int nDataLen = GetFileLen(file);
	nDataLen = nDataLen / 16 * 16;
	if (nDataLen == nRainbowChainCount * 16)
	{
		printf("precomputation of this rainbow table already finished\n");
		fclose(file);
		return 0;
	}
	if (nDataLen > 0)
		printf("continuing from interrupted precomputation...\n");
	fseek(file, nDataLen, SEEK_SET);

	// Generate rainbow table
	CChainWalkContext cwc;
	cwc.Dump();

    printf("Generating...\n");
	clock_t t1 = clock();
    std::mutex file_mutex;
    bool stop = false;
    int i;
    #pragma omp parallel for
	for(i = nDataLen / 16; i < nRainbowChainCount; i++) {
	    if (stop) {
	        continue;
	    }
        CChainWalkContext cwc;
        cwc.GenerateRandomIndex();
        uint64 startIndex = cwc.GetIndex();

        int nPos;
        for (nPos = 0; nPos < nRainbowChainLen - 1; nPos++)
        {
            cwc.IndexToPlain();
            cwc.PlainToHash();
            cwc.HashToIndex(nPos);
        }

        uint64 endIndex = cwc.GetIndex();
        {
            std::lock_guard<std::mutex> lockGuard(file_mutex);
            if (fwrite(&startIndex, 1, 8, file) != 8)
            {
                printf("disk write fail 2\n");
                stop = true;
            }
            if (fwrite(&endIndex, 1, 8, file) != 8)
            {
                printf("disk write fail 3\n");
                stop = true;
            }
        }
	}
    clock_t t2 = clock();
    int nSecond = (t2 - t1) / CLOCKS_PER_SEC;
    printf("%d rainbow chains generated in %d m %d s\n", nRainbowChainCount,
           nSecond / 60,
           nSecond % 60);

	// Close
	fclose(file);

	return 0;
}
