#include "BpTree.h"
#include <vector>

bool BpTree::Insert(int frequency, set<string> itemset) // insert into B+-Tree
{ 
	if (itemset.size() <= 1) // if element size is zero or once, not save into the B+-Tree
	{ 
		return true; // return
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
				if(curNode->getMostLeftChild()==NULL) // if no mor most left childe
					break; // break
				curNode=curNode->getMostLeftChild(); // set the curNode to curNode's getMostLeftChild()
			}

			map<int, FrequentPatternNode *>::iterator iter; // declare iterator
			map<int, FrequentPatternNode *>::iterator cmpNextIter; // declare iterator

			while(curNode!=NULL){ // repeat while curNode is not NULL
				for(iter=curNode->getDataMap()->begin(); iter!=curNode->getDataMap()->end(); iter++){ // get data from dataNode
					if(frequency>iter->first){ // if newly inserted frequency is bigger than current dataNode's frequency
						if(curNode->getNext()!=NULL){ // if next dataNode is exist, compare with next dataNode's first frequency
							cmpNextIter=curNode->getNext()->getDataMap()->begin(); // next dataNode's first iterator
							if(frequency<cmpNextIter->first){ // compare with newrly frequency and next dataNode's first frequency
								FrequentPatternNode *newFqptNode = new FrequentPatternNode;
								newFqptNode->InsertList(itemset); // save first argument and second argument in FrequentPatterNode

								curNode->insertDataMap(frequency, newFqptNode); // insert new frequency into the current dataNode

								if (excessDataNode(curNode) == true) // judge need to split
								{
									splitDataNode(curNode); // split data node
									return true;
								}
								return true;
							}
						}
						else{ // curNode->getNext()==NULL
							FrequentPatternNode *newFqptNode = new FrequentPatternNode;
							newFqptNode->InsertList(itemset); // save first argument and second argument in FrequentPatterNode

							curNode->insertDataMap(frequency, newFqptNode); // insert new frequency into the current dataNode

							if (excessDataNode(curNode) == true) // judge need to split
							{
								splitDataNode(curNode); // split data node
								return true;
							}
							return true;
						}
					}
					else if(frequency<iter->first){ // if less than current dataNode's frequency
						FrequentPatternNode *newFqptNode = new FrequentPatternNode;
						newFqptNode->InsertList(itemset); // save first argument and second argument in FrequentPatterNode

						curNode->insertDataMap(frequency, newFqptNode); // insert new frequency into the current dataNode

						if (excessDataNode(curNode) == true) // judge need to split
						{
							splitDataNode(curNode); // split data node
							return true;
						}

						return true;
					}
				}

				if(curNode->getNext()==NULL){ // if curNode's next node is NULL
					break;
				}

				curNode=curNode->getNext(); // if not inserted current dataNode. move to next dataNode
			}

		}
		else // if dataNode is already exist
		{														
			BpTreeNode *searchData = searchDataNode(frequency);  // get the search DataNode
			map<int, FrequentPatternNode *>::iterator iter; // declare iterator

			for (iter = searchData->getDataMap()->begin(); iter != searchData->getDataMap()->end(); iter++)
			{
				if (iter->first == frequency) // if same frequency
					iter->second->InsertList(itemset); // insert list
			}
		}
	}

	return true;
}

BpTreeNode *BpTree::searchDataNode(int n) // serarch data node and return
{
	BpTreeNode *pCur = root; // initialize

	while (pCur != NULL) // move to most left child
	{
		if (pCur->getMostLeftChild() == NULL)
			break;
		pCur = pCur->getMostLeftChild();
	}

	map<int, FrequentPatternNode *>::iterator dataIter = pCur->getDataMap()->begin();

	while(true){
		for (dataIter = pCur->getDataMap()->begin(); dataIter != pCur->getDataMap()->end(); dataIter++)
		{
			if (dataIter->first == n) // data node's frequency is same with n
			{
				return pCur; // return pCur
			}
		}

		if(pCur->getNext()==NULL) // if no more next dataNode
			break; // break
		pCur=pCur->getNext(); // move next dataNode
	}

	return NULL;
}

void BpTree::splitDataNode(BpTreeNode *pDataNode) // split data Node
{
	BpTreeDataNode *newDataNode = new BpTreeDataNode;
	BpTreeIndexNode *newIndexNode=NULL;
	BpTreeNode*splitNewIndexNode=NULL;
	int splitIndex = ceil((order - 1) / 2.0) + 1; // get the index to split
	int compareIndex = 1;
	vector<int> v;
	bool splitIndexNodes = false;

	map<int, FrequentPatternNode *>::iterator iter; // declare iterator

	// insert data to newDataNode 
	for (iter = pDataNode->getDataMap()->begin(); iter != pDataNode->getDataMap()->end(); iter++)
	{
		if (compareIndex == splitIndex)
		{
			if (pDataNode->getParent() != NULL)
			{ // if split node's parent node(index node) is already exist
				pDataNode->getParent()->insertIndexMap(iter->first, newDataNode); // insert

				if (excessIndexNode(pDataNode->getParent()) == true) // if indexNode is same with order num, split
				{
					splitIndexNodes=true;
					splitIndexNode(pDataNode->getParent()); // split index node
				}
			}
			else
			{
				newIndexNode = new BpTreeIndexNode;
				newIndexNode->insertIndexMap(iter->first, newDataNode); // indexNode insert
			}
			newDataNode->insertDataMap(iter->first, iter->second); // insert the data to newDataNode
			v.push_back(iter->first);
		}
		else if (compareIndex > splitIndex)
		{
			newDataNode->insertDataMap(iter->first, iter->second); // insert in newDataNode
			v.push_back(iter->first); // insert in vector to delete data
		}
		compareIndex++;
	}

	for (int i = 0; i < v.size(); i++) // delete data in pDataNode
	{
		pDataNode->deleteMap(v[i]);
	}
	newDataNode->setNext(pDataNode->getNext()); // connect with pDataNode and newDataNode
	if(newDataNode->getNext()!=NULL){
		newDataNode->getNext()->setPrev(newDataNode); // setPrev
	}
	pDataNode->setNext(newDataNode); // pDataNode is already exist node
	newDataNode->setPrev(pDataNode);	// newDataNode is newly make node

	if ((newIndexNode!=NULL)&&(newDataNode->getParent() == NULL)) // if indexNode is newly making
	{
		newIndexNode->setMostLeftChild(pDataNode); // newIndexNode's most left child is pDataNode
		pDataNode->setParent(newIndexNode); // set parent
		newDataNode->setParent(newIndexNode); // set parent
	}
	else if(newIndexNode==NULL){ // if indexNode(pDataNode's parent) is already exist
		newDataNode->setParent(pDataNode->getParent()); // set parent
		pDataNode->setParent(newDataNode->getParent()); // set parent
	}

	// if indexNode is not split and dataNode's up indexNode's most left dataNode's prev node is not exist, set the root
	if ((splitIndexNodes == false) && (pDataNode->getParent()->getMostLeftChild()->getPrev() == NULL))
		root = pDataNode->getParent(); // settting the root

	return;
}

void BpTree::splitIndexNode(BpTreeNode *pIndexNode) // split index node
{
	BpTreeIndexNode *newIndexNode=new BpTreeIndexNode;
	BpTreeNode *newnewIndexNode=NULL;
	BpTreeNode *curNode=pIndexNode->getParent();
	int splitIndex = ceil((order - 1) / 2.0) + 1; // get the index to split
	int compareIndex = 1;
	vector<int> v;

	map<int, BpTreeNode *>::iterator iter; // declare iterator

	// insert data to newIndexNode 
	for (iter = pIndexNode->getIndexMap()->begin(); iter != pIndexNode->getIndexMap()->end(); iter++)
	{
		if (compareIndex == splitIndex)
		{
			if (curNode != NULL) // if split node's parent(index Node) is already exist
			{ 
				v.push_back(iter->first); 
				curNode->insertIndexMap(iter->first, newIndexNode); // insert the data to curNode
				newIndexNode->setParent(pIndexNode->getParent()); // set parent
				newIndexNode->setMostLeftChild(iter->second); // set most left child
				newIndexNode->getMostLeftChild()->setParent(newIndexNode); // set parent
			}
			else
			{
				v.push_back(iter->first);
				newnewIndexNode = new BpTreeIndexNode; // newly making index node's up
				newnewIndexNode->insertIndexMap(iter->first, newIndexNode); // indexNode insert
				curNode=newnewIndexNode;
				newnewIndexNode->setMostLeftChild(pIndexNode); // set most left child
				pIndexNode->setParent(newnewIndexNode); // set parent
				newIndexNode->setParent(newnewIndexNode); // set parent
				iter->second->setParent(newIndexNode); // set parent
				newIndexNode->setMostLeftChild(iter->second); // set most left child
			}
		}
		else if (compareIndex > splitIndex)
		{
			newIndexNode->insertIndexMap(iter->first, iter->second); // insert data in newIndexNode
			iter->second->setParent(newIndexNode); // set parent
			v.push_back(iter->first);
		}
		compareIndex++;
	}

	for (int i = 0; i < v.size(); i++)
	{
		pIndexNode->deleteMap(v[i]); // delete data in pIndexNode
	}
	
	// if making newly up indexNode, set the root
	if(newnewIndexNode!=NULL){
		if(newnewIndexNode->getMostLeftChild()!=NULL){
			BpTreeNode* temp = newnewIndexNode;
			while(true){
				if(temp->getMostLeftChild()==NULL){
					if(temp->getPrev()==NULL){ // if prev is null 
						root=newnewIndexNode; // set the root
						break;
					}
				}
			temp=temp->getMostLeftChild(); // get most left child
			}
		}
	}
	
	if(excessIndexNode(curNode)==true){ // recursive split
		splitIndexNode(curNode); // split index Node
	}
	
	return;
}

bool BpTree::excessDataNode(BpTreeNode *pDataNode) // judge data node split
{
	if (pDataNode->getDataMap()->size() > order - 1)
		return true; // order is equal to the number of elements
	else
		return false;
}

bool BpTree::excessIndexNode(BpTreeNode *pIndexNode) // judge index node split
{
	if (pIndexNode->getIndexMap()->size() > order - 1)
		return true; // order is equal to the number of elements
	else
		return false;
}

bool BpTree::printConfidence(string item, double item_frequency, double min_confidence) // print confidence in frequent pattern
{
	flog.open("log.txt", ios::app);

	BpTreeNode* moveNode=root; // get the root
	int i=0;

	flog<<"========PRINT_CONFIDENCE========"<<endl;
	
	map<int, FrequentPatternNode*>::iterator iter; // declare iterator
	multimap<int, set<string> >::iterator frequentIter; // declare iterator
	set<string>::iterator stringIter; // declare iterator

	while (moveNode->getMostLeftChild() != NULL) { //go down til we meet data node (not index node)
		moveNode = moveNode->getMostLeftChild();
	}

	double confidence; // save for calculated confidence

	while (moveNode != NULL) { // while next data node is not NULL
		map<int, FrequentPatternNode*> iterData = *moveNode->getDataMap();
		for (iter=iterData.begin(); iter != iterData.end(); iter++) {
			multimap<int, set<string>> fqData = iter->second->getList();
			for(frequentIter=fqData.begin(); frequentIter!=fqData.end(); frequentIter++){
				set<string> stringData = frequentIter->second;
				for(stringIter=stringData.begin(); stringIter!=stringData.end(); stringIter++){
					if((*stringIter==item)){
						confidence=iter->first/item_frequency; // calculate confidence
						if(confidence>=min_confidence){ // same or bigger
							i++;
							if(i==1) // judge first print
							flog<<"FrequentPattern Frequency Confidence"<<endl;
							flog<<"{";
							for(stringIter=stringData.begin(); stringIter!=stringData.end(); stringIter++){
								if((++stringIter)--==stringData.end()){ // end
									flog<<*stringIter; // print item
									break;
								}
					
							flog <<*stringIter<<", "; // print item
							}
							flog<<"} "<<iter->first<<" "<<confidence <<endl; // print confidence
						}
					}
				}
			}
		}
		moveNode = moveNode->getNext(); // move moveNode to next
	}

	if(i==0) // there is nothing to print Frequent Pattern
		flog<<"ERROR 600"<<endl; 

	flog<<"================================="<<endl<<endl;
	return true;
}

bool BpTree::printRange(string item, int min, int max)
{
	// implement in Manager.cpp line 416~514
	return true;
}

void BpTree::printFrequentPatterns(set<string> pFrequentPattern) { // print frequent patterns
	*fout << "{";
	set<string> curPattern = pFrequentPattern;
	for (set<string>::iterator it = curPattern.begin(); it != curPattern.end();) { // repeat while meat end
		string temp = *it++;
		*fout << temp; // print
		if (it == curPattern.end()) {
			*fout << "} "; // print
			break;
		}
		*fout << ", "; // print
	}
}
