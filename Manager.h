#pragma once
#include "FPGrowth.h"
#include "BpTree.h"
#include <string>
#include <string.h>
class Manager
{
private:
	char* cmd;
	FPGrowth* fpgrowth; // FP-Growth
	BpTree* bptree; // B+-tree
public:

	ofstream* fout;
	ofstream flog;
	ifstream fin;
	Manager(int threshold, int bpOrder)	//constructor
	{
		/* You must fill here */
		fpgrowth = new FPGrowth(fout, threshold);
		bptree = new BpTree(fout, bpOrder);
	}


	~Manager()//destructor
	{
		/* You must fill here */
	}

	

	void run(const char* command);
	bool LOAD();
	bool BTLOAD();
	
	bool PRINT_ITEMLIST();
	bool PRINT_FPTREE();

	bool PRINT_RANGE(char* item, int start, int end);
	bool PRINT_CONFIDENCE(char* item, double rate);
	bool PRINT_BPTREE(char* item, int min_frequency);
	//bool PRINT_BPTREE();


	void printErrorCode(string command, int code);
	void printSuccessCode();

	bool SAVE();

};

