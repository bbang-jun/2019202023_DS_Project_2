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
		char *ptr = strtok(factor, " ");
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
    		tokItem = strtok(NULL, " ");
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
			//flog<<"정렬된 트랜잭션"<<endl;
			//for(iter=transactionList.begin(); iter!=transactionList.end(); iter++){
				//flog<<*iter<<endl;
			//}
			fpgrowth->createFPtree(fpgrowth->getTree(), fpgrowth->getHeaderTable(), transactionList, 1);
			transactionList.clear(); 
			transactionGetLine.clear(); 
		}
	}

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
	}
	else{
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

	result_txt.close();
	return true;
}

bool Manager::PRINT_ITEMLIST() {
	flog<<"====PRINT_ITEMLIST==="<<endl;
	flog<<"Item   Frequency"<<endl;
	fpgrowth->getHeaderTable()->descendingIndexTable(); // adpat descending to header table
	fpgrowth->getHeaderTable()->PRINT_ITEMLIST(); // indextable 출력
	flog<<"======================="<<endl<<endl;
	//fpgrowth->getHeaderTable()->printThresholdTable(); // 쓰레쉬홀드 제거된거 출력

	//fpgrowth->getHeaderTable()->first(); // dataTable 출력

	return true;
}

bool Manager::PRINT_FPTREE() { // for print FPTREE
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

	flog<<"========================="<<endl;
}

bool Manager::PRINT_BPTREE(char* item, int min_frequency) {
	ofstream flog;
	flog.open("log.txt", ofstream::app);
	BpTreeNode* moveNode=bptree->getRoot();

	// while(moveNode->getMostLeftChild()!=NULL)
	// 	moveNode=moveNode->getMostLeftChild();

	// map<int, FrequentPatternNode*>::iterator mapIter=moveNode->getDataMap()->begin();
	// multimap<int, set<string>>::iterator multimapIter=mapIter->second->getList().begin();
	// set<string>::iterator setIter=multimapIter->second.begin();
	
	// flog << "====== PRINT_BPTREE ======" << endl;

	// while(moveNode!=NULL){
	// 	while(mapIter!=moveNode->getDataMap()->end()){
	// 		while(multimapIter!=mapIter->second->getList().end()){
	// 			while(setIter!=multimapIter->second.end()){
	// 				flog<<*setIter<<endl;
	// 				setIter++;
	// 			}
	// 			multimapIter++;
	// 		}
	// 		mapIter++;
	// 	}
	// }


	map<int, FrequentPatternNode*>::iterator iter;
	multimap<int, set<string> >::iterator frequentIter;
	set<string>::iterator stringIter;


	moveNode = bptree->getRoot();
	while (moveNode->getMostLeftChild() != NULL) { //go down til we meet data node (not index node)
		moveNode = moveNode->getMostLeftChild();
	}
	flog << "====== PRINT_BPTREE ======" << endl;
//

//
	while (moveNode != NULL) { // map<int, FrequentPatternNode*>*
		map<int, FrequentPatternNode*> data1 = *moveNode->getDataMap();
		for (iter=data1.begin(); iter != data1.end(); iter++) {
			multimap<int, set<string>> data2 = iter->second->getList();
			for(frequentIter=data2.begin(); frequentIter!=data2.end(); frequentIter++){
				set<string> data3 = frequentIter->second;
				for(stringIter=data3.begin(); stringIter!=data3.end(); stringIter++){

					flog << "{"<< *stringIter <<"} "<<endl;
				}
				// bptree->printFrequentPatterns(frequentIter->second);
				// cout<<endl;
				flog<<frequentIter->first<<endl;
			}
			cout<<endl;
			flog<<iter->first<<endl;
		}
		moveNode = moveNode->getNext(); //move moveNode to next
	}
	flog << "======================" << endl<<endl;
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