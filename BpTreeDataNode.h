#ifndef _BpTreeDataNode_H_
#define _BpTreeDataNode_H_

#include "BpTreeNode.h"
class BpTreeDataNode : public BpTreeNode // inheritance
{
private:
	map <int, FrequentPatternNode*> mapData;
	BpTreeNode* pNext;
	BpTreeNode* pPrev;
public:
	BpTreeDataNode() { // constructor
		pNext = NULL;
		pPrev = NULL;
	}

	void setNext(BpTreeNode* pN) { pNext = pN; } // set next pointer
	void setPrev(BpTreeNode* pN) { pPrev = pN; } // sert prev pointer
	BpTreeNode* getNext() { return pNext; } // return next pointer
	BpTreeNode* getPrev() { return pPrev; } // return prev pointer

	void insertDataMap(int n, FrequentPatternNode* pN) { // insert certain key and value of dataMap
		mapData.insert(map<int, FrequentPatternNode*>::value_type(n, pN)); // insert data
	}

	void deleteMap(int n) { // delete certain key of map
		mapData.erase(n);
	}
	map<int, FrequentPatternNode*>* getDataMap() { return &mapData; } // return dataMap
};

#endif
