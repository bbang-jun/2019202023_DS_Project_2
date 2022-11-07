#pragma once
#include "HeaderTable.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <set>

class FPGrowth // FP-Growth Class
{
private:
	int threshold; // restrict frequency (빈도수 제한)
	FPNode* fpTree; // FP-Tree(자식 노드를 제외하고 NULL 값을 가짐)-root
	HeaderTable* table; // Header Table
	map<set<string>, int> frequenctPatterns; // generated Frequent Patterns(생성된 Frequent Patterns)
	ofstream* fout; // log file (log 파일)
	ofstream flog; // result file (result 파일)
public:
	FPGrowth(ofstream *fout, int threshold = 3) { // constructor
		this->threshold = threshold; // set threshold 
		flog.open("result.txt", ios::app); 
		flog.setf(ios::fixed); // 고정 소수점 출력
		fpTree = new FPNode;  table = new HeaderTable; // 동적할당
		this->fout = fout;
	}
	~FPGrowth(); // desctructor
	void createTable(string item, int frequency) { table->insertTable(item, frequency); } // create index table, data table
	void createFPtree(FPNode* root, HeaderTable* table, list<string> item_array, int frequency); // create FP tree
	void connectNode(HeaderTable* table, string item, FPNode* node); // connect the node in FP tree

	
	void frequenctPatternSetting() { 
		table->ascendingIndexTable();
		frequenctPatterns = getFrequentPatterns(table, fpTree);
	}

	map<set<string>, int> getFrequentPatterns(HeaderTable* pTable, FPNode* pTree);
	void powerSet(map<set<string>, int>* FrequentPattern, vector<string> data, string item, int frequency, int* flag, int depth);
	bool contains_single_path(FPNode* pNode);
	int item_frequency(string item) {return table->find_frequency(item);}
	FPNode* getTree() { return fpTree; } // return fpTree
	HeaderTable* getHeaderTable() { return table; } // return table

	bool printList();
	bool printTree();
	void saveFrequentPatterns();
};