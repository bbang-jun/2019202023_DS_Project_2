#include "Manager.h"

void Manager::run(const char* command)
{
	string commandFromtxt;
	fin.open(command);
	flog.open("log.txt", ios::app);
	if(!fin)
	{
		flog << "File Open Error" << endl;
		return;
	}
	while (!fin.eof())
	{
		getline(fin, commandFromtxt, '\n');

		if(commandFromtxt=="LOAD"){
			LOAD();
		}
		else if(commandFromtxt=="BTLOAD"){
			BTLOAD();
		}
		else if(commandFromtxt=="PRINT_ITEMLIST"){
			PRINT_ITEMLIST();
		}
		else if(commandFromtxt=="PRINT_FPTREE"){
			PRINT_FPTREE();
		}
		else if(commandFromtxt=="PRINT_BPTREE"){
			//PRINT_BPTREE();
		}
		else if(commandFromtxt=="PRINT_CONFIDENCE"){
			//PRINT_CONFIDENCE();
		}
		else if(commandFromtxt=="PRINT_RANGE"){
			//PRINT_RANGE();
		}
		else if(commandFromtxt=="SAVE"){
			SAVE();
		}
		else if(commandFromtxt=="EXIT"){

		}
	}
	fin.close();
	return;
}

bool Manager::LOAD()
{
	string item, strtokLine;
	ifstream market_txt;
	market_txt.open("market.txt", ios::app);

	if(!market_txt){
		flog<<"========LOAD========"<<endl;
		flog<<"ERROR 100"<<endl;
		flog<<"===================="<<endl<<endl;
	}
	else{
		while(!market_txt.eof()){
			getline(market_txt, strtokLine, '\n');
			char temp[200];
			strcpy(temp, strtokLine.c_str());
			char* ptr=strtok(temp, "\t");
			while(true){
				ptr=strtok(NULL, "\t");
				if(ptr==NULL)
					break;
				item=ptr;
				flog<<item<<endl;
			}
		}
		flog<<"end"<<endl;
	}
	return true;
}

bool Manager::BTLOAD()
{
	
	return true;
}

bool Manager::PRINT_ITEMLIST() {
	
}

bool Manager::PRINT_FPTREE() {
	
}

bool Manager::PRINT_BPTREE(char* item, int min_frequency) {
	
}

bool Manager::PRINT_CONFIDENCE(char* item, double rate) {
	
}

bool Manager::PRINT_RANGE(char* item, int start, int end) {
	
}

void Manager::printErrorCode(int n) {				//ERROR CODE PRINT
	//flog << ERROR " << n << " << endl;
	flog << "=======================" << endl << endl;
}

void Manager::printSuccessCode() {//SUCCESS CODE PRINT 
	flog << "Success" << endl;
	flog << "=======================" << endl << endl;
}

bool Manager::SAVE(){

}