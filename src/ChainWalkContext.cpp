/*
   RainbowCrack - a general propose implementation of Philippe Oechslin's faster time-memory trade-off technique.

   Copyright (C) Zhu Shuanglei <shuanglei@hotmail.com>
*/

#ifdef _WIN32
	#pragma warning(disable : 4786)
#endif

#include "ChainWalkContext.h"

#include <cstring>
#include <ctype.h>
#include <math.h>
#include <sstream>
#include <iostream>
#include <openssl/rand.h>
#include <stdlib.h>
#ifdef _WIN32
	#pragma comment(lib, "libcrypto.lib")
#endif

#include "PrefixParser.h"

//////////////////////////////////////////////////////////////////////

string CChainWalkContext::m_sHashRoutineName;
HASHROUTINE CChainWalkContext::m_pHashRoutine;
int CChainWalkContext::m_nHashLen;

uint64 CChainWalkContext::m_nPlainSpaceTotal;

int CChainWalkContext::m_nRainbowTableIndex;
uint64 CChainWalkContext::m_nReduceOffset;

vector<tuple<uint64, uint64, int, string>> CChainWalkContext::prefix_ranges;

//////////////////////////////////////////////////////////////////////

CChainWalkContext::CChainWalkContext()
{
}

CChainWalkContext::~CChainWalkContext()
{
}

//////////////////////////////////////////////////////////////////////

bool CChainWalkContext::SetHashRoutine(string sHashRoutineName)
{
	CHashRoutine hr;
	hr.GetHashRoutine(sHashRoutineName, m_pHashRoutine, m_nHashLen);
	if (m_pHashRoutine != NULL)
	{
		m_sHashRoutineName = sHashRoutineName;
		return true;
	}
	else
		return false;
}


bool CChainWalkContext::Setup(int nRainbowTableIndex,int width, string specFile, string shashRoutineName) {
    SetRainbowTableCountry(specFile);
    m_nRainbowTableIndex = nRainbowTableIndex;
    m_nReduceOffset = m_nPlainSpaceTotal / width + (65536 * nRainbowTableIndex);
    return SetHashRoutine(shashRoutineName);
}

bool CChainWalkContext::SetRainbowTableCountry(string specFile)
{
    if (specFile == "")
        return false;

    char cSpecFile[65];
    snprintf(cSpecFile, 64, "./specs/%s.csv", specFile.c_str());
    string mySpecFile(cSpecFile);
    vector<tuple<string, int>> prefixes = PrefixParser::read_numberspecs(mySpecFile);

    uint64 count = 0;
    for (tuple<string, int> prefix : prefixes) {
        uint64 localcount = pow(10, std::get<1>(prefix));
        uint64 end = count + localcount - 1;
        prefix_ranges.emplace_back(make_tuple(count, end, std::get<1>(prefix), std::get<0>(prefix)));
        count += localcount;
    }
    m_nPlainSpaceTotal = count;
    return true;
}

bool CChainWalkContext::SetupWithPathName(string sPathName, int& nRainbowChainLen, int& nRainbowChainCount)
{
	// something like lm_alpha#1-7_0_100x16_test.rt

#ifdef _WIN32
	int nIndex = sPathName.find_last_of('\\');
#else
	int nIndex = sPathName.find_last_of('/');
#endif
	if (nIndex != -1)
		sPathName = sPathName.substr(nIndex + 1);

	if (sPathName.size() < 3)
	{
		printf("%s is not a rainbow table\n", sPathName.c_str());
		return false;
	}
	if (sPathName.substr(sPathName.size() - 3) != ".rt")
	{
		printf("%s is not a rainbow table\n", sPathName.c_str());
		return false;
	}

	// Parse
	vector<string> vPart;
	if (!SeperateString(sPathName, "__x_.", vPart))
	{
		printf("filename %s not identified\n", sPathName.c_str());
		return false;
	}

	string sHashRoutineName   = vPart[0];
	int nRainbowTableIndex    = atoi(vPart[1].c_str());
	int mRainbowChainLen          = atoi(vPart[2].c_str());
	int mRainbowChainCount        = atoi(vPart[3].c_str());
	string sNumberSpecs = vPart[4];

	nRainbowChainCount = mRainbowChainCount;
	nRainbowChainLen = mRainbowChainLen;
    Setup(nRainbowTableIndex, mRainbowChainLen, sNumberSpecs, sHashRoutineName);

	return true;
}

string CChainWalkContext::GetHashRoutineName()
{
	return m_sHashRoutineName;
}

int CChainWalkContext::GetHashLen()
{
	return m_nHashLen;
}


int CChainWalkContext::GetRainbowTableIndex()
{
	return m_nRainbowTableIndex;
}

void CChainWalkContext::Dump()
{
	printf("hash routine: %s\n", m_sHashRoutineName.c_str());
	printf("hash length: %d\n", m_nHashLen);
	printf("rainbow table index: %d\n", m_nRainbowTableIndex);
	printf("reduce offset: %s\n", uint64tostr(m_nReduceOffset).c_str());
	printf("Maximum index: %ld\n", m_nPlainSpaceTotal);
	/*printf("prefixes: \n");
	for (tuple<uint64, uint64, uint, string> tuple: prefix_ranges) {
	    printf("\t%s: from %ld to %ld; %d digits\n", std::get<3>(tuple).c_str(), std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple));
	}*/
	printf("\n");
}

void CChainWalkContext::GenerateRandomIndex()
{
	RAND_bytes((unsigned char*)&m_nIndex, 64);
	m_nIndex = m_nIndex % m_nPlainSpaceTotal;
}

void CChainWalkContext::SetIndex(uint64 nIndex)
{
	m_nIndex = nIndex;
}

void CChainWalkContext::SetHash(unsigned char* pHash)
{
	memcpy(m_Hash, pHash, m_nHashLen);
}

void CChainWalkContext::IndexToPlain()
{
    int lower = 0;
    int upper = prefix_ranges.size() - 1;
    int index = 0;
    tuple<uint64, uint64, int, string> prefix_tuple;
    while (lower <= upper) {
        index = (lower + upper) / 2;
        prefix_tuple = prefix_ranges[index];
        if (std::get<1>(prefix_tuple) < m_nIndex) {
            lower = index + 1;
        } else if (std::get<0>(prefix_tuple) > m_nIndex) {
            upper = index - 1;
        } else {
            break;
        }
    }

    //cout << std::get<3>(prefix_tuple) << ":" << std::get<2>(prefix_tuple) << ":";

    uint64 range_index = m_nIndex - std::get<0>(prefix_tuple);
    std::ostringstream oss;
    oss << range_index;
    string number = oss.str();
    //cout << number << '\n';
    int num_zeros = std::get<2>(prefix_tuple) - number.length();
    int i = 0;
    for (i; i < std::get<3>(prefix_tuple).length(); i++) {
        m_Plain[i] = std::get<3>(prefix_tuple).at(i);
    }
    int j = i;
    for (j; j < num_zeros + i; j++) {
        m_Plain[j] = '0';
    }
    int k = j;
    for (int i = 0; i < number.length(); i++) {
        m_Plain[k++] = number.at(i);
    }
    m_Plain[k] = '\0';
    m_nPlainLen = k;
    //cout << GetPlain() << '\n';
}

void CChainWalkContext::PlainToHash()
{
	m_pHashRoutine(m_Plain, m_nPlainLen, m_Hash);
}

void CChainWalkContext::HashToIndex(int nPos)
{
	m_nIndex = (*(uint64*)m_Hash + (m_nReduceOffset * nPos)) % m_nPlainSpaceTotal;
}

uint64 CChainWalkContext::GetIndex()
{
	return m_nIndex;
}

string CChainWalkContext::GetPlain()
{
	string sRet;
	int i;
	for (i = 0; i < m_nPlainLen; i++)
	{
		char c = m_Plain[i];
		if (c >= 32 && c <= 126)
			sRet += c;
		else
			sRet += '?';
	}

	return sRet;
}

string CChainWalkContext::GetBinary()
{
	return HexToStr(m_Plain, m_nPlainLen);
}

string CChainWalkContext::GetPlainBinary()
{
	string sRet;
	sRet += GetPlain();
	int i;
	for (i = 0; i < 17 - m_nPlainLen; i++)
		sRet += ' ';

	sRet += "|";

	sRet += GetBinary();
	for (i = 0; i < 17 - m_nPlainLen; i++)
		sRet += "  ";

	return sRet;
}

string CChainWalkContext::GetHash()
{
	return HexToStr(m_Hash, m_nHashLen);
}

bool CChainWalkContext::CheckHash(unsigned char* pHash)
{
	if (memcmp(m_Hash, pHash, m_nHashLen) == 0)
		return true;

	return false;
}
