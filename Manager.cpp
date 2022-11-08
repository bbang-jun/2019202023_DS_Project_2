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
	string  strtokLine;
	ifstream market_txt;
	int tempFrequency;

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
			char* charItem=strtok(temp, "\t");
			while(true){
				tempFrequency = fpgrowth->getHeaderTable()->find_frequency(charItem);
				fpgrowth->createTable(charItem, tempFrequency);
				charItem=strtok(NULL, "\t");
				if(charItem==NULL)
					break;
			}
		}
		fpgrowth->getHeaderTable()->descendingIndexTable();
		fpgrowth->createThIndexTable(); // threshold 이상인 것만 남기는 table 생성
		fpgrowth->getHeaderTable()->makeDataTable();
	}

	market_txt.close();

	market_txt.open("market.txt", ios::app);
		if(!market_txt){
		flog<<"========LOAD========"<<endl;
		flog<<"ERROR 100"<<endl;
		flog<<"===================="<<endl<<endl;
	}
	else{
		while(!market_txt.eof()){
			list<string> transactionList;
			getline(market_txt, strtokLine, '\n');
			char temp[200];
			strcpy(temp, strtokLine.c_str());
			char* charItem=strtok(temp, "\t");
			while(true){
				transactionList.push_back(charItem);
				charItem=strtok(NULL, "\t");
				if(charItem==NULL)
					break;
				
			}
			//transactionList.sort();
			fpgrowth->createFPtree(fpgrowth->getTree(), fpgrowth->getHeaderTable(), transactionList, 1);
			transactionList.clear();
		}

		//fpgrowth->getHeaderTable()->first();
	}



//FPNode* root, HeaderTable* table, list<string> item_array, int frequency

	return true;
}

bool Manager::BTLOAD()
{
	
	return true;
}

bool Manager::PRINT_ITEMLIST() {
	
	fpgrowth->getHeaderTable()->PRINT_ITEMLIST(); // indextable 출력

	fpgrowth->getHeaderTable()->printThresholdTable(); // 쓰레쉬홀드 제거된거 출력

	fpgrowth->getHeaderTable()->first(); // dataTable 출력

	return true;
}

bool Manager::PRINT_FPTREE() {
	
}

bool Manager::PRINT_BPTREE(char* item, int min_frequency) {
	
}

bool Manager::PRINT_CONFIDENCE(char* item, double rate) {
	
}

bool Manager::PRINT_RANGE(char* item, int start, int end) {
	
}

void Manager::printErrorCode(string command, int code) {				//ERROR CODE PRINT
	flog << "ERROR " << code << endl;
	flog << "=======================" << endl << endl;
}

void Manager::printSuccessCode() {//SUCCESS CODE PRINT 
	flog << "Success" << endl;
	flog << "=======================" << endl << endl;
}

bool Manager::SAVE(){

}