#include <iostream>
#include <string>
#include "math.h"
#include <vector>

using namespace std;


//data packet
struct Datapacket{
	string srcaddress;
	string destaddress;
	string content;
	Datapacket(){}
};
//message content of the routing packet
struct Node{
	string destaddress;
	int estdelay = 0;
	Node(){}
};

//routing packet
struct Routingpacket{
	string gateway;
	int delay = 0;
	int nentries = 0;
	vector<Node> entries;
	Routingpacket(){}
};

//entry in routing table vector
struct tableRow{
	string destaddress;
	string gateway;
	int delay = 0;
	tableRow(){}
};

//find the index for spesific route
int tableIndex(vector<tableRow> routingtable, string gateway) {
	for(uint32_t i = 0; i < routingtable.size(); i++) {
		if(gateway == routingtable[i].destaddress) {
			return i;
		}
	}
	return -1;
}

//look to see if destination already exists in routing table
bool searchTable(vector<tableRow> routingtable, string address) {
	for(uint32_t i = 0; i < routingtable.size(); i++) {
		if(address == routingtable[i].destaddress) {
			return true;
		}
	}
	return false;
}

//update the routing table after each routing packet
void updateTable(vector<tableRow> &routingtable, vector<string> &output, Routingpacket rp, string myadr) {
	string gateway = rp.gateway;
	int delay = rp.delay;
	tableRow temptable = tableRow();

	//check if neighbor exists, if not create new entry
	if (!(searchTable(routingtable, gateway))) {
		temptable.gateway = gateway;
		temptable.destaddress = gateway;
		temptable.delay = delay;
        routingtable.push_back(temptable);
    }

	//if neighbor exists, check if new data has less delay and update if yes
	else {
    	int i = tableIndex(routingtable, gateway);
    	if(delay < routingtable[i].delay) {
    		routingtable[i].delay = delay;
    	}
    }
	//check the contents of the routing packet
	for(int i = 0; i < rp.nentries; i++) {
		//ignore distance to own router
		if(!(rp.entries[i].destaddress == myadr)) {
			delay = delay + rp.entries[i].estdelay;
			//if entry doesn't exist, push new entry into vector
			if (!(searchTable(routingtable, myadr))) {
				temptable.gateway = gateway;
				temptable.destaddress = rp.entries[i].destaddress;
				temptable.delay = delay;
				routingtable.push_back(temptable);
			}
			//check if new delay is less than current delay, if so, update
			else {
				int index = tableIndex(routingtable, rp.entries[i].destaddress);
				if(delay < routingtable[index].delay) {
					routingtable[index].delay = delay;
				}
			}
		}
	}
	output.push_back("THANK YOU");
}

//check data packet
void checkRoute(Datapacket dp, vector<tableRow> &routingtable, vector<string> &output) {
	string destination = dp.destaddress;
	//check if destination exists in routing table, save output to vector
	if(searchTable(routingtable, destination)) {
		int i = tableIndex(routingtable, destination);
		string outdelay = to_string(routingtable[i].delay);
		string gateway = routingtable[i].gateway + " " + outdelay;
		output.push_back(gateway);
	}
	//if not, print no route
	else {
		output.push_back("NO ROUTE");
	}
}


int main(){
	string myadr;
	int packamount;
	Datapacket dp = Datapacket();
	Routingpacket rp = Routingpacket();
	char packtype; //type of packet
	Node node = Node();
	vector<tableRow> routingtable;
	vector<string> output;

	cin >> myadr;
	cin >> packamount;
	string temp; //all entries on a routing packet
    
	for (int i = 0; i < packamount; i++) {

		cin >> packtype;
		switch(packtype) {
		case 'D' :
			cin >> dp.srcaddress;
        	cin >> dp.destaddress;
            cin >> temp;
        	checkRoute(dp,routingtable,output);
        	break;

		case 'R' :
        	cin >> rp.gateway;
        	cin >> rp.delay;
        	cin >> rp.nentries;

        	for (int i = 0; i < rp.nentries; i++) {
        		cin >> node.destaddress;
        		cin >> node.estdelay;
        		rp.entries.push_back(node);
        	}

    		updateTable(routingtable, output, rp, myadr);
        	break;
		}
	}
	cout << endl;
	for(uint32_t i = 0; i < routingtable.size(); i++){
		cout << routingtable[i].destaddress << " " << routingtable[i].gateway << " " << routingtable[i].delay << endl;
	}
	for(uint32_t i = 0; i < output.size(); i++){
		cout << output[i] << endl;
	}
    return 0;
}
