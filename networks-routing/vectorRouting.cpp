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
int tableIndex(vector<tableRow> routingtable, string address, string gateway) {
	for(uint32_t i = 0; i < routingtable.size(); i++) {
		if(address == routingtable[i].destaddress && gateway == routingtable[i].gateway) {
			return i;
		}
	}
	return -1;
}

int gatewayIndex(vector<tableRow> routingtable, string gateway) {
	for(uint32_t i = 0; i < routingtable.size(); i++) {
		if(gateway == routingtable[i].gateway) {
			return i;
		}
	}
	return -1;
}
//look to see if destination already exists in routing table
bool searchTable(vector<tableRow> routingtable, string address, string gateway) {
	for(uint32_t i = 0; i < routingtable.size(); i++) {
		if(address == routingtable[i].destaddress && gateway == routingtable[i].gateway) {
			return true;
		}
	}
	return false;
}

bool searchGateWays(vector<tableRow> routingtable, string address) {
	for(uint32_t i = 0; i < routingtable.size(); i++) {
		if(address == routingtable[i].gateway) {
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
	if (!(searchGateWays(routingtable, gateway))) {
		temptable.gateway = gateway;
		temptable.destaddress = gateway;
		temptable.delay = delay;
        routingtable.push_back(temptable);
    }

	//if neighbor exists, update delay
	else {
    	int i = gatewayIndex(routingtable, gateway);
    		routingtable[i].delay = delay;
    }
	//check the contents of the routing packet
	for(int i = 0; i < rp.nentries; i++) {
		//ignore distance to own router
		if(rp.entries[i].destaddress != myadr) {
			delay = rp.delay + rp.entries[i].estdelay;
			string destination = rp.entries[i].destaddress;
			//if entry doesn't exist, push new entry into vector
			if (!(searchTable(routingtable, destination, gateway))) {
				temptable.gateway = gateway;
				temptable.destaddress = rp.entries[i].destaddress;
				temptable.delay = delay;
				routingtable.push_back(temptable);
			}
			//update delay
			else {
				int index = tableIndex(routingtable, rp.entries[i].destaddress, gateway);
					delay = rp.delay + rp.entries[i].estdelay;
					routingtable[index].delay = delay;
			}
		}
	}
	output.push_back("THANK YOU");
}

//check data packet
void checkRoute(Datapacket dp, vector<tableRow> &routingtable, vector<string> &output) {
	string destination = dp.destaddress;
	//check if destination exists in routing table, save output to vector
	vector<tableRow> possibleRoutes;
	for (uint32_t i = 0; i < routingtable.size(); i++) {
		if (routingtable[i].destaddress == destination) {
			possibleRoutes.push_back(routingtable[i]);
		}
	}
	if (possibleRoutes.size() == 0) {
		output.push_back("NO ROUTE");
		return;
	}

	int smallestDelay = possibleRoutes[0].delay;
	int index = 0;
	for (uint32_t i = 0; i < possibleRoutes.size(); i++) {
		if (smallestDelay > possibleRoutes[i].delay) {
			index = i;
		}
	}
	string delay = to_string(possibleRoutes[index].delay);
	string gateway = possibleRoutes[index].gateway + " " + delay;
	output.push_back(gateway);
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
	string entrysum; //all entries on a routing packet
	for (int i = 0; i < packamount; i++) {
		string temp;
		cin >> packtype;
		switch(packtype) {
		case 'D' :
			cin >> dp.srcaddress;
        	cin >> dp.destaddress;
        	getline(cin, dp.content);
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
        	rp.entries.clear();
    		break;
		default :
			string temp;
			getline(cin, temp);
			break;
		}
	}
	for(uint32_t i = 0; i < output.size(); i++){
		cout << output[i] << endl;
	}
    return 0;
}
