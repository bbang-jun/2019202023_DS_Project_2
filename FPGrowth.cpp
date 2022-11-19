#include "FPGrowth.h"


FPGrowth::~FPGrowth() {
	delete fpTree;
	delete table;
}

void FPGrowth::createFPtree(FPNode* root, HeaderTable* table, list<string> item_array, int frequency) {

	FPNode*curNode=root; // 현재 노드는 root
	map<string, FPNode*>::iterator dataTableIter; // dataTable의 반복자
	list<pair<int,string>>::iterator indexTableIter; // indexTable의 반복자
	list<string>::iterator item_array_Iter; 

	while(item_array.empty()!=true){ // while item_array is not empty

		FPNode*findChildren = curNode->getChildrenNode(item_array.front());
		if(findChildren==NULL){  // if find the first item name in item_array, but it is not found
			FPNode*newNode=new FPNode;
			newNode->updateFrequency(frequency);
			newNode->setParent(curNode);
			curNode->pushchildren(item_array.front(), newNode); // push the children(map)
			
			int i=0;
			int dataTableSize=getHeaderTable()->getdataTable().size();
			map<string, FPNode*> tempDataTable=getHeaderTable()->getdataTable();

			while(i!=dataTableSize){
				if(tempDataTable.begin()->first==item_array.front()){

					FPNode* temp=table->getdataTable().find(item_array.front())->second;
					if(temp->getNext()==NULL){
						temp->setNext(newNode); // dataTable의 Pointer가 처음 들어온 각 상품 노드를 가리킴
					}
					else{
						while(temp->getNext()!=NULL) // dataTable의 각 item 이름을 가진 fp-tree의 item node들과 서로 연결
							temp=temp->getNext();
						temp->setNext(newNode);
					}
				}
				tempDataTable.erase(tempDataTable.begin());
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
}

// connect with table and node
void FPGrowth::connectNode(HeaderTable* table, string item, FPNode* node) {
	
}

list<string> FPGrowth::makeOrderedTransaction(list<string> item_array){
	
}

bool FPGrowth::contains_single_path(FPNode* pNode) {
	if (pNode->getChildren().size() == 0) return true;
	else if (pNode->getChildren().size() > 1) return false;
	return contains_single_path(pNode->getChildren().begin()->second);
}

map<set<string>, int> FPGrowth::getFrequentPatterns(HeaderTable* pTable, FPNode* pTree) {

	return {};
}



void FPGrowth::powerSet(map<set<string>, int>* FrequentPattern, vector<string> data, string item, int frequency, int* ptr, int depth) {
	if (data.size() == depth) {
		set<string> set; set.insert(item);
		for (int i = 0; i < data.size(); i++) { if (ptr[i] == 1) set.insert(data[i]); }
		FrequentPattern->insert(make_pair(set, frequency)); return;
	}
	ptr[depth] = 1;
	powerSet(FrequentPattern, data, item, frequency, ptr, depth + 1);
	ptr[depth] = 0;
	powerSet(FrequentPattern, data, item, frequency, ptr, depth + 1);
}

bool FPGrowth::printList() {

	return true;
}
bool FPGrowth::printTree() {
	return true;
}
void FPGrowth::saveFrequentPatterns(){

}
