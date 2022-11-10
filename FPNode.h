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
	int frequency; // item frequency(상품 빈도수)
	FPNode* parent; // parent node(부모 노드)
	FPNode* next; // next FPNode(다음 FPNode를 가리킴)
	map<string, FPNode*> children; // map container with child node(자식 노드를 가지고 있는 map 컨테이너)

public:
	FPNode(); // constructor
	~FPNode(); // destructor
	void setParent(FPNode* node) { this->parent = node; } // 해당 노드의 부모 설정
	void setNext(FPNode* node) { next = node; } // dataTable의 pointer가 fp-tree의 해당 노드를 가리킴
	void pushchildren(string item, FPNode* node) { children.insert(map<string, FPNode*>::value_type(item, node)); }
	void updateFrequency(int frequency) { this->frequency += frequency; }

	int getFrequency() { return frequency; }
	FPNode* getParent() { return parent; }
	FPNode* getNext() { return next; }
	FPNode* getChildrenNode(string item);
	map<string, FPNode*> getChildren() { return children; }
	string getItemName(FPNode* node);
};
