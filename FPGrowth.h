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
	FPNode* fpTree; // FP-Tree root
	HeaderTable* table; // Header Table
	map<set<string>, int> frequenctPatterns; // generated Frequent Patterns
	ofstream* fout; // log file
	ofstream flog; // result file
public:
	FPGrowth(ofstream *fout, int threshold = 3) { // constructor
		this->threshold = threshold; // set threshold 
		flog.open("result.txt", ios::app); 
		flog.setf(ios::fixed); // print fixed float point
		fpTree = new FPNode;  table = new HeaderTable; // dynamic allocate
		this->fout = fout;
	}
	~FPGrowth(); // desctructor
	void createTable(string item, int frequency) { table->insertTable(item, frequency); } // create index table, data table
	void createThIndexTable() { table->makeThIndexTable(threshold); }
	void createFPtree(FPNode* root, HeaderTable* table, list<string> item_array, int frequency); // create FP tree
	void connectNode(HeaderTable* table, string item, FPNode* node); // connect the node in FP tree


	list<string> makeOrderedTransaction(list<string> item_array); // sort transaction for ascending frequency

	
	void frequenctPatternSetting() { 
		table->ascendingIndexTable();
		frequenctPatterns = getFrequentPatterns(table, fpTree);
	}

	map<set<string>, int> getFrequentPatterns(HeaderTable* pTable, FPNode* pTree); // for implement save
	void powerSet(map<set<string>, int>* FrequentPattern, vector<string> data, string item, int frequency, int* flag, int depth); // for implement save
	bool contains_single_path(FPNode* pNode); // for implement save
	int item_frequency(string item) {return table->find_frequency(item);} // for implement save
	FPNode* getTree() { return fpTree; } // return fpTree
	HeaderTable* getHeaderTable() { return table; } // return table

	bool printList(); // print of list
	bool printTree(); // print of tree
	void saveFrequentPatterns(); // for implement save
};