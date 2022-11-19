#include "FPGrowth.h"


FPGrowth::~FPGrowth() { // dectructor
	delete fpTree;
	delete table;
}

void FPGrowth::createFPtree(FPNode* root, HeaderTable* table, list<string> item_array, int frequency) { // create FPGrowth Tree

	FPNode*curNode=root; // current node is root

	while(item_array.empty()!=true){ // while item_array is not empty

		FPNode*findChildren = curNode->getChildrenNode(item_array.front());
		if(findChildren==NULL){  // if find the first item name in item_array, but it is not found
			FPNode*newNode=new FPNode;
			newNode->updateFrequency(frequency); // setting newNode
			newNode->setParent(curNode); // setting newNode
			curNode->pushchildren(item_array.front(), newNode); // push the children(map)
			
			int i=0;
			int dataTableSize=getHeaderTable()->getdataTable().size(); // get dataTable's size
			map<string, FPNode*> tempDataTable=getHeaderTable()->getdataTable(); // make tempDataTable for create tree

			while(i!=dataTableSize){
				if(tempDataTable.begin()->first==item_array.front()){ // compare with temp data table and item arrays first element

					FPNode* temp=table->getdataTable().find(item_array.front())->second;
					if(temp->getNext()==NULL){
						temp->setNext(newNode); // dataTable's Pointer is pointing first insert each item nod
					}
					else{
						while(temp->getNext()!=NULL) // connect with dataTable's item name and fp-tree's item node
							temp=temp->getNext();
						temp->setNext(newNode); // connect node
					}
				}
				tempDataTable.erase(tempDataTable.begin()); // erase tempDataTable because begin() element(compare complete)
				i++;
			}
			
			item_array.pop_front(); // push after, pop front
			curNode=newNode;
		}
		else{ // if alreay insert
			findChildren->updateFrequency(frequency); // update frequency
			curNode=findChildren;
			FPNode*newNode=new FPNode;
			newNode->updateFrequency(frequency);
			newNode->setParent(curNode);
			curNode->pushchildren(item_array.front(), newNode); // push the children(map)
			item_array.pop_front(); // update after, pop front
		}
	}

	return;
}

// connect with table and node
void FPGrowth::connectNode(HeaderTable* table, string item, FPNode* node) {
	// implement in FPGrowth.cpp 30~37 lines
}

list<string> FPGrowth::makeOrderedTransaction(list<string> item_array){ // sort transaction for ascending frequency
	// implement in Manager.cpp 189~240 lines
}

bool FPGrowth::contains_single_path(FPNode* pNode) { // for implement SAVE
	if (pNode->getChildren().size() == 0) return true;
	else if (pNode->getChildren().size() > 1) return false;
	return contains_single_path(pNode->getChildren().begin()->second);
}

map<set<string>, int> FPGrowth::getFrequentPatterns(HeaderTable* pTable, FPNode* pTree) { // for implement SAVE

	return {};
}



void FPGrowth::powerSet(map<set<string>, int>* FrequentPattern, vector<string> data, string item, int frequency, int* ptr, int depth) { // for implement SAVE
	if (data.size() == depth) {
		set<string> set; set.insert(item);
		for (int i = 0; i < data.size(); i++) { if (ptr[i] == 1) set.insert(data[i]); }
		FrequentPattern->insert(make_pair(set, frequency)); return;
	}
	ptr[depth] = 1;
	powerSet(FrequentPattern, data, item, frequency, ptr, depth + 1); // recursive
	ptr[depth] = 0;
	powerSet(FrequentPattern, data, item, frequency, ptr, depth + 1); // recursive
}

bool FPGrowth::printList() {

	return true;
}
bool FPGrowth::printTree() {
	return true;
}
void FPGrowth::saveFrequentPatterns(){

}
