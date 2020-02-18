
#include <iostream>
#include <string>
#include <vector>

//a table struct to store a row of a routing table
struct tableEntry {

	std::uint32_t  Ip = 0;
	std::uint32_t  subnet = 0;
	std::string gateway;
	tableEntry() {}
};



int main() {

	int a,b;
	//a is the loops for routing table input
	//b is the loops of the packets to be routed
	std::cin >> a >> b;

	std::vector<tableEntry> routring_table;
	int octets[4];


	for (int h = 0; h < a; h++) {

		std::uint32_t ip = 0;
		std::uint32_t  subnet = 0;
		tableEntry temp = tableEntry();

		//retrieve IP and turns it to 32bit integer
		for(int i = 0; i < 4; ++i) {
			if(i != 0) std::cin.get();
			std::cin >> octets[i];
			ip <<= 8u;
			ip += octets[i];
		}
		//retrieve Subnet mask and turns it to 32bit integer
		for(int i = 0; i < 4; ++i) {
			if(i != 0) std::cin.get();
			std::cin >> octets[i];
			subnet <<= 8u;
			subnet += octets[i];
		}

		std::string gateway;
		std::cin >> gateway;


		ip = ip & subnet; // bitwise AND on IP and subnet.
		temp.Ip = ip;
		temp.subnet = subnet;
		temp.gateway = gateway;
		routring_table.push_back(temp);


	}


	for (int i = 0; i < b; i++) {

		std::uint32_t destIp = 0;
		std::uint32_t bestSubnet = 0;
		int bestGateway = -1;


		for(int i = 0; i < 4; ++i) {
			if(i != 0) std::cin.get();
			std::cin >> octets[i];
			destIp <<= 8u;
			destIp += octets[i];
		}

		for (int h = 0; h < a; h++) {
			std::uint32_t tempIp = destIp & routring_table[h].subnet;

			if (tempIp == routring_table[h].Ip && routring_table[h].subnet >= bestSubnet) {
				bestSubnet = routring_table[h].subnet;
				bestGateway = h;

			}
		}
		if (bestGateway != -1)
			std::cout << routring_table[bestGateway].gateway << std::endl;
		else
			std::cout << "ERROR";
	}



	return 0;
}
