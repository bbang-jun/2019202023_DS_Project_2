#include "HeaderTable.h"

HeaderTable::~HeaderTable() {

}

void HeaderTable::insertTable(string item, int frequency) {

	if (frequency == 0){
		indexTable.push_back(make_pair(frequency+1, item));
	} 
	else
	{
		int i;
		int indexTableSize = indexTable.size();
		list<pair<int, string>> tempIndexTable;
		tempIndexTable=indexTable;
		pair<int, string> tempPair;

		for (i = 0; i < indexTableSize; i++)
		{
			tempPair=tempIndexTable.front(); //temppair is the front pair of tempindex
			tempIndexTable.pop_front(); //pop the tempindex
			if (tempPair.second == item) //if temppair's second is same with item
				tempPair.first+=1; //add frequency
			tempIndexTable.push_back(make_pair(tempPair.first, tempPair.second)); //push the temppair into tempindex
		}
		indexTable=tempIndexTable;
	}
	//FPNode* pointerNode = new FPNode;

	// dataTable.insert(make_pair(item, pointerNode)); //insert the pair of str and pointerNode to dataTable
}

int HeaderTable::find_frequency(string item){
	int frequency = 0;
	int i;
	int indexTableSize = indexTable.size();
	list<pair <int, string>> tempIndexTable;
	tempIndexTable=indexTable;
	pair<int, string> tempPair;

	for (i = 0; i < indexTableSize; i++)
	{
		tempPair=tempIndexTable.front();
		if (tempPair.second == item) 
		{
			frequency = tempPair.first; 
			
			break;
		}
		else 
			tempIndexTable.pop_front(); 
	}

	return frequency; 
}

bool HeaderTable::find_item(string item){
	list<pair<int, string>>::iterator iter;

	for(auto iter= indexTable.begin(); iter!=indexTable.end(); iter++){
		if(item==iter->second){
			return true;
		}
	}
	return false;
}

void HeaderTable::PRINT_ITEMLIST(){
		list<pair<int, string>>::iterator iter;
		for(auto iter= indexTable.begin(); iter!=indexTable.end(); iter++){
			flog<<iter->first<<" "<<iter->second<<endl;
		}
}