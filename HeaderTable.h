#pragma once
#include "FPNode.h"
#include <list>


class HeaderTable
{
private:
	list<pair<int, string> > indexTable; // list with item frequency and item name(pair)
	map<string, FPNode*> dataTable; // map cotainer with item information
public:
	ofstream flog;
	HeaderTable() { 	flog.open("log.txt", ios::app);} // constructor
	~HeaderTable(); // destructor
	void insertTable(string item, int frequency);
	list<pair<int, string>> getindexTable() { return indexTable; }
	map<string, FPNode*> getdataTable() { return dataTable; }
	FPNode* getNode(string item) { return dataTable.find(item)->second; }
	void descendingIndexTable() { indexTable.sort(greater<pair<int, string>>()); }
	void ascendingIndexTable() { indexTable.sort(); }
	int find_frequency(string item);
	bool find_item(string item);
	void PRINT_ITEMLIST();
};


