#ifndef _BPTREEINDEXNODE_H_
#define _BPTREEINDEXNODE_H_

#include "BpTreeNode.h"
//template <class T>
class BpTreeIndexNode : public BpTreeNode { // inheritance
private:
	map <int, BpTreeNode*> mapIndex;

public:
	BpTreeIndexNode() {} // constructor


	void insertIndexMap(int n, BpTreeNode* pN) { // insert certain key and value of indexMap
		mapIndex.insert(map<int, BpTreeNode*>::value_type(n, pN)); // insert data
	}

	void deleteMap(int n) { // delete certain key of map
		mapIndex.erase(n);
	}

	map <int, BpTreeNode*>* getIndexMap() { return &mapIndex; } // return indexmap

};

#endif