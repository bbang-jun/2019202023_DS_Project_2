#pragma once
#include "FPNode.h"
#include <list>


class HeaderTable
{
private:
	list<pair<int, string>> indexTable; // list with item frequency and item name(pair)
	list<pair<int, string>> thIndexTable;
	map<string, FPNode*> dataTable; // map cotainer with item information
public:
	ofstream flog;
	HeaderTable() { 	flog.open("log.txt", ios::app);} // constructor
	~HeaderTable(); // destructor
	void insertTable(string item, int frequency);
	void makeThIndexTable(int threshold);
	void makeDataTable();
	list<pair<int, string>> getindexTable() { return indexTable; }
	list<pair<int, string>> getThIndexTable() { return thIndexTable; }
	map<string, FPNode*> getdataTable() { return dataTable; }
	FPNode* getNode(string item) { return dataTable.find(item)->second; }
	void descendingIndexTable() { indexTable.sort(greater<pair<int, string>>()); } // frequency 내림차순 정렬
	void ascendingIndexTable() { indexTable.sort(); }
	void descendingThIndexTable() { thIndexTable.sort(greater<pair<int, string>>()); } // frequency 내림차순 정렬
	void ascendingThIndexTable() { thIndexTable.sort(); }
	int find_frequency(string item);
	bool find_item(string item);
	void PRINT_ITEMLIST();
	void first();
	void printThresholdTable();
};


