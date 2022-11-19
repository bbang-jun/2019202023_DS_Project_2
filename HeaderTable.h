#pragma once
#include "FPNode.h"
#include <list>


class HeaderTable
{
private:
	list<pair<int, string>> indexTable; // list with item frequency and item name(pair)
	list<pair<int, string>> thIndexTable; // list with same or bigger frequency and item name(pair)
	map<string, FPNode*> dataTable; // map cotainer with item information
public:
	ofstream flog;
	HeaderTable() { 	flog.open("log.txt", ios::app);} // constructor
	~HeaderTable(); // destructor
	void insertTable(string item, int frequency); // make index table(insert)
	void makeThIndexTable(int threshold); // make same or bigger threshold index table
	void makeDataTable(); // make Data table
	list<pair<int, string>> getindexTable() { return indexTable; } // return the indexTable
	list<pair<int, string>> getThIndexTable() { return thIndexTable; } // return the thIndexTable(same or bigger frequency)
	map<string, FPNode*> getdataTable() { return dataTable; } // return the dataTable
	FPNode* getNode(string item) { return dataTable.find(item)->second; } // return certain item's node
	void descendingIndexTable() { indexTable.sort(greater<pair<int, string>>()); thIndexTable.sort(greater<pair<int, string>>());} // sorting desceinding frequency index table
	void ascendingIndexTable() { indexTable.sort(); thIndexTable.sort(); } // sorting ascending frequency index table
	void descendingThIndexTable() { thIndexTable.sort(greater<pair<int, string>>()); } // sorting desceinding frequency ThIndex table
	void ascendingThIndexTable() { thIndexTable.sort(); } // sorting ascending frequency ThIndex table
	int find_frequency(string item); // find certain item's frequency
	bool find_item(string item); // find certain item
	void PRINT_ITEMLIST(); // for PRINT_ITEMLIST command
};


