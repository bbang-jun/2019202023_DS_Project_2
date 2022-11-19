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
		// if(commandFromtxt==NULL)
		// 	break;
		char factor[100];
		if(commandFromtxt.empty()==true)
			break;
		strcpy(factor, commandFromtxt.c_str());
		char *ptr = strtok(factor, "\t");
		commandFromtxt=ptr;

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
			char *tokItem, *tokLeastFrequency;
    		tokItem = strtok(NULL, "\t");
   	 		tokLeastFrequency = strtok(NULL, "");
			if(tokItem==NULL || tokLeastFrequency==NULL){
				flog<<"========PRINT_BPTREE========"<<endl;
				flog<<"ERROR 500"<<endl;
				flog<<"=========================="<<endl<<endl;
				continue;
			}
			int frequency = atoi(tokLeastFrequency);

			PRINT_BPTREE(tokItem, frequency);
		}
		else if(commandFromtxt=="PRINT_CONFIDENCE"){
			char *tokItem, *tokConfidence;
    		tokItem = strtok(NULL, "\t");
   	 		tokConfidence = strtok(NULL, "");
			if(tokItem==NULL || tokConfidence==NULL){
				flog<<"========PRINT_CONFIDENCE========"<<endl;
				flog<<"ERROR 600"<<endl;
				flog<<"=========================="<<endl<<endl;
				continue;
			}
			double confidence;
			try{ // if tokConfidence is not number
				confidence = stod(tokConfidence);
				confidence = (round((int)(confidence*100))/100.0);
			}
			catch(const invalid_argument &){
				flog<<"========PRINT_CONFIDENCE========"<<endl;
				flog<<"ERROR 600"<<endl;
				flog<<"=========================="<<endl<<endl;
				continue;
			}
			PRINT_CONFIDENCE(tokItem, confidence);
		}
		else if(commandFromtxt=="PRINT_RANGE"){
			char *tokItem, *tokFirstRange, *tokSecondRange;
    		tokItem = strtok(NULL, "\t");
   	 		tokFirstRange = strtok(NULL, "\t");
			tokSecondRange = strtok(NULL, "");
			if(tokItem==NULL || tokFirstRange==NULL || tokSecondRange==NULL){
				flog<<"========PRINT_Range========"<<endl;
				flog<<"ERROR 700"<<endl;
				flog<<"=========================="<<endl<<endl;
				continue;
			}
			int firstRange, secondRange;
			try{
				firstRange = stoi(tokFirstRange);
				secondRange = stoi(tokSecondRange);
			}
			catch(const invalid_argument &){
				flog<<"========PRINT_Range========"<<endl;
				flog<<"ERROR 700"<<endl;
				flog<<"=========================="<<endl<<endl;
				continue;
			}

			PRINT_RANGE(tokItem, firstRange, secondRange);
		}
		else if(commandFromtxt=="SAVE"){
			SAVE();
		}
		else if(commandFromtxt=="EXIT"){
			flog<<"=======EXIT========"<<endl;
			flog<<"Success"<<endl;
			flog<<"==================="<<endl;

			break;
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
		return true;
	}
	else{
		if(fpgrowth->getTree()->getChildren().empty()==false){ // if data is already insert
			flog<<"========LOAD========"<<endl;
			flog<<"ERROR 100"<<endl;
			flog<<"===================="<<endl<<endl;
			return true;
		}
		while(!market_txt.eof()){
			getline(market_txt, strtokLine, '\n');
			string judgeEnd=strtokLine;
			if(judgeEnd.empty()==true){
				break;
			}
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
		list<string> transactionList;
		list<string> transactionGetLine;
		list<pair<int, string>> thIndexTable=fpgrowth->getHeaderTable()->getThIndexTable();
		map<string, FPNode*> dataTable=fpgrowth->getHeaderTable()->getdataTable();
		thIndexTable.sort(greater<pair<int, string>>());
		while(!market_txt.eof()){
			list<pair<int, string>> tempthIndexTable = thIndexTable;
			getline(market_txt, strtokLine, '\n');
			string judgeEnd=strtokLine;
			if(judgeEnd.empty()==true){
				break;
			}
			char temp[200];
			strcpy(temp, strtokLine.c_str());
			char* charItem=strtok(temp, "\t");

			while(true){
				transactionGetLine.push_back(charItem);

				charItem=strtok(NULL, "\t");
				if(charItem==NULL)
					break;
			}

			while(true){ // transaction에서 threshold 이상인 상품에 대해 frequency 순으로 정렬하여 transactionList 구성
				list<string>::iterator iter;
				for(iter=transactionGetLine.begin(); iter!=transactionGetLine.end(); iter++){
					if(tempthIndexTable.front().second==*iter){
						transactionList.push_back(tempthIndexTable.front().second);
						tempthIndexTable.pop_front();
						break;
					}
				}
				if(iter==transactionGetLine.end())
					tempthIndexTable.pop_front();;
				if(tempthIndexTable.empty())
					break;
			}
			list<string>::iterator iter;
			// below annotation is print of sorted transaction
			//flog<<"sorted transaction"<<endl;
			//for(iter=transactionList.begin(); iter!=transactionList.end(); iter++){
				//flog<<*iter<<endl;
			//}
			fpgrowth->createFPtree(fpgrowth->getTree(), fpgrowth->getHeaderTable(), transactionList, 1);
			transactionList.clear(); 
			transactionGetLine.clear(); 
		}
	}

	flog<<"========LOAD========"<<endl;
	flog<<"Success"<<endl;
	flog<<"===================="<<endl<<endl;

	return true;
}

bool Manager::BTLOAD()
{
	string  strtokLine;
	ifstream result_txt;
	set<string> itemset;

	result_txt.open("result.txt", ios::app);

	if(!result_txt){
		flog<<"========BTLOAD========"<<endl;
		flog<<"ERROR 200"<<endl;
		flog<<"===================="<<endl<<endl;
		return true;
	}
	else{
		if(bptree->getRoot()!=NULL){
			flog<<"========BTLOAD========"<<endl;
			flog<<"ERROR 200"<<endl;
			flog<<"===================="<<endl<<endl;
			return true;
		}
		while(!result_txt.eof()){
			getline(result_txt, strtokLine, '\n');
			string judgeEnd=strtokLine;
			if(judgeEnd.empty()==true){
				break;
			}
			char temp[200];
			strcpy(temp, strtokLine.c_str());
			char* charItem=strtok(temp, "\t");
			int frequency = atoi(charItem);
			while(true){
				charItem=strtok(NULL, "\t");

				if(charItem==NULL)
					break;
				itemset.insert(charItem);
			}
			bptree->Insert(frequency, itemset);
			itemset.clear();
		}
	}

	flog<<"========BTLOAD========"<<endl;
	flog<<"Success"<<endl;
	flog<<"===================="<<endl<<endl;

	result_txt.close();
	return true;
}

bool Manager::PRINT_ITEMLIST() {
	if(fpgrowth->getHeaderTable()->getindexTable().empty()==true){
		flog<<"========PRINT_ITEMLIST========"<<endl;
		flog<<"ERROR 300"<<endl;
		flog<<"=============================="<<endl<<endl;
		return true;
	}
	flog<<"========PRINT_ITEMLIST========"<<endl;
	flog<<"Item   Frequency"<<endl;
	fpgrowth->getHeaderTable()->descendingIndexTable(); // adpat descending to indexTable
	fpgrowth->getHeaderTable()->PRINT_ITEMLIST(); // print indexTable
	flog<<"==============================="<<endl<<endl;

	//fpgrowth->getHeaderTable()->printThresholdTable(); // print eliminate of below the threshold
	//fpgrowth->getHeaderTable()->first(); // print dataTable

	return true;
}

bool Manager::PRINT_FPTREE() { // for print FPTREE

	if(fpgrowth->getTree()->getChildren().empty()==true){ // if data is already insert
		flog<<"========PRINT_FPTREE========"<<endl;
		flog<<"ERROR 400"<<endl;
		flog<<"============================="<<endl<<endl;
		return true;
	}
	map<string, FPNode*> dataTable=fpgrowth->getHeaderTable()->getdataTable();
	list<pair<int, string>> ThIndexTable=fpgrowth->getHeaderTable()->getThIndexTable();

	list<pair<int, string>>::iterator ThIndexTableIter=ThIndexTable.begin();
	map<string, FPNode*>::iterator dataTableIter=dataTable.begin();

	map<string, FPNode*>::iterator childrenIter;
	map<string, FPNode*> children;

	ThIndexTable.sort(); // sort thIndexTable

	FPNode*Pointer; // data table's pointer node
	FPNode*pathNode;                
	FPNode*moveNode;

	flog<<"========PRINT_FPTREE========"<<endl;

	flog<<"{StandardItem,Frequency} (Path_Item,Frequency)"<<endl;

		for(ThIndexTableIter=ThIndexTable.begin(); ThIndexTableIter!=ThIndexTable.end(); ThIndexTableIter++){
			flog<<"{"<<ThIndexTableIter->second<<", "<<ThIndexTableIter->first<<"}"<<endl;
			for(dataTableIter=dataTable.begin(); dataTableIter!=dataTable.end(); dataTableIter++){
				if(ThIndexTableIter->second==dataTableIter->first){
					Pointer=dataTableIter->second;
					moveNode=Pointer->getNext();
					break;
				}
			}

			while(moveNode!=NULL){
				pathNode=moveNode;
				while(pathNode!=fpgrowth->getTree()){
					children=pathNode->getParent()->getChildren();
					for(childrenIter=children.begin(); childrenIter!=children.end(); childrenIter++){
						if(childrenIter->second==pathNode){
							flog<<"("<<childrenIter->first<<", "<<pathNode->getFrequency()<<") ";
							break;
						}
					}
					pathNode=pathNode->getParent();
				}
				flog<<endl;
				moveNode=moveNode->getNext();
			}
		}

	flog<<"========================="<<endl<<endl;
}

bool Manager::PRINT_BPTREE(char* item, int min_frequency) {

	if(bptree->getRoot()==NULL){
		flog<<"========PRINT_BPTREE========"<<endl;
		flog<<"ERROR 500"<<endl;
		flog<<"=========================="<<endl<<endl;
		return true;
	}
	ofstream flog;
	flog.open("log.txt", ofstream::app);
	BpTreeNode* moveNode=bptree->getRoot();
	int i=0;

	map<int, FrequentPatternNode*>::iterator iter;
	multimap<int, set<string> >::iterator frequentIter;
	set<string>::iterator stringIter;


	moveNode = bptree->getRoot();
	while (moveNode->getMostLeftChild() != NULL) { //go down til we meet data node (not index node)
		moveNode = moveNode->getMostLeftChild();
	}
	
	flog << "========PRINT_BPTREE========" << endl;
	
		while (moveNode != NULL) {
		map<int, FrequentPatternNode*> data1 = *moveNode->getDataMap();
		for (iter=data1.begin(); iter != data1.end(); iter++) {
			multimap<int, set<string>> data2 = iter->second->getList();
			for(frequentIter=data2.begin(); frequentIter!=data2.end(); frequentIter++){
				set<string> data3 = frequentIter->second;
				for(stringIter=data3.begin(); stringIter!=data3.end(); stringIter++){
					if((*stringIter==item)&&(iter->first>=min_frequency)){
						i++;
						if(i==1)
							flog <<"FrequentPattern		Frequency"<<endl;
						flog<<"{";
						for(stringIter=data3.begin(); stringIter!=data3.end(); stringIter++){
						if((++stringIter)--==data3.end()){
							flog<<*stringIter;
							break;
						}
					
						flog <<*stringIter<<", ";
						}
						flog<<"} "<<iter->first<<endl;
					}
				}
			}
		}
		moveNode = moveNode->getNext(); //move moveNode to next
	}
	if(i==0) // there is nothing to print Frequent Pattern
		flog<<"ERROR 500"<<endl; 

	flog << "==========================" << endl<<endl;
}

bool Manager::PRINT_CONFIDENCE(char* item, double rate) {


	list<pair<int, string>> indexTable = fpgrowth->getHeaderTable()->getindexTable();

	if(indexTable.empty()==true||bptree->getRoot()==NULL){
		flog<<"========PRINT_CONFIDENCE========"<<endl;
		flog<<"ERROR 600"<<endl;
		flog<<"=================================="<<endl<<endl;
		return true;
	}

	int totalFrequeny=fpgrowth->getHeaderTable()->find_frequency(item);

	bptree->printConfidence(item, totalFrequeny, rate);

	return true;
}

bool Manager::PRINT_RANGE(char* item, int start, int end) {
	
	if(bptree->getRoot()==NULL){
		flog<<"========PRINT_RANGE========"<<endl;
		flog<<"ERROR 700"<<endl;
		flog<<"=========================="<<endl<<endl;
		return true;
	}

	BpTreeNode* moveNode=bptree->getRoot();

	map<int, FrequentPatternNode*>::iterator iter;
	multimap<int, set<string> >::iterator frequentIter;
	set<string>::iterator stringIter;
	int i=0;


	moveNode = bptree->getRoot();
	while (moveNode->getMostLeftChild() != NULL) { //go down til we meet data node (not index node)
		moveNode = moveNode->getMostLeftChild();
	}

	flog<<"========PRINT_RANGE========"<<endl;
	
	while (moveNode != NULL) {
		map<int, FrequentPatternNode*> data1 = *moveNode->getDataMap();
		for (iter=data1.begin(); iter != data1.end(); iter++) {
			multimap<int, set<string>> data2 = iter->second->getList();
			for(frequentIter=data2.begin(); frequentIter!=data2.end(); frequentIter++){
				set<string> data3 = frequentIter->second;
				for(stringIter=data3.begin(); stringIter!=data3.end(); stringIter++){
					if((*stringIter==item)&&((iter->first>=start)&&(iter->first<=end))){
						i++;
						if(i==1)
						flog<<"FrequentPattern Frequency"<<endl;
						flog<<"{";
						for(stringIter=data3.begin(); stringIter!=data3.end(); stringIter++){
							if((++stringIter)--==data3.end()){
								flog<<*stringIter;
								break;
							}
							flog <<*stringIter<<", ";
						}
						flog<<"} "<<iter->first<<endl;
					}
				}
			}
		}
		moveNode = moveNode->getNext(); //move moveNode to next
	}
	if(i==0) // there is nothing to print Frequent Pattern
		flog<<"ERROR 700"<<endl; 

	flog<<"============================="<<endl<<endl;
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