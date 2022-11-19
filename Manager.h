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

	ofstream* fout; // fout
	ofstream flog; // flog
	ifstream fin; // fin

	Manager(int threshold, int bpOrder)	// constructor
	{
		fpgrowth = new FPGrowth(fout, threshold); // dynamic allocate fpgrowth
		bptree = new BpTree(fout, bpOrder); // dynamic allocate bptree
	}

	~Manager(){	//destructor
	delete fpgrowth; // delete dynamic allocate fpgrowth
	delete bptree; // delete dynamic allocate bptree
	}

	void run(const char* command);
	bool LOAD(); // LOAD command
	bool BTLOAD(); // BTLOAD command
	bool PRINT_ITEMLIST(); // PRINT_ITEMLIST command
	bool PRINT_FPTREE(); // PRINT_FPTREE command
	bool PRINT_RANGE(char* item, int start, int end); // PRINT_RANGE command
	bool PRINT_CONFIDENCE(char* item, double rate); // PRINT_CONFIDENCE command
	bool PRINT_BPTREE(char* item, int min_frequency); // PRINT_BPTREE command
	void printErrorCode(string command, int code); // print of error code in command
	void printSuccessCode(); // print of success code in command
	bool SAVE(); // SAVE command
};

