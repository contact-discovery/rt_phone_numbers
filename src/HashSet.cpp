/*
   RainbowCrack - a general propose implementation of Philippe Oechslin's faster time-memory trade-off technique.

   Copyright (C) Zhu Shuanglei <shuanglei@hotmail.com>
*/

#ifdef _WIN32
	#pragma warning(disable : 4786)
#endif

#include "HashSet.h"
#include <mutex>

CHashSet::CHashSet()
{
}

CHashSet::~CHashSet()
{
}

void CHashSet::AddHash(string sHash)
{
    std::lock_guard<std::mutex> lockGuard(mutex);
	if (sHash == "aad3b435b51404ee")
		return;

	int i;
	for (i = 0; i < m_vHash.size(); i++)
	{
		if (m_vHash[i] == sHash)
			return;
	}

	//printf("debug: adding hash %s\n", sHash.c_str());

	m_vHash.push_back(sHash);
	m_vFound.push_back(false);
	m_vPlain.push_back("");
	m_vBinary.push_back("");
}

bool CHashSet::AnyhashLeft()
{
    std::lock_guard<std::mutex> lockGuard(mutex);
	int i;
	for (i = 0; i < m_vHash.size(); i++)
	{
		if (!m_vFound[i])
			return true;
	}

	return false;
}

bool CHashSet::AnyHashLeftWithLen(int nLen)
{
    std::lock_guard<std::mutex> lockGuard(mutex);
	int i;
	for (i = 0; i < m_vHash.size(); i++)
	{
		if (!m_vFound[i])
			if (m_vHash[i].size() == nLen * 2)
				return true;
	}

	return false;
}

void CHashSet::GetLeftHashWithLen(vector<string>& vHash, int nLen)
{
    std::lock_guard<std::mutex> lockGuard(mutex);
	vHash.clear();

	int i;
	for (i = 0; i < m_vHash.size(); i++)
	{
		if (!m_vFound[i])
			if (m_vHash[i].size() == nLen * 2)
				vHash.push_back(m_vHash[i]);
	}
}

void CHashSet::SetPlain(string sHash, string sPlain, string sBinary)
{
    std::lock_guard<std::mutex> lockGuard(mutex);
	int i;
	for (i = 0; i < m_vHash.size(); i++)
	{
		if (m_vHash[i] == sHash)
		{
			m_vFound[i]    = true;
			m_vPlain[i]    = sPlain;
			m_vBinary[i]   = sBinary;
			return;
		}
	}
}

bool CHashSet::GetPlain(string sHash, string& sPlain, string& sBinary)
{
    std::lock_guard<std::mutex> lockGuard(mutex);
	if (sHash == "aad3b435b51404ee")
	{
		sPlain  = "";
		sBinary = "";
		return true;
	}

	int i;
	for (i = 0; i < m_vHash.size(); i++)
	{
		if (m_vHash[i] == sHash)
		{
			if (m_vFound[i])
			{
				sPlain  = m_vPlain[i];
				sBinary = m_vBinary[i];
				return true;
			}
		}
	}

	return false;
}

int CHashSet::GetStatHashFound()
{
    std::lock_guard<std::mutex> lockGuard(mutex);
	int nHashFound = 0;
	int i;
	for (i = 0; i < m_vHash.size(); i++)
	{
		if (m_vFound[i])
			nHashFound++;
	}

	return nHashFound;
}

int CHashSet::GetStatHashTotal()
{
    std::lock_guard<std::mutex> lockGuard(mutex);
	return m_vHash.size();
}
