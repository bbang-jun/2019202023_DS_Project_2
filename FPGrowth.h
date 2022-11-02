#pragma once
#include "HeaderTable.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <set>

class FPGrowth // FP-Growth Class
{
private:
	int threshold; // restrict frequency
	FPNode* fpTree; // FP-Tree
	HeaderTable* table; // Header Table
	map<set<string>, int> frequenctPatterns; // generated Frequent Patterns
	ofstream* fout; // log file
	ofstream flog; // result file
public:
	FPGrowth(ofstream *fout, int threshold = 3) { // constructor
		this->threshold = threshold;
		flog.open("result.txt", ios::app);
		flog.setf(ios::fixed);
		fpTree = new FPNode;  table = new HeaderTable;
		this->fout = fout;
	}
	~FPGrowth(); // desctructor
	void createTable(char* item, int frequency) { table->insertTable(item, frequency); }
	void createFPtree(FPNode* root, HeaderTable* table, list<string> item_array, int frequency);
	void connectNode(HeaderTable* table, string item, FPNode* node);

	
	void frequenctPatternSetting() {
		table->ascendingIndexTable();
		frequenctPatterns = getFrequentPatterns(table, fpTree);
	}

	map<set<string>, int> getFrequentPatterns(HeaderTable* pTable, FPNode* pTree);
	void powerSet(map<set<string>, int>* FrequentPattern, vector<string> data, string item, int frequency, int* flag, int depth);
	bool contains_single_path(FPNode* pNode);
	int item_frequency(string item) {return table->find_frequency(item);}
	FPNode* getTree() { return fpTree; }
	HeaderTable* getHeaderTable() { return table; }

	bool printList();
	bool printTree();
	void saveFrequentPatterns();

	};


