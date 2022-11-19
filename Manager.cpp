#include "Manager.h"

void Manager::run(const char* command) // execute all command
{
	string commandFromtxt; 
	fin.open(command);
	flog.open("log.txt", ios::app);
	if(!fin) // if file is not open
	{
		flog << "File Open Error" << endl;
		return;
	}
	while (!fin.eof()) // until end of file
	{
		getline(fin, commandFromtxt, '\n'); // get the line of command.txt file
		char factor[100];
		if(commandFromtxt.empty()==true) // if command line is empty
			break;
		strcpy(factor, commandFromtxt.c_str());
		char *ptr = strtok(factor, "\t");
		commandFromtxt=ptr;

		if(commandFromtxt=="LOAD"){ // if command is LOAD
			LOAD();
		}
		else if(commandFromtxt=="BTLOAD"){ // if command is BTLOAD
			BTLOAD();
		}
		else if(commandFromtxt=="PRINT_ITEMLIST"){ // if command is PRINT_ITEMLIST
			PRINT_ITEMLIST();
		}
		else if(commandFromtxt=="PRINT_FPTREE"){ // if command is PRINT_FPTREE
			PRINT_FPTREE();
		}
		else if(commandFromtxt=="PRINT_BPTREE"){ // if command is PRINT_BPTREE
			char *tokItem, *tokLeastFrequency;
    		tokItem = strtok(NULL, "\t"); // get first argument(factor)
   	 		tokLeastFrequency = strtok(NULL, ""); // get second argument(factor)

			if(tokItem==NULL || tokLeastFrequency==NULL){ // if at least one argument is NULL, print ERROR 500
				flog<<"========PRINT_BPTREE========"<<endl;
				flog<<"ERROR 500"<<endl;
				flog<<"=========================="<<endl<<endl;
				continue;
			}
			int frequency = atoi(tokLeastFrequency); // char to int

			PRINT_BPTREE(tokItem, frequency); 
		}
		else if(commandFromtxt=="PRINT_CONFIDENCE"){ // if command is PRINT_CONFIDENCE
			char *tokItem, *tokConfidence;
    		tokItem = strtok(NULL, "\t"); // get first argument(factor)
   	 		tokConfidence = strtok(NULL, ""); // get second argument(factor)
			if(tokItem==NULL || tokConfidence==NULL){ // if at least one argument is NULL, print ERROR 600
				flog<<"========PRINT_CONFIDENCE========"<<endl;
				flog<<"ERROR 600"<<endl;
				flog<<"=========================="<<endl<<endl;
				continue;
			}
			double confidence;
			try{ // if tokConfidence is not a number
				confidence = stod(tokConfidence); // string to double
				confidence = (round((int)(confidence*100))/100.0); // rounding
			}
			catch(const invalid_argument &){ // print ERROR 600
				flog<<"========PRINT_CONFIDENCE========"<<endl;
				flog<<"ERROR 600"<<endl;
				flog<<"=========================="<<endl<<endl;
				continue;
			}
			PRINT_CONFIDENCE(tokItem, confidence);
		}
		else if(commandFromtxt=="PRINT_RANGE"){ // if command is PRINT_RANGE
			char *tokItem, *tokFirstRange, *tokSecondRange;
    		tokItem = strtok(NULL, "\t"); // get first argument(factor)
   	 		tokFirstRange = strtok(NULL, "\t"); // get second argument(factor)
			tokSecondRange = strtok(NULL, ""); // get third argument(factor)
			if(tokItem==NULL || tokFirstRange==NULL || tokSecondRange==NULL){ // if at least one argument is NULL, print ERROR 700
				flog<<"========PRINT_Range========"<<endl;
				flog<<"ERROR 700"<<endl;
				flog<<"=========================="<<endl<<endl;
				continue;
			}
			int firstRange, secondRange;
			try{ // if firstRange or secondRange is not a number
				firstRange = stoi(tokFirstRange);
				secondRange = stoi(tokSecondRange);
			}
			catch(const invalid_argument &){ // print ERROR 700
				flog<<"========PRINT_Range========"<<endl;
				flog<<"ERROR 700"<<endl;
				flog<<"=========================="<<endl<<endl;
				continue;
			}

			PRINT_RANGE(tokItem, firstRange, secondRange);
		}
		else if(commandFromtxt=="SAVE"){ // if command is SAVE
			// SAVE();
		}
		else if(commandFromtxt=="EXIT"){ // if command is EXIT
			flog<<"=======EXIT========"<<endl;
			flog<<"Success"<<endl;
			flog<<"==================="<<endl;

			break;
		}
	}
	fin.close();
	return;
}

bool Manager::LOAD() // read market.txt and make FPGrowth Tree
{
	string  strtokLine;
	ifstream market_txt;
	int tempFrequency;
	list<string>::iterator cmpIter;
	list<pair<int, string>>::iterator iter;
	bool judge=true;

	market_txt.open("market.txt", ios::app); // open the file market.txt

	if(!market_txt){ // if file is not open, print the ERROR 100
		flog<<"========LOAD========"<<endl;
		flog<<"ERROR 100"<<endl;
		flog<<"===================="<<endl<<endl;
		return true;
	}
	else{
		if(fpgrowth->getTree()->getChildren().empty()==false){ // if data is already insert, print the ERROR 100
			flog<<"========LOAD========"<<endl;
			flog<<"ERROR 100"<<endl;
			flog<<"===================="<<endl<<endl;
			return true;
		}
		while(!market_txt.eof()){ // until the end of file
			getline(market_txt, strtokLine, '\n'); // get the line of market.txt file
			string judgeEnd=strtokLine; // judge of file end
			if(judgeEnd.empty()==true){
				break;
			}
			char temp[200];
			strcpy(temp, strtokLine.c_str());
			char* charItem=strtok(temp, "\t");
			tempFrequency = fpgrowth->getHeaderTable()->find_frequency(charItem); // for renew of frequency
			fpgrowth->createTable(charItem, tempFrequency); // create the indexTable
			list<string> tempList;
			tempList.push_back(charItem);
			while(true){ // insert the item into the headerTable
				charItem=strtok(NULL, "\t"); // if item is not exist
				if(charItem==NULL)
					break;

				for(cmpIter=tempList.begin(); cmpIter!=tempList.end(); cmpIter++){
					if(charItem==*cmpIter){
						judge=false;
						break;
					}
				}

				if(judge=false){
					charItem=strtok(NULL, "\t");
					judge=true;
					if(charItem==NULL)
						break;
				}
				else{
					tempFrequency = fpgrowth->getHeaderTable()->find_frequency(charItem); // for renew of frequency
					fpgrowth->createTable(charItem, tempFrequency); // create the indexTable
					tempList.push_back(charItem);
				}
			}
		}
		fpgrowth->createThIndexTable(); // make the table, to make same or bigger than threshold
		fpgrowth->getHeaderTable()->makeDataTable(); // make the dataTable
	}

	market_txt.close(); // close market.txt file

	market_txt.open("market.txt", ios::app); // reopen market.txt file

	if(!market_txt){ // if file is not open, print the ERROR 100
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
			if(tempthIndexTable.size()==0){ // if same or bigger with threshold table's size is 0, print the ERROR 100
				flog<<"========LOAD========"<<endl;
				flog<<"ERROR 100"<<endl;
				flog<<"===================="<<endl<<endl;
				return true;
			}
	
			getline(market_txt, strtokLine, '\n'); // get the line in market.txt file
			string judgeEnd=strtokLine; 
			if(judgeEnd.empty()==true){ // judge of file end
				break;
			}
			char temp[200];
			strcpy(temp, strtokLine.c_str());
			char* charItem=strtok(temp, "\t");

			while(true){ // make transaction list
				transactionGetLine.push_back(charItem);

				charItem=strtok(NULL, "\t");
				if(charItem==NULL)
					break;
			}
			
		
			while(true){ // sort the transaction, same or bigger with indexTable's descending frequency
				list<string>::iterator iter;
				for(iter=transactionGetLine.begin(); iter!=transactionGetLine.end(); iter++){
					if(tempthIndexTable.front().second==*iter){ // if same item finding
						transactionList.push_back(tempthIndexTable.front().second); // push the item into sorted transactionlist(transactionList)
						tempthIndexTable.pop_front(); // eliminate temp threshold index table
						break;
					}
				}
				if(iter==transactionGetLine.end()) // delete last item
					tempthIndexTable.pop_front();
				if(tempthIndexTable.empty()) // if temp index table is empty, break
					break;
			}
			list<string>::iterator iter;

			fpgrowth->createFPtree(fpgrowth->getTree(), fpgrowth->getHeaderTable(), transactionList, 1); // create FPGrowth Tree
			transactionList.clear(); // clear the trasactionList
			transactionGetLine.clear();  // clear the transactionGetLine
		}
	}

	// print success code
	flog<<"========LOAD========"<<endl;
	flog<<"Success"<<endl;
	flog<<"===================="<<endl<<endl;

	return true;
}

bool Manager::BTLOAD() // read result.txt and make B+-Tree
{
	string  strtokLine;
	ifstream result_txt;
	set<string> itemset;

	result_txt.open("result.txt", ios::app); // open the result.txt file

	if(!result_txt){ // if result.txt file is not open, print the ERROR 200
		flog<<"========BTLOAD========"<<endl;
		flog<<"ERROR 200"<<endl;
		flog<<"===================="<<endl<<endl;
		return true;
	}
	else{
		if(bptree->getRoot()!=NULL){ // if B+-Tree's root is not exist, print the ERROR 200
			flog<<"========BTLOAD========"<<endl;
			flog<<"ERROR 200"<<endl;
			flog<<"===================="<<endl<<endl;
			return true;
		}
		while(!result_txt.eof()){ // until result.txt's end of file
			getline(result_txt, strtokLine, '\n'); // get the line from result.txt file
			string judgeEnd=strtokLine; 
			if(judgeEnd.empty()==true){ // judge end of file
				break;
			}
			char temp[200];
			strcpy(temp, strtokLine.c_str());
			char* charItem=strtok(temp, "\t");
			int frequency = atoi(charItem); // change char to int

			while(true){ // make set<string>, for B+-Tree's insert
				charItem=strtok(NULL, "\t");

				if(charItem==NULL)
					break;
				itemset.insert(charItem);
			}
			bptree->Insert(frequency, itemset); // insert frequency and itemset from using Insert fuction, to make B+-Tree
			itemset.clear(); // clear itemset
		}
	}

	// print success code
	flog<<"========BTLOAD========"<<endl;
	flog<<"Success"<<endl;
	flog<<"===================="<<endl<<endl;

	result_txt.close(); // close the result.txt
	return true;
}

bool Manager::PRINT_ITEMLIST() { // print indexTable
	if(fpgrowth->getHeaderTable()->getindexTable().empty()==true){ // if header table is emtpy, print the ERROR 300
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

	if(fpgrowth->getTree()->getChildren().empty()==true){ // if FP-Tree is empty, print the ERROR 400
		flog<<"========PRINT_FPTREE========"<<endl;
		flog<<"ERROR 400"<<endl;
		flog<<"============================="<<endl<<endl;
		return true;
	}
	map<string, FPNode*> dataTable=fpgrowth->getHeaderTable()->getdataTable(); // declare iterator
	list<pair<int, string>> ThIndexTable=fpgrowth->getHeaderTable()->getThIndexTable(); // declare iterator

	list<pair<int, string>>::iterator ThIndexTableIter=ThIndexTable.begin(); // declare iterator
	map<string, FPNode*>::iterator dataTableIter=dataTable.begin(); // declare iterator

	map<string, FPNode*>::iterator childrenIter; // declare iterator
	map<string, FPNode*> children;

	ThIndexTable.sort(); // sort thIndexTable

	FPNode*Pointer; // data table's pointer node
	FPNode*pathNode; // move node for print(go to the up)  
	FPNode*moveNode; // move node for setting pathNode(go to the right(next))

	flog<<"========PRINT_FPTREE========"<<endl;

	flog<<"{StandardItem,Frequency} (Path_Item,Frequency)"<<endl;

		for(ThIndexTableIter=ThIndexTable.begin(); ThIndexTableIter!=ThIndexTable.end(); ThIndexTableIter++){
			flog<<"{"<<ThIndexTableIter->second<<", "<<ThIndexTableIter->first<<"}"<<endl; // print the standard item and frequency
			for(dataTableIter=dataTable.begin(); dataTableIter!=dataTable.end(); dataTableIter++){
				if(ThIndexTableIter->second==dataTableIter->first){ // compare indexTable and dataTable
					Pointer=dataTableIter->second; // find case
					moveNode=Pointer->getNext(); // moveNode is next node of dataTable's pointer node
					break;
				}
			}

			while(moveNode!=NULL){ // while moveNode is not NULL
				pathNode=moveNode; // setting pathNode for print
				while(pathNode!=fpgrowth->getTree()){ // while pathNode is not root(=top)
					children=pathNode->getParent()->getChildren();
					for(childrenIter=children.begin(); childrenIter!=children.end(); childrenIter++){
						if(childrenIter->second==pathNode){
							flog<<"("<<childrenIter->first<<", "<<pathNode->getFrequency()<<") "; // print the path
							break;
						}
					}
					pathNode=pathNode->getParent(); // go up for print
				}
				flog<<endl;
				moveNode=moveNode->getNext(); // move Next for setting pathNode
			}
		}

	flog<<"========================="<<endl<<endl;

	return true;
}

bool Manager::PRINT_BPTREE(char* item, int min_frequency) { // print the B+-tree, satisfying item and min_frequency

	if(bptree->getRoot()==NULL){ // if B+-tree's root is NULL, print ERROR 500
		flog<<"========PRINT_BPTREE========"<<endl;
		flog<<"ERROR 500"<<endl;
		flog<<"=========================="<<endl<<endl;
		return true;
	}
	ofstream flog;
	flog.open("log.txt", ofstream::app);
	BpTreeNode* moveNode=bptree->getRoot(); // moveNode's initial is B+-Tree's root
	int i=0;

	map<int, FrequentPatternNode*>::iterator iter; // declare iterator
	multimap<int, set<string> >::iterator frequentIter; // declare iterator
	set<string>::iterator stringIter; // declare iterator

	while (moveNode->getMostLeftChild() != NULL) { // go down until meet data node (not index node)
		moveNode = moveNode->getMostLeftChild();
	}
	
	flog << "========PRINT_BPTREE========" << endl;
	
		while (moveNode != NULL) { // moveNode is first of dataNode list
		map<int, FrequentPatternNode*> iterData = *moveNode->getDataMap();
		for (iter=iterData.begin(); iter != iterData.end(); iter++) {
			multimap<int, set<string>> fqIterData = iter->second->getList();
			for(frequentIter=fqIterData.begin(); frequentIter!=fqIterData.end(); frequentIter++){
				set<string> stringIterData = frequentIter->second;
				for(stringIter=stringIterData.begin(); stringIter!=stringIterData.end(); stringIter++){
					if((*stringIter==item)&&(iter->first>=min_frequency)){
						i++;
						if(i==1) // print only once
							flog <<"FrequentPattern		Frequency"<<endl;
						flog<<"{";
						for(stringIter=stringIterData.begin(); stringIter!=stringIterData.end(); stringIter++){
						if((++stringIter)--==stringIterData.end()){ // compare index for find item name
							flog<<*stringIter; // print FrequentPattern
							break;
						}
					
						flog <<*stringIter<<", "; // print FrequentPattern
						}
						flog<<"} "<<iter->first<<endl; // print frequency
					}
				}
			}
		}
		moveNode = moveNode->getNext(); // move moveNode to next
	}
	if(i==0) // there is nothing to print Frequent Pattern, print ERROR 500
		flog<<"ERROR 500"<<endl; 

	flog << "==========================" << endl<<endl;

	return true;
}

bool Manager::PRINT_CONFIDENCE(char* item, double rate) { // print same or bigger confidence


	list<pair<int, string>> indexTable = fpgrowth->getHeaderTable()->getindexTable(); // get indexTable for check emtpy

	if(indexTable.empty()==true||bptree->getRoot()==NULL){ // if indexTable is empty or B+-Tree's root is not exist, print ERROR 600
		flog<<"========PRINT_CONFIDENCE========"<<endl;
		flog<<"ERROR 600"<<endl;
		flog<<"=================================="<<endl<<endl;
		return true;
	}

	int totalFrequeny=fpgrowth->getHeaderTable()->find_frequency(item); // totla frequency of item

	bptree->printConfidence(item, totalFrequeny, rate); // call the printConfidence function, to print result

	return true;
}

bool Manager::PRINT_RANGE(char* item, int start, int end) { // print the B+-Tree's Frequent Pattern with range condition
	
	if(bptree->getRoot()==NULL){ // if B+-Tree's root is NULL, print ERROR 700
		flog<<"========PRINT_RANGE========"<<endl;
		flog<<"ERROR 700"<<endl;
		flog<<"=========================="<<endl<<endl;
		return true;
	}

	BpTreeNode* moveNode=bptree->getRoot(); // initilize moveNode to B+-Tree's root

	map<int, FrequentPatternNode*>::iterator iter; // declase iterator
	multimap<int, set<string> >::iterator frequentIter; // declase iterator
	set<string>::iterator stringIter; // declase iterator
	int i=0;

	while (moveNode->getMostLeftChild() != NULL) { // go down until meet data node (not index node)
		moveNode = moveNode->getMostLeftChild();
	}

	flog<<"========PRINT_RANGE========"<<endl;
	
	while (moveNode != NULL) { // repeat moveNode is not NULL
		map<int, FrequentPatternNode*> iterData = *moveNode->getDataMap();
		for (iter=iterData.begin(); iter != iterData.end(); iter++) {
			multimap<int, set<string>> fqData = iter->second->getList();
			for(frequentIter=fqData.begin(); frequentIter!=fqData.end(); frequentIter++){
				set<string> stringData = frequentIter->second;
				for(stringIter=stringData.begin(); stringIter!=stringData.end(); stringIter++){
					if((*stringIter==item)&&((iter->first>=start)&&(iter->first<=end))){
						i++;
						if(i==1) // print only once
						flog<<"FrequentPattern Frequency"<<endl;
						flog<<"{";
						for(stringIter=stringData.begin(); stringIter!=stringData.end(); stringIter++){
							if((++stringIter)--==stringData.end()){
								flog<<*stringIter; // print FrequentPattern
								break;
							}
							flog <<*stringIter<<", "; // print FrequentPattern
						}
						flog<<"} "<<iter->first<<endl; // print frequency
					}
				}
			}
		}
		moveNode = moveNode->getNext(); // move moveNode to next
	}
	if(i==0) // there is nothing to print Frequent Pattern, print the ERROR 700
		flog<<"ERROR 700"<<endl; 

	flog<<"============================="<<endl<<endl;

	return true;
}

void Manager::printErrorCode(string command, int code) { // ERROR CODE PRINT
	flog << "ERROR " << code << endl;
	flog << "=======================" << endl << endl;
}

void Manager::printSuccessCode() {// SUCCESS CODE PRINT 
	flog << "Success" << endl;
	flog << "=======================" << endl << endl;
}

bool Manager::SAVE(){

}