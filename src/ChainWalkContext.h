/*
   RainbowCrack - a general propose implementation of Philippe Oechslin's faster time-memory trade-off technique.

   Copyright (C) Zhu Shuanglei <shuanglei@hotmail.com>
*/

#ifndef _CHAINWALKCONTEXT_H
#define _CHAINWALKCONTEXT_H

#include "HashRoutine.h"
#include "Public.h"

class CChainWalkContext
{
public:
	CChainWalkContext();
	virtual ~CChainWalkContext();
    static int m_nRainbowTableIndex;
    static uint64 m_nPlainSpaceTotal; // Configuration

private:
	static string m_sHashRoutineName;
	static HASHROUTINE m_pHashRoutine;							// Configuration
	static int m_nHashLen;										// Configuration

	//static unsigned char m_PlainCharset[256];					// Configuration

								// Performance consideration

	static uint64 m_nReduceOffset;								// Performance consideration

	// Context
	uint64 m_nIndex;
	unsigned char m_Plain[MAX_PLAIN_LEN];
	int m_nPlainLen;
	unsigned char m_Hash[MAX_HASH_LEN];

	static vector<tuple<uint64, uint64, int, string>> prefix_ranges;

private:
    static bool SetHashRoutine(string sHashRoutineName);
    static bool SetRainbowTableCountry(string newcountry);

public:
	static bool Setup(int nRainbowTableIndex, int width, string country, string shashRoutineName);
	static bool SetupWithPathName(string sPathName, int& nRainbowChainLen, int& nRainbowChainCount);	// Wrapper
	static string GetHashRoutineName();
	static int GetHashLen();
	static int GetRainbowTableIndex();
	static void Dump();

	void GenerateRandomIndex();
	void SetIndex(uint64 nIndex);
	void SetHash(unsigned char* pHash);		// The length should be m_nHashLen

	void IndexToPlain();
	void PlainToHash();
	void HashToIndex(int nPos);

	uint64 GetIndex();
	string GetPlain();
	string GetBinary();
	string GetPlainBinary();
	string GetHash();
	bool CheckHash(unsigned char* pHash);	// The length should be m_nHashLen
};

#endif
