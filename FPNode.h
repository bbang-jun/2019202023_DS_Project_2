#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include <cmath>
using namespace std;
class FPNode
{
private:
	int frequency; // item frequency
	FPNode* parent; // parent node
	FPNode* next; // next FPNode
	map<string, FPNode*> children; // map container with child node

public:
	FPNode(); // constructor
	~FPNode(); // destructor
	void setParent(FPNode* node) { this->parent = node; } // set node's parent pointer
	void setNext(FPNode* node) { next = node; } // dataTable's pointer is pointing fp-tree's certain node
	void pushchildren(string item, FPNode* node) { children.insert(map<string, FPNode*>::value_type(item, node)); } // push the children(connect)
	void updateFrequency(int frequency) { this->frequency += frequency; } // renew(updatate) frequency

	int getFrequency() { return frequency; } // return the frequecy
	FPNode* getParent() { return parent; } // return parent node
	FPNode* getNext() { return next; } // return next node
	FPNode* getChildrenNode(string item); // return childrenNode
	map<string, FPNode*> getChildren() { return children; } // return children map
	string getItemName(FPNode* node); // return ItemName
};
