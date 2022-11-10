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
	
	return true;
}

bool Manager::PRINT_ITEMLIST() {
	//fpgrowth->getHeaderTable()->descendingIndexTable(); // adpat descending to header table
	//fpgrowth->getHeaderTable()->PRINT_ITEMLIST(); // indextable 출력

	//fpgrowth->getHeaderTable()->printThresholdTable(); // 쓰레쉬홀드 제거된거 출력

	//fpgrowth->getHeaderTable()->first(); // dataTable 출력

	return true;
}

bool Manager::PRINT_FPTREE() { // for print FPTREE
	list<pair<int, string>> thIndexTable=fpgrowth->getHeaderTable()->getThIndexTable(); // for initialize tempThIndexTable
	map<string, FPNode*> dataTable=fpgrowth->getHeaderTable()->getdataTable();
	list<pair<int, string>> tempThIndexTable=thIndexTable;
	list<pair<int, string>>::iterator tempThIndexTableIter=tempThIndexTable.begin();
	map<string, FPNode*>::iterator dataTableIter=dataTable.begin();

	thIndexTable.sort(); // sort thIndexTable
	tempThIndexTable.sort(); // sort tempThIndexTable

	flog<<"========PRINT_FPTREE========"<<endl;
	flog<<"{StandardItem,Frequency} (Path_Item,Frequency)"<<endl;

	FPNode*Pointer; // data table's pointer node

	list<pair<int, string>> cycle = tempThIndexTable; // cycle number is size of ThIndexTable

	while(cycle.empty()!=true){ //

		for(dataTableIter=dataTable.begin(); dataTableIter!=dataTable.end(); dataTableIter++){ // datatable의 노드를 찾기 위함
			if(dataTableIter->first==cycle.front().second){ // dataTable에서 indexTable의 내림차순으로의 상품을 찾으면 dataTable의 해당 상품 노드 저장
				Pointer=dataTable.find(cycle.front().second)->second; // second는 상품명
				break;
			} // eggs에 대한 FPNode*를 찾음
		}

		FPNode*bottomNode=Pointer->getNext(); // 해당 상품의 1차원 연결 노드 중 가장 아래(위로 이동x)에 있고, 옆으로 이동하기 위한 노드
		FPNode*pathNode; // move 노드 기준으로 위까지 출력하기 위해 위로 이동하는 노드
		FPNode* checkNode; // data table의 pointer node로부터 끝까지 next를 하여 it의 node와 동일하면 ()를 출력하기 위해 생성
		FPNode* rootUnderNode; // root 노드 바로 밑 노드
		bool judge=false;
		flog<<"{"<<cycle.front().second<<", "<<cycle.front().first<<"}"<<endl; // {} 부분 출력
		

		while(1){ // 처음에는 동일한 위치에서 시작
			
			pathNode=bottomNode;
			while(pathNode->getParent()!=NULL){
				forPrint:
				map<string, FPNode*>::iterator it; 
				tempThIndexTable=thIndexTable; // 제거된 tempThIndexTable을 다시 처음으로 돌려서 비교하기 위함
				while(1){ // it와 pathNode를 맞추기 위한 작업 (맞아야만 제대로된 상품명 출력 가능)
					it=pathNode->getParent()->getChildren().find(tempThIndexTable.front().second); // 위로 올라갔다가 getChildren에서 find를 통해 해당 노드를 찾은 반복자
					if(it->second!=pathNode){ // 출력하기 위한 해당 노드와 pathNode와 다르면 tempThIndexTable의 pop front
						tempThIndexTable.pop_front();
					}
					else{
						break;
					}
				}
				for(dataTableIter=dataTable.begin(); dataTableIter!=dataTable.end(); dataTableIter++){ // data table에 대한 반복자
					if(judge==true){ // judge가 true인 경우는 처음부터 다시 봐야 하는 data table에 대한 반복자가 처음으로 만들어 주기 위함
						dataTableIter=dataTable.begin();
						judge=false; // 만들고 다시 false로 돌리고 출력 후에 다시 true로 만듬
					}
					tempThIndexTable=thIndexTable; // pop된 tempThIndexTable을 다시 처음으로 돌려서 비교하기 위함
					while(1){ // it와 pathNode를 맞추기 위한 작업 (맞아야만 제대로된 상품명 출력 가능)
						it=pathNode->getParent()->getChildren().find(tempThIndexTable.front().second); 
						if(it->second!=pathNode){
							tempThIndexTable.pop_front();
						}
						else{
							break;
						}
					}

					// 무조건 dataTable의 각 상품노드, 그리고 그와 연결된 fp-tree의 상품 노드들을 모두 돌아서 검색
					if(dataTableIter->first!=it->first){ // dataTableIter를 반복하여 일치하지 않으면 다음순서로 이동(위의 가장 큰 for문) 
						continue;
					}
					else{ // 같은 경우
						// 체크노드는 1차원 돌아서 찾는 노드
						reSettingPointer:
						checkNode=Pointer; // data table의 FpNode*부터 시작(사실상 head)
						while(1){ // 
							if(it->second==checkNode){ // 반복자의 FPNode*와 checkNode가 동일한 경우
								flog<<"("<<it->first<<", "<<pathNode->getFrequency()<<") "; // ()내부 출력
								if(pathNode->getParent()->getParent()!=NULL) // pathNode가 root의 children이 아닌 경우
									judge=true; // judge를 true로 만듬. 위에서 테이블을 살리기 위함
								if(pathNode->getParent()->getParent()==NULL){ // pathNode가 root의 children인 경우
									rootUnderNode=pathNode; // 제일 위의 출력하는 노드라는 의미로 rootUnderNode에 pathNode 저장
									break;
								}
									
								pathNode=pathNode->getParent(); // 위에 더 출력할 node가 남았으면 위로 이동
								goto forPrint; // xyz로 이동하여 다시 출력 과정 거치기
							}	
							if(checkNode->getNext()==NULL){ // checkNode 끝까지 갔는데 없는 경우 다시 dataTable으 참조하여 PointerNode를 설정함
								for(dataTableIter=dataTable.begin(); dataTableIter!=dataTable.end(); dataTableIter++){ // datatable의 노드를 찾기 위함
									if(dataTableIter->first==tempThIndexTable.front().second){ // dataTable에서 indexTable의 내림차순으로의 상품을 찾으면 dataTable의 해당 상품 노드 저장
										Pointer=dataTable.find(tempThIndexTable.front().second)->second; // second는 상품명
										goto reSettingPointer;
									} 
								}
							}
							checkNode=checkNode->getNext(); // 반복자의 FPNode*와 checkNode가 동일하지 않으면 next로 옆으로 이동하여 다시 비교하기 위해 while(1)로 이동
						}
					}
					if(pathNode==rootUnderNode){ // 현재 출력한 노드가 root의 children 노드일 때 (경로를 다 출력한 경우)
						if(bottomNode->getNext()==NULL){ // 한 아이템에 대한 모든 경로를 출력한 경우
							flog<<endl;
							break; // 1차 연속 break(다음 아이템에 대한 모든 경로를 출력하기 위해)
						}
						flog<<endl;
						pathNode=bottomNode->getNext(); // 해당 아이템의 다음 경로를 출력하기 위함
						bottomNode=bottomNode->getNext(); // 해당 아이템의 다음 경로를 출력하기 위함
						dataTableIter=dataTable.begin(); // data table 반복자를 처음으로 이동
					}
				}
				if(bottomNode->getNext()==NULL){
					break; // 2차 연속 break(다음 아이템에 대한 모든 경로를 출력하기 위해)
				}
			}
			if(bottomNode->getNext()==NULL){ 
				break; // 3차 연속 break(다음 아이템에 대한 모든 경로를 출력하기 위해)
			}
		}
		cycle.pop_front();
	}
	flog<<"========================="<<endl;
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