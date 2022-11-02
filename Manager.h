#pragma once
#include "FPGrowth.h"
#include "BpTree.h"

class Manager
{
private:
	char* cmd;
	FPGrowth* fpgrowth; // FP-Growth
	BpTree* bptree; // B+-tree
public:
	Manager(int threshold, int bpOrder)	//constructor
	{
		/* You must fill here */
	}


	~Manager()//destructor
	{
		/* You must fill here */
	}

	ifstream fin;
	ofstream flog;
	

	void run(const char* command);
	bool LOAD();
	bool BTLOAD();
	
	bool PRINT_ITEMLIST();
	bool PRINT_FPTREE();

	bool PRINT_RANGE(char* item, int start, int end);
	bool PRINT_CONFIDENCE(char* item, double rate);
	bool PRINT_BPTREE(char* item, int min_frequency);


	void printErrorCode(int n);
	void printSuccessCode();

};

