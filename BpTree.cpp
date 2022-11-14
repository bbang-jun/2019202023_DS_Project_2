#include "BpTree.h"

bool BpTree::Insert(int frequency, set<string> itemset) { // 2	milk	eggs
	if(itemset.size()<=1){ // 공집합이거나 원소가 하나인 집합 저장x
		return true;
	}

	int num=0;
	BpTreeNode*curNode;

	//BpTreeNode* checkSplit;
	if (root == NULL) { //if there's nothing in bptree
		BpTreeDataNode* dataNode = new BpTreeDataNode;
		FrequentPatternNode * newFqptNode = new FrequentPatternNode;
		newFqptNode->InsertList(itemset); // FrequentPatterNode의 1번째 인자와 2번째 인자 저장
		newFqptNode->setFrequency(frequency);
		dataNode->insertDataMap(frequency, newFqptNode); // 파란색 노드 완성
		root = dataNode;
	}
	else { 
		if(searchDataNode(frequency)==NULL){ // data 노드 목록 중에 frequency와 일치하는 dataNode가 없는 경우
			//map<int, FrequentPatternNode*>::iterator dataIter;
			
			curNode=root;
			while(curNode->getMostLeftChild()!=NULL){ // 가장 왼쪽 dataNode
				curNode=curNode->getMostLeftChild();
			}
			
			FrequentPatternNode * newFqptNode = new FrequentPatternNode;
			newFqptNode->InsertList(itemset); // FrequentPatterNode의 1번째 인자와 2번째 인자 저장

			if(excessDataNode(curNode)==false){ // order-1 이하이면 삽입
				curNode->insertDataMap(frequency, newFqptNode);
			}
			else{ // oder-1을 초과하면 스플릿
				curNode->insertDataMap(frequency, newFqptNode);
				//splitDataNode(curNode);
			}
			
		}
		else{ // data 노드 목록 중 frequency와 일치하는 dataNode가 있는 경우
			BpTreeNode* searchData = searchDataNode(frequency); // 같은 frequency면 frequent patterNode에 삽입
			map<int, FrequentPatternNode*>::iterator iter;

			for(iter=searchData->getDataMap()->begin(); iter!=searchData->getDataMap()->end(); iter++){
				if(iter->first==frequency){
					//searchData->getDataMap()
					iter->second->InsertList(itemset);
				}
			}
		}
	}

	return true;
}


BpTreeNode* BpTree::searchDataNode(int n) {
	BpTreeNode* pCur = root;

	map<int, FrequentPatternNode*>::iterator dataIter=pCur->getDataMap()->begin();

	if(pCur->getMostLeftChild()==NULL){
		for(dataIter=pCur->getDataMap()->begin(); dataIter!=pCur->getDataMap()->end(); dataIter++){
			if(dataIter->first==n){
				return pCur;
			}
		}
	}
	else{
		while(pCur->getMostLeftChild()!=NULL){
			pCur=pCur->getMostLeftChild();
		}
		while(pCur!=NULL){
			
			for(dataIter=pCur->getDataMap()->begin(); dataIter!=pCur->getDataMap()->end(); dataIter++){
				if(dataIter->first==n){
					return pCur;
				}
			}
			if(pCur->getNext()==NULL)
				break;
			pCur=pCur->getNext();
		}
	}
	
	return NULL;
}

void BpTree::splitDataNode(BpTreeNode* pDataNode) {
	BpTreeDataNode* newDataNode = new BpTreeDataNode;
	BpTreeIndexNode* newIndexNode = new BpTreeIndexNode;
	map<int, FrequentPatternNode*>* pData = pDataNode->getDataMap();
	int index = 1;
	int first, second;
	BpTreeNode* originNextNode = new BpTreeNode;
	originNextNode = pDataNode->getNext(); //originNextNode=pDataNode's next node

	for (auto iter = pData->begin(); iter != pData->end(); iter++) {
		if (index == 2) { //if index==2 (after executing loop once)
			newDataNode->insertDataMap(iter->first, iter->second); //insert newDataNode to data node
			if (pDataNode->getParent() != NULL) { //if pDataNode's parent exists
				pDataNode->getParent()->insertIndexMap(iter->first, newDataNode); //insert to pDataNode's parent(index node)
				newDataNode->setParent(pDataNode->getParent()); //set newDataNode's parent as pDataNode's parent
			}
			else { //if pDataNode's parent doesn't exist
				newIndexNode->insertIndexMap(iter->first, newDataNode); //insert newIndexNode as index node
				pDataNode->setParent(newIndexNode); //set pDataNode's parent as newIndexNode
				newIndexNode->setMostLeftChild(pDataNode); //set newIndexNode's most Left Child as pDataNode
				newDataNode->setParent(newIndexNode); //set newDataNode's parent as newIndexNode
			}
			first = iter->first; //store iter->first as string
		}
		if (index == 3) { //if index==3 (after executing loop twice)
			newDataNode->insertDataMap(iter->first, iter->second); //insert newDataNode as index node
			second = iter->first; //store iter->first as string
		}
		index++;
	}

	pDataNode->deleteMap(first); //delete with name(first)
	pDataNode->deleteMap(second); //delete with name(second)
	pDataNode->setNext(newDataNode); //set pDataNode's next as newDataNode
	newDataNode->setPrev(pDataNode); //set newDataNode's prev as pDataNode

	if (originNextNode != NULL) { //if originNextNode exists
		newDataNode->setNext(originNextNode); //set newDataNode's next as originNextNode
		originNextNode->setPrev(newDataNode); //set originNextNode's prev as newDataNode
	}
}


void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
	BpTreeIndexNode* newParentNode = new BpTreeIndexNode;
	BpTreeIndexNode* newChildNode = new BpTreeIndexNode;
	map<int, BpTreeNode*>* pIndex = pIndexNode->getIndexMap();
	BpTreeNode* originChildNode = new BpTreeNode;
	int index = 1;
	int first, second;
	for(auto iter = pIndex->begin(); iter != pIndex->end(); iter++) {
		if (index == 2) { //if index==2 (after executing this loop once)
			originChildNode = iter->second; //store iter->second as originChildNode
			newParentNode->insertIndexMap(iter->first, newChildNode); //insert to index map
			if (pIndexNode->getParent() != NULL) { //if pIndexNode's parent exists
				pIndexNode->getParent()->insertIndexMap(iter->first, newChildNode); //insert to index map
				newChildNode->setParent(pIndexNode->getParent()); //set newChildNode's parent as pIndexNode's parent
			}
			else { //if pIndexNode's parent doesn't exist
				newChildNode->setParent(newParentNode); //set newChildNode's parent as newParentNode
				pIndexNode->setParent(newParentNode); //set pIndexNode's parent as newParentNode
				newParentNode->setMostLeftChild(pIndexNode); //set newParentNode's most left child as pIndexNode
			}
			first = iter->first; //store iter->first as string
		}
		if (index == 3) { //if index==3 (after executing this loop twice)
			newChildNode->insertIndexMap(iter->first, iter->second); //insert to index map
			iter->second->setParent(newChildNode); //set iter->second's parent as newChildNode
			newChildNode->setMostLeftChild(originChildNode); //set newChildNode's most left child as originChildNode
			originChildNode->setParent(newChildNode); //set originChildNode's parent as newChildNode
			second = iter->first; //store iter->first as string
		}
		index++; //increase index
	}

	pIndexNode->deleteMap(first); //delete with name(first)
	pIndexNode->deleteMap(second); //delete with name(second)
}

bool BpTree::excessDataNode(BpTreeNode* pDataNode) {
	if (pDataNode->getDataMap()->size() > order - 1) return true;//order is equal to the number of elements 
	else return false;
}

bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) {
	if (pIndexNode->getIndexMap()->size() > order - 1)return true;//order is equal to the number of elements 
	else return false;
}


bool BpTree::printConfidence(string item, double item_frequency, double min_confidence)
{

	return true;
}
bool BpTree::printFrequency(string item, int min_frequency)//print winratio in ascending order
{

	return true;
}
bool BpTree::printRange(string item, int min, int max) {

	return true;
}
void BpTree::printFrequentPatterns(set<string> pFrequentPattern, string item) {
	*fout << "{";
	set<string> curPattern = pFrequentPattern;
	curPattern.erase(item);
	for (set<string>::iterator it = curPattern.begin(); it != curPattern.end();) {
		string temp = *it++;
		if (temp != item) *fout << temp;
		if (it == curPattern.end()) {
			*fout << "} ";
			break;
		}
		*fout << ", ";
	}
}
