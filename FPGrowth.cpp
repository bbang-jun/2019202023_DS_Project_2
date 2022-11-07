#include "FPGrowth.h"


FPGrowth::~FPGrowth() {

}

void FPGrowth::createFPtree(FPNode* root, HeaderTable* table, list<string> item_array, int frequency) {
	//table->getdataTable().insert({});





	FPNode*curNode=root;

	while(item_array.empty()!=true){ // while item_array is not empty
		FPNode*findChildren = curNode->getChildrenNode(item_array.front());
		if(findChildren==NULL){  // if find the first item name in item_array, but it is not found
			FPNode*newNode=new FPNode;
			newNode->updateFrequency(frequency);
			newNode->setParent(curNode);
			curNode->pushchildren(item_array.front(), newNode); // push the children(map)

			list<string>::iterator iter=item_array.begin();
			
			item_array.pop_front(); // push after, pop front
			curNode=newNode;
			cout<<*iter<<" "<<curNode->getFrequency()<<"->";
		}
		else{ // if alreay insert
			findChildren->updateFrequency(frequency); // update frequency
			list<string>::iterator iter=item_array.begin();
			
			item_array.pop_front(); // update after, pop front
			curNode=findChildren;
			cout<<*iter<<" "<<curNode->getFrequency()<<"->";
		}
	}
	cout<<endl;
	cout<<"======"<<endl;

	// if(curNode->getChildrenNode(item_array.front())==NULL){

	// }
	// else{

	// }

	// while(item_array.empty()!=true){
	// 	curNode->pushchildren(item_array.front(), newNode);
	// 	item_array.pop_front();
	// }

	// for(iter=item_array.begin(); iter!=item_array.end(); iter++){
		
	// }
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
