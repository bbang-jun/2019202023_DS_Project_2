#pragma once

#include <set>
#include <map>
using namespace std;

class FrequentPatternNode
{
private:
	int frequency;		// FrequentPattern frequency
	multimap<int, set<string> > FrequentPatternList;	// first is FrequentPattern size, second is FrequentPattern data

public:
	FrequentPatternNode() { frequency = 0; } // constructor
	~FrequentPatternNode() { FrequentPatternList.clear(); } // desctructor
	void setFrequency(int frequency) { this->frequency = frequency; } // set the freuqncy
	void InsertList(set<string> item) { FrequentPatternList.insert(multimap<int, set<string>>::value_type(item.size(),item)); } // insert set to FrequentPastternList
	int getFrequency() { return frequency; } // return the frequency
	multimap<int, set<string> > getList() { return FrequentPatternList; } // return the FrequentPatternList
};


