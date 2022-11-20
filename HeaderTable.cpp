#include "HeaderTable.h"

HeaderTable::~HeaderTable() {} // destructor

void HeaderTable::insertTable(string item, int frequency) { // make index table(insert)

	if (frequency == 0){ // if some item is first inserted
		indexTable.push_back(make_pair(frequency+1, item)); // insert in indexTable
	} 
	else
	{
		int i=0; // declare i to compare
		int indexTableSize = indexTable.size(); // get indexTable's size
		list<pair<int, string>> tempIndexTable=indexTable; // variale of indexTable
		pair<int, string> tempPair; // declare temPair

		while(i!=indexTableSize){ // repeat while indexTableSize
			tempPair=tempIndexTable.front(); // temppair is the front pair of tempindex
			tempIndexTable.pop_front(); // pop front the tempindex
			if (tempPair.second == item) // if temppair's second is same with item
				tempPair.first+=1; // add frequency
			tempIndexTable.push_back(make_pair(tempPair.first, tempPair.second)); // push the temppair into tempindex
			i++;
		}

		indexTable=tempIndexTable; // save tempIndexTable to indexTable
		tempIndexTable.clear(); // clear the tempIndexTable
	}

	return;
}

void HeaderTable::makeThIndexTable(int threshold){ // make same or bigger threshold index table
	int i=0; // declare i to compare
	int indexTableSize = indexTable.size(); // get indexTable's size
	list<pair<int, string>> tempIndexTable=indexTable;
	pair<int, string> tempPair; // declare temPair

	while(i!=indexTableSize){ // repeat while indexTableSize
		tempPair=tempIndexTable.front(); // temppair is the front pair of tempindex
		tempIndexTable.pop_front(); // pop front the tempindex
		if(tempPair.first>=threshold){ // if satifying same or bigger than threshold
			thIndexTable.push_back(make_pair(tempPair.first, tempPair.second)); // insert in thIndexTable
		}
		i++; // increment of i
	}
	tempIndexTable.clear(); // clear the tempIndexTable

	return;
}

void HeaderTable:: makeDataTable(){ // make Data table

	int i=0; // declare i to compare
	int ThIndexTableSize = thIndexTable.size(); // get thIndexTable's size
	list<pair<int, string>> tempIndexTable=thIndexTable;
	pair<int, string> tempPair; // declare temPair

	while(i!=ThIndexTableSize){ // repeat while ThIndexTableSize
		FPNode* Pointer = new FPNode; // make new node
		tempPair=tempIndexTable.front(); // temppair is the front pair of tempindex
		tempIndexTable.pop_front(); //pop the tempindex
		dataTable.insert(make_pair(tempPair.second, Pointer)); // insert in dataTable
		i++; // increment of i
	}

	return;
}

int HeaderTable::find_frequency(string item){ // find certain item's frequency
	int frequency = 0; // declare frequency for save
	int i=0; // declare i to compare
	int indexTableSize = indexTable.size(); // get indexTableSize
	list<pair <int, string>> tempIndexTable=indexTable;
	pair<int, string> tempPair; // declare temPair

	while(i!=indexTableSize){ // repeat while indexTableSize
		tempPair=tempIndexTable.front();
		if (tempPair.second == item) // if find same name item in tempIndexTable
		{
			frequency = tempPair.first; // save freuqncy
			break;
		}
		else 
			tempIndexTable.pop_front(); // pop first element in tempIndexTable
		i++; // increment of i
	}

	return frequency; // return certain item's frequency
}

bool HeaderTable::find_item(string item){ // find the certain item in indexTable
	list<pair<int, string>>::iterator iter; // declare iterator
	for(auto iter= indexTable.begin(); iter!=indexTable.end(); iter++){ // repeat
		if(item==iter->second){ // if find item in indexTable
			return true; 
		}
	}
	return false;
}

void HeaderTable::PRINT_ITEMLIST(){ // print index table
	list<pair<int, string>>::iterator iter; // declare iterator
	for(auto iter= indexTable.begin(); iter!=indexTable.end(); iter++){ // repeat
		flog<<iter->second<<" "<<iter->first<<endl; // print all item name and frequency in index table
	}

	return;
}
