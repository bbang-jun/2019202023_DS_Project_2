#include "HeaderTable.h"

HeaderTable::~HeaderTable() {

}

void HeaderTable::insertTable(string item, int frequency) {

	if (frequency == 0){
		indexTable.push_back(make_pair(frequency+1, item));
	} 
	else
	{
		int i=0;
		int indexTableSize = indexTable.size();
		list<pair<int, string>> tempIndexTable;
		tempIndexTable=indexTable;
		pair<int, string> tempPair;

		while(i!=indexTableSize){
			tempPair=tempIndexTable.front(); //temppair is the front pair of tempindex
			tempIndexTable.pop_front(); //pop the tempindex
			if (tempPair.second == item) //if temppair's second is same with item
				tempPair.first+=1; //add frequency
			tempIndexTable.push_back(make_pair(tempPair.first, tempPair.second)); //push the temppair into tempindex
			i++;
		}

		indexTable=tempIndexTable;
		tempIndexTable.clear();
	}
}

void HeaderTable::makeThIndexTable(int threshold){
	int i=0;
	int indexTableSize = indexTable.size();
	list<pair<int, string>> tempIndexTable;
	tempIndexTable=indexTable;
	pair<int, string> tempPair;

	while(i!=indexTableSize){
		tempPair=tempIndexTable.front(); //temppair is the front pair of tempindex
		tempIndexTable.pop_front(); //pop the tempindex
		if(tempPair.first>=threshold){
			thIndexTable.push_back(make_pair(tempPair.first, tempPair.second));
		}
		i++;
	}
	tempIndexTable.clear();
}

void HeaderTable:: makeDataTable(){

	FPNode* Pointer = new FPNode;
	int i=0;
	int ThIndexTableSize = thIndexTable.size();
	list<pair<int, string>> tempIndexTable;
	tempIndexTable=thIndexTable;
	pair<int, string> tempPair;

	while(i!=ThIndexTableSize){
		tempPair=tempIndexTable.front(); //temppair is the front pair of tempindex
		tempIndexTable.pop_front(); //pop the tempindex
		dataTable.insert(make_pair(tempPair.second, Pointer));
		 // map<string, FPNode*>
		i++;
	}
}

int HeaderTable::find_frequency(string item){
	int frequency = 0;
	int i=0;
	int indexTableSize = indexTable.size();
	list<pair <int, string>> tempIndexTable;
	tempIndexTable=indexTable;
	pair<int, string> tempPair;

	while(i!=indexTableSize){
		tempPair=tempIndexTable.front();
		if (tempPair.second == item) 
		{
			frequency = tempPair.first; 
			
			break;
		}
		else 
			tempIndexTable.pop_front(); 
		i++;
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

void HeaderTable::printThresholdTable(){
		list<pair<int, string>>::iterator iter;
		for(auto iter= thIndexTable.begin(); iter!=thIndexTable.end(); iter++){
			flog<<iter->first<<" "<<iter->second<<endl;
		}
}

void HeaderTable::first(){
		map<string, FPNode*>::iterator iter;
		for(auto iter= dataTable.begin(); iter!=dataTable.end(); iter++){
			flog<<iter->first<<" "<<endl;
		}//iter->second->getNext()->getFrequency()<<
}
