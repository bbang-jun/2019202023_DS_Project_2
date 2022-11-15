#include "BpTree.h"
#include<vector>

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
			
			while(curNode->getMostLeftChild()!=NULL){
				map <int, BpTreeNode*>::iterator iter;

				for(iter=curNode->getIndexMap()->begin(); iter!=curNode->getIndexMap()->end(); iter++){
					if(iter->first>=frequency){
						curNode=curNode->getMostLeftChild();
						break;
					}
					else if(iter->first<frequency){
						curNode=iter->second;
						break;
					}
				}
				if(curNode->getMostLeftChild()==NULL)
					break;
			}
			
			FrequentPatternNode * newFqptNode = new FrequentPatternNode;
			newFqptNode->InsertList(itemset); // FrequentPatterNode의 1번째 인자와 2번째 인자 저장

			curNode->insertDataMap(frequency, newFqptNode);

			if(excessDataNode(curNode)==true){
				splitDataNode(curNode);
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

	while(pCur!=NULL){
		if(pCur->getMostLeftChild()==NULL)
			break;
		pCur=pCur->getMostLeftChild();
	}

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
	BpTreeIndexNode* newIndexNode;
	int splitIndex = ((order-1)/2.0)+1;
	int compareIndex=1;
	vector<int> v;
	bool splitIndexNodes = false;

	map<int, FrequentPatternNode*>::iterator iter;

	// 새로 생긴 dataNode에 split할 index 이후의 요소들 삽입
	for(iter=pDataNode->getDataMap()->begin(); iter!=pDataNode->getDataMap()->end(); iter++){
		if(compareIndex==splitIndex){ 
			if(pDataNode->getParent()!=NULL){ // 스플릿할 노드의 부모(인덱스 노드)가 존재하는 경우 그 노드에 
				//newIndexNode=pDataNode->getParent();
				pDataNode->getParent()->insertIndexMap(iter->first, newDataNode); // frequency와 가르키기 위한 새로 생긴 데이터 노드로 insert
			}
			else{
				newIndexNode=new BpTreeIndexNode;
				newIndexNode->insertIndexMap(iter->first, newDataNode); // indexNode insert
			}
			newDataNode->insertDataMap(iter->first, iter->second); // 새로 생기는 dataNode insert 시작
			v.push_back(iter->first);
		}
		else if(compareIndex>splitIndex){
			newDataNode->insertDataMap(iter->first, iter->second); // 새로운 노드에는 넣고
			v.push_back(iter->first);
		}
		compareIndex++;
	}

	for(int i=0; i<v.size(); i++){
		pDataNode->deleteMap(v[i]);
	}

	pDataNode->setNext(newDataNode); // pDataNode는 기존 노드
	if(pDataNode->getParent()==NULL){
		pDataNode->setParent(newIndexNode); // 기존 노드가 새로 생긴 indexNode를 parent로 함
		newIndexNode->setMostLeftChild(pDataNode); // 새로 생긴 indexNode가 기존의 dataNode를 가장 왼쪽 노드로 가르킴
	}
	newDataNode->setPrev(pDataNode); // newDataNode는 갈라져 나온 노드
	newDataNode->setParent(pDataNode->getParent()); // 갈라져 나온 노드가 새로 생긴 indexNode를 parent로 함
	
	// indexNode가 split 된 적 없고, dataNode 위 indexNode의 가장 왼쪽  dataNode의 이전 노드가 없으면 root로 설정
	if((splitIndexNodes==false) && (pDataNode->getParent()->getMostLeftChild()->getPrev()==NULL)){
		root=pDataNode->getParent();
	}

	return;
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
