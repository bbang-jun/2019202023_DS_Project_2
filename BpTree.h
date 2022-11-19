#ifndef _BpTree_H_
#define _BpTree_H_

#include "BpTreeDataNode.h"
#include "BpTreeIndexNode.h"
#include <fstream>
#include <iostream>
#include "HeaderTable.h"

class BpTree {
private:
	BpTreeNode* root; // root of BpTree
	int			order;		// m children
	ofstream* fout;
public:
	BpTree(ofstream *fout, int order = 3) { // constructor
		root = NULL; // root is NULL
		this->order = order; // setting order
		this->fout = fout; // setting fout
	}
	
	
	bool		Insert(int key, set<string> set); // Insert Node fuction
	bool		excessDataNode(BpTreeNode* pDataNode); // satisfy dataNode split
	bool		excessIndexNode(BpTreeNode* pIndexNode); // satisfy indexNode split
	void		splitDataNode(BpTreeNode* pDataNode); // split of dataNode
	void		splitIndexNode(BpTreeNode* pIndexNode); // split of indexNode
	ofstream flog;
	BpTreeNode* getRoot() { return root; } // return the root
	BpTreeNode* searchDataNode(int n); // search certain frequency of DataNode

	void	printFrequentPatterns(set<string> pFrequentPattern); // print of frequentPatterns
	bool	printFrequency(string item, int min_frequency); // print of frequency
	bool	printConfidence(string item, double item_frequency, double min_confidence); // print of confidence
	bool 	printRange(string item, int min, int max); // print range of bptree

};

#endif
