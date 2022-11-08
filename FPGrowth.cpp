#include "FPGrowth.h"


FPGrowth::~FPGrowth() {

}

void FPGrowth::createFPtree(FPNode* root, HeaderTable* table, list<string> item_array, int frequency) {

	FPNode*curNode=root; // 현재 노드는 root
	map<string, FPNode*>::iterator mapiter;
	list<pair<int,string>>::iterator indexTableIter;
	list<string>::iterator item_array_Iter;

	while(item_array.empty()!=true){ // while item_array is not empty
		//for(item_array_Iter=item_array.begin(); item_array_Iter!=item_array.end(); item_array_Iter++){
			//if(item_array_Iter)
		//}

		//for(indexTableIter=table->getindexTable().begin(); indexTableIter!=table->getindexTable().end(); indexTableIter++){

		//}

		//if(table->getindexTable().find(item_array.front())->first)
		FPNode*findChildren = curNode->getChildrenNode(item_array.front());
		if(findChildren==NULL){  // if find the first item name in item_array, but it is not found
			FPNode*newNode=new FPNode;
			newNode->updateFrequency(frequency);
			newNode->setParent(curNode);
			curNode->pushchildren(item_array.front(), newNode); // push the children(map)
			
			if(table->getdataTable().find(item_array.front())!=table->getdataTable().end()){

			//list<string> temp_item_array=item_array;
			for(auto const &pair: table->getdataTable()){
				if(item_array.front()==pair.first){
						FPNode* temp=pair.second;
						if(temp->getNext()==NULL){
							temp->setNext(newNode);
						}
					}
			}
				// for(mapiter=table->getdataTable().begin(); mapiter!=table->getdataTable().end(); mapiter++){
				// 	//FPNode*temp=table->getdataTable().find(temp_item_array.front())->second;
				// 	if(item_array.front()==mapiter->first){
				// 		FPNode* temp=mapiter->second;
				// 		if(temp->getNext()==NULL){
				// 			temp->setNext(newNode);
				// 		}
				// 	}

				// }
			}

			list<string>::iterator iter=item_array.begin();
			
			item_array.pop_front(); // push after, pop front
			curNode=newNode;
			//cout<<*iter<<" "<<curNode->getFrequency()<<"->"; white meal이 이상하게 깨져서 나오는 cout들.
		}
		else{ // if alreay insert
			findChildren->updateFrequency(frequency); // update frequency
			list<string>::iterator iter=item_array.begin();
			
			item_array.pop_front(); // update after, pop front
			curNode=findChildren;
			//cout<<*iter<<" "<<curNode->getFrequency()<<"->";
		}
	}
	//cout<<endl;
	//cout<<"======"<<endl;
}

// connect with table and node
void FPGrowth::connectNode(HeaderTable* table, string item, FPNode* node) {
	
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
