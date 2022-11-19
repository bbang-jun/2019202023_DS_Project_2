#pragma once
#include "FrequentPatternNode.h"
#include "FPNode.h"
class BpTreeNode
{
private:
	BpTreeNode* pParent;
	BpTreeNode* pMostLeftChild;

public:
	BpTreeNode() { // constructor
		pParent = NULL;
		pMostLeftChild = NULL;
	}
	~BpTreeNode() {} // dectructor

	void setMostLeftChild(BpTreeNode* pN) { pMostLeftChild = pN; } // sertting most left child of node
	void setParent(BpTreeNode* pN) { pParent = pN; } // set the parent node(pointer)

	BpTreeNode* getParent() { return pParent; } // return parnet node
	BpTreeNode* getMostLeftChild() { return pMostLeftChild; } // return most left child node

	virtual void setNext(BpTreeNode* pN) {} // virtural fuction for polymorphism
	virtual void setPrev(BpTreeNode* pN) {} // virtural fuction for polymorphism
	virtual BpTreeNode* getNext() { return NULL; } // virtural fuction for polymorphism
	virtual BpTreeNode* getPrev() { return NULL; } // virtural fuction for polymorphism

	virtual void insertDataMap(int n, FrequentPatternNode* pN) {} // virtural fuction for polymorphism
	virtual void insertIndexMap(int n, BpTreeNode* pN) {} // virtural fuction for polymorphism
	virtual void deleteMap(int n) {}

	virtual map<int, BpTreeNode*>* getIndexMap() { return {}; } // virtural fuction for polymorphism
	virtual map<int, FrequentPatternNode*>* getDataMap() { return {}; } // virtural fuction for polymorphism

};
