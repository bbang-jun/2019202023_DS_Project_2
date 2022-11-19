#include "BpTree.h"
#include <vector>

bool BpTree::Insert(int frequency, set<string> itemset) // insert into B+-Tree
{ 
	if (itemset.size() <= 1) // if element size is zero or once, not save into the B+-Tree
	{ 
		return true;
	}

	if (root == NULL) // if root is NULL, make root node
	{ 
		BpTreeDataNode *dataNode = new BpTreeDataNode; // make first dataNode
		FrequentPatternNode *newFqptNode = new FrequentPatternNode; // make FrequentPatterNode for connnect with dataNode
		newFqptNode->InsertList(itemset); // save FrequentPatterNode's first argument
		newFqptNode->setFrequency(frequency); // save FrequentPatterNode's second argument
		dataNode->insertDataMap(frequency, newFqptNode); // make complete dataNode's data
		root = dataNode; // set the root is dataNode
	}
	else // if root is exist
	{
		if (searchDataNode(frequency) == NULL) // if dataNode is not exist, correspond that dataNode
		{ 
			BpTreeNode*curNode = root; // current Node is root

			while (curNode->getMostLeftChild() != NULL) // move curNode to dataNode
			{
				if(curNode->getMostLeftChild()==NULL)
					break;
				curNode=curNode->getMostLeftChild();
			}

			map<int, FrequentPatternNode *>::iterator iter; // declare iterator
			map<int, FrequentPatternNode *>::iterator cmpNextIter;

			while(curNode!=NULL){ // 해당 frequency를 가진 dataNode가 없을 때 삽입 진행하는 코드

				for(iter=curNode->getDataMap()->begin(); iter!=curNode->getDataMap()->end(); iter++){
					if(frequency>iter->first){ // 새로 들어온 frequency가 현재 dataNode의 frequency보다 큰 경우
						if(curNode->getNext()!=NULL){ // 다음 dataNode가 존재하는 경우에 다음 dataNode의 첫 번째의 frequency와 비교
							cmpNextIter=curNode->getNext()->getDataMap()->begin(); // 다음 dataNode의 첫 번째의 반복자
							if(frequency<cmpNextIter->first){ // 새로 들어온 frequency와 다음 dataNode의 첫 번째 frequency와 비교 (굳이 필요 없는 것 같은데....)
								FrequentPatternNode *newFqptNode = new FrequentPatternNode;
								newFqptNode->InsertList(itemset); // FrequentPatterNode의 1번째 인자와 2번째 인자 저장

								curNode->insertDataMap(frequency, newFqptNode); // 현재 dataNode에 새로운 frequency 삽입

								if (excessDataNode(curNode) == true)
								{
									splitDataNode(curNode);
									return true;
								}

								return true;
							}
						}
						else{ // curNode->getNext()==NULL 다음 dataNode가 존재하지 않으면 현재 dataNode에 삽입
							FrequentPatternNode *newFqptNode = new FrequentPatternNode;
							newFqptNode->InsertList(itemset); // FrequentPatterNode의 1번째 인자와 2번째 인자 저장

							curNode->insertDataMap(frequency, newFqptNode);

							if (excessDataNode(curNode) == true)
							{
								splitDataNode(curNode);
								return true;
							}

							return true;
						}
					}
					else if(frequency<iter->first){ // 현재 dataNode의 frequency보다 작으면 삽입
						FrequentPatternNode *newFqptNode = new FrequentPatternNode;
						newFqptNode->InsertList(itemset); // FrequentPatterNode의 1번째 인자와 2번째 인자 저장

						curNode->insertDataMap(frequency, newFqptNode);

						if (excessDataNode(curNode) == true)
						{
							splitDataNode(curNode);
							return true;
						}

						return true;
					}
				}

				if(curNode->getNext()==NULL){
					break;
				}

				curNode=curNode->getNext(); // 해당 dataNode에서 삽입되지 않았으면 다음 dataNode로 이동
			}

		}
		else
		{														// data 노드 목록 중 frequency와 일치하는 dataNode가 있는 경우
			BpTreeNode *searchData = searchDataNode(frequency); // 같은 frequency면 frequent patterNode에 삽입
			map<int, FrequentPatternNode *>::iterator iter;

			for (iter = searchData->getDataMap()->begin(); iter != searchData->getDataMap()->end(); iter++)
			{
				if (iter->first == frequency)
				{
					// searchData->getDataMap()
					iter->second->InsertList(itemset);
				}
			}
		}
	}

	return true;
}

BpTreeNode *BpTree::searchDataNode(int n)
{
	BpTreeNode *pCur = root;

	while (pCur != NULL) // 가장 왼쪽 dataNode까지 이동
	{
		if (pCur->getMostLeftChild() == NULL)
			break;
		pCur = pCur->getMostLeftChild();
	}

	map<int, FrequentPatternNode *>::iterator dataIter = pCur->getDataMap()->begin();


	while(1){
		for (dataIter = pCur->getDataMap()->begin(); dataIter != pCur->getDataMap()->end(); dataIter++)
		{
			if (dataIter->first == n)
			{
				return pCur;
			}
		}

		if(pCur->getNext()==NULL)
			break;
		pCur=pCur->getNext();
	}

	return NULL;
}

void BpTree::splitDataNode(BpTreeNode *pDataNode)
{

	BpTreeDataNode *newDataNode = new BpTreeDataNode;
	BpTreeIndexNode *newIndexNode=NULL;
	BpTreeNode*splitNewIndexNode=NULL;
	int splitIndex = ((order - 1) / 2.0) + 1;
	int compareIndex = 1;
	vector<int> v;
	bool splitIndexNodes = false;

	map<int, FrequentPatternNode *>::iterator iter;

	// 새로 생긴 dataNode에 split할 index 이후의 요소들 삽입
	for (iter = pDataNode->getDataMap()->begin(); iter != pDataNode->getDataMap()->end(); iter++)
	{
		if (compareIndex == splitIndex)
		{
			if (pDataNode->getParent() != NULL)
			{ // 스플릿할 노드의 부모(인덱스 노드)가 존재하는 경우 그 노드에
				// newIndexNode=pDataNode->getParent();
				pDataNode->getParent()->insertIndexMap(iter->first, newDataNode); // frequency와 가르키기 위한 새로 생긴 데이터 노드로 insert

				if (excessIndexNode(pDataNode->getParent()) == true) // indexNode가 order 개수가 되면 split
				{
					splitIndexNodes=true;
					splitIndexNode(pDataNode->getParent());
				}
			}
			else
			{
				newIndexNode = new BpTreeIndexNode;
				newIndexNode->insertIndexMap(iter->first, newDataNode); // indexNode insert
			}
			newDataNode->insertDataMap(iter->first, iter->second); // 새로 생기는 dataNode insert 시작
			v.push_back(iter->first);
		}
		else if (compareIndex > splitIndex)
		{
			newDataNode->insertDataMap(iter->first, iter->second); // 새로운 노드에는 넣고
			v.push_back(iter->first);
		}
		compareIndex++;
	}

	for (int i = 0; i < v.size(); i++)
	{
		pDataNode->deleteMap(v[i]);
	}
	newDataNode->setNext(pDataNode->getNext()); // 데이터 노드간 연결
	if(newDataNode->getNext()!=NULL){
		newDataNode->getNext()->setPrev(newDataNode);
	}
	pDataNode->setNext(newDataNode); // pDataNode는 기존 노드
	newDataNode->setPrev(pDataNode);	// newDataNode는 갈라져 나온 노드

	if ((newIndexNode!=NULL)&&(newDataNode->getParent() == NULL))
	{
		newIndexNode->setMostLeftChild(pDataNode);
		pDataNode->setParent(newIndexNode);
		newDataNode->setParent(newIndexNode);
	}
	else if(newIndexNode==NULL){
		newDataNode->setParent(pDataNode->getParent()); 
		pDataNode->setParent(newDataNode->getParent());
	}


	// indexNode가 split 된 적 없고, dataNode 위 indexNode의 가장 왼쪽  dataNode의 이전 노드가 없으면 root로 설정
	if ((splitIndexNodes == false) && (pDataNode->getParent()->getMostLeftChild()->getPrev() == NULL))
	{
		root = pDataNode->getParent();
	}


	return;
}

void BpTree::splitIndexNode(BpTreeNode *pIndexNode)
{
	BpTreeIndexNode *newIndexNode=new BpTreeIndexNode;
	BpTreeNode *newnewIndexNode=NULL;
	BpTreeNode *curNode=pIndexNode->getParent();
	int splitIndex = ceil((order - 1) / 2.0) + 1;
	int compareIndex = 1;
	vector<int> v;

	map<int, BpTreeNode *>::iterator iter;

	// 새로 생긴 dataNode에 split할 index 이후의 요소들 삽입
	for (iter = pIndexNode->getIndexMap()->begin(); iter != pIndexNode->getIndexMap()->end(); iter++)
	{
		if (compareIndex == splitIndex)
		{
			if (curNode != NULL)
			{ // 스플릿할 노드의 부모(인덱스 노드)가 존재하는 경우 그 노드에
				// newIndexNode=pDataNode->getParent();
				v.push_back(iter->first);
				curNode->insertIndexMap(iter->first, newIndexNode); // frequency와 가르키기 위한 새로 생긴 데이터 노드로 insert
				newIndexNode->setParent(pIndexNode->getParent());
				newIndexNode->setMostLeftChild(iter->second);
				newIndexNode->getMostLeftChild()->setParent(newIndexNode);
			}
			else
			{
				v.push_back(iter->first);
				newnewIndexNode = new BpTreeIndexNode;
				newnewIndexNode->insertIndexMap(iter->first, newIndexNode); // indexNode insert
				curNode=newnewIndexNode;
				newnewIndexNode->setMostLeftChild(pIndexNode);
				pIndexNode->setParent(newnewIndexNode);
				newIndexNode->setParent(newnewIndexNode);
				iter->second->setParent(newIndexNode);
				newIndexNode->setMostLeftChild(iter->second);
				int k=0;
			}
		}
		else if (compareIndex > splitIndex)
		{
			newIndexNode->insertIndexMap(iter->first, iter->second); // 새로운 노드에는 넣고
			iter->second->setParent(newIndexNode);
			v.push_back(iter->first);
		}
		compareIndex++;
	}

	for (int i = 0; i < v.size(); i++)
	{
		pIndexNode->deleteMap(v[i]);
	}
	
	

	// indexNode가 split 된 적 없고, dataNode 위 indexNode의 가장 왼쪽  dataNode의 이전 노드가 없으면 root로 설정
	if(newnewIndexNode!=NULL){
		if(newnewIndexNode->getMostLeftChild()!=NULL){
		BpTreeNode* temp = newnewIndexNode;
		while(1){
			if(temp->getMostLeftChild()==NULL){
				if(temp->getPrev()==NULL){
					root=newnewIndexNode;
					break;
				}
			}
		temp=temp->getMostLeftChild();
		}
	}
	}
	
	

	if(excessIndexNode(curNode)==true){
		splitIndexNode(curNode);
	}
	
	return;
}

bool BpTree::excessDataNode(BpTreeNode *pDataNode)
{
	if (pDataNode->getDataMap()->size() > order - 1)
		return true; // order is equal to the number of elements
	else
		return false;
}

bool BpTree::excessIndexNode(BpTreeNode *pIndexNode)
{
	if (pIndexNode->getIndexMap()->size() > order - 1)
		return true; // order is equal to the number of elements
	else
		return false;
}

bool BpTree::printConfidence(string item, double item_frequency, double min_confidence)
{
	flog.open("log.txt", ios::app);

	BpTreeNode* moveNode=root;
	int i=0;

	flog<<"========PRINT_CONFIDENCE========"<<endl;
	
	
	map<int, FrequentPatternNode*>::iterator iter;
	multimap<int, set<string> >::iterator frequentIter;
	set<string>::iterator stringIter;

	while (moveNode->getMostLeftChild() != NULL) { //go down til we meet data node (not index node)
		moveNode = moveNode->getMostLeftChild();
	}

	double confidence;

	while (moveNode != NULL) {
		map<int, FrequentPatternNode*> data1 = *moveNode->getDataMap();
		for (iter=data1.begin(); iter != data1.end(); iter++) {
			multimap<int, set<string>> data2 = iter->second->getList();
			for(frequentIter=data2.begin(); frequentIter!=data2.end(); frequentIter++){
				set<string> data3 = frequentIter->second;
				for(stringIter=data3.begin(); stringIter!=data3.end(); stringIter++){
					if((*stringIter==item)){
						confidence=iter->first/item_frequency;
						if(confidence>=min_confidence){
							i++;
							if(i==1)
							flog<<"FrequentPattern Frequency	Confidence"<<endl;
							flog<<"{";
							for(stringIter=data3.begin(); stringIter!=data3.end(); stringIter++){
								if((++stringIter)--==data3.end()){
									flog<<*stringIter;
									break;
								}
					
							flog <<*stringIter<<", ";
							}
							flog<<"} "<<iter->first<<" "<<confidence <<endl;
						}
					}
				}
			}
		}
		moveNode = moveNode->getNext(); //move moveNode to next
	}

	if(i==0) // there is nothing to print Frequent Pattern
		flog<<"ERROR 600"<<endl; 

	flog<<"================================="<<endl<<endl;
	return true;
}

bool BpTree::printRange(string item, int min, int max)
{

	return true;
}
void BpTree::printFrequentPatterns(set<string> pFrequentPattern) {
	//*fout << "{";
	cout<<"{";
	set<string> curPattern = pFrequentPattern;
	for (set<string>::iterator it = curPattern.begin(); it != curPattern.end();) {
		string temp = *it++;
		//*fout << temp;
		cout<<temp;
		if (it == curPattern.end()) {
			//*fout << "} ";
			cout<<"} ";
			break;
		}
		//*fout << ", ";
		cout<<", ";
	}
}
