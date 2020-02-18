#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <streambuf>
#include "math.h"
#include <vector>
#include <cstdlib>

using namespace std;


//incoming frame class
struct frame{
public:
    int port;
    string srcadr;
    string destadr;
    string len;
    string payload;
    string checksum;

};

//class for source address/port elements on switch table
class addresses{

    public:
    int port;
    string srcadr;

    addresses(int port, string srcadr): port(port), srcadr(srcadr){}

    bool operator==(const addresses & a2) const
	{
		if(this->srcadr.compare(a2.srcadr) == 0 && this->port==a2.port)
			return true;
		else
			return false;
	}

    bool operator==(string destadr) const
	{
		if(this->srcadr.compare(destadr) == 0)
			return true;
		else
			return false;
	}

};

class IncGenerator {
    public:
        int current_;
        IncGenerator (int start) : current_(start) {}
        int operator() () { return current_++; }
};


//function converter from hexadecimal to decimal
int hextodec(string hex){

    int result = 0;
    for (unsigned int i = 0; i < hex.size() ; i++) {
        if (hex[i]>=48 && hex[i]<=57) {
            result += (hex[i]-48)*pow(16,hex.length()-i-1);
        } else if (hex[i]>=65 && hex[i]<=70) {
            result += (hex[i]-55)*pow(16,hex.length( )-i-1);
        } else if (hex[i]>=97 && hex[i]<=102) {
            result += (hex[i]-87)*pow(16,hex.length()-i-1);
        }
    }
    return result;
}

//add source address & port on table
void buildTable(vector<addresses> &table, frame frame){
   if (table.size()==0){
       table.push_back(addresses(frame.port,frame.srcadr));
    } else if (find(table.begin(),table.end(),addresses(frame.port,frame.srcadr)) == table.end()) {
        table.push_back(addresses(frame.port,frame.srcadr));
    }
}



string checksum(string frame) {
    int sum = 0;
    string temp;

    for(unsigned int i = 0; i < frame.size() ; i += 2){
       sum += hextodec(temp+frame.at(i)+frame.at(i+1));
    }

    stringstream ss;
    ss << hex << sum % 128;
    string s(ss.str());

   return s;
}



//function to flood frames, forward to specific port or no port at all
void forwardFrames(vector<addresses> &table, frame f,int ports){
	
	
	if(stoi(f.checksum,0,16)!=stoi(checksum(f.payload),0,16)) {
              cout<<"ERROR"<<endl;
			  return;
        }
	
    vector<int> links(ports);
    IncGenerator g (0);
    generate( links.begin(), links.end(), g);

	
    if (find(table.begin(),table.end(),addresses(f.port,f.destadr))!= table.end()) {

        cout<<f.destadr<<f.srcadr<<f.len<<f.payload<<f.checksum<<endl;

    } else if (find(table.begin(),table.end(),f.destadr) == table.end()) {
        cout<<f.destadr<<f.srcadr<<f.len<<f.payload<<f.checksum<<" ";

        for(unsigned int i=0; i < links.size(); i++){
            if(links[i]!=f.port)
            cout<<links[i]<<" ";
        }
        cout<<endl;

    } else {

        for(unsigned int i=0; i <table.size(); i++) {
            if(f.destadr == table[i].srcadr) {
                cout<<f.destadr<<f.srcadr<<f.len<<f.payload<<f.checksum<<" "<<table[i].port<<endl;
                break;
            }
        }
    }
}














int main(){


    int l;
    int m;
    vector<addresses> table;



	cin >> l;
    cin >> m;



	frame f;
	char c;
	int counter = -2;
	int messageNum = 0;
	while (messageNum < m) {
        cin >> c;

        if (!cin) {
        	break;
        }

		counter++;
		if(counter == -1) {
			f.port = c-48;

		}
		else if (counter < 4) {
		    f.destadr.push_back(c);

		}
		else if ( counter < 8) {
		    f.srcadr.push_back(c);

		}
        else if( counter < 12){
            f.len.push_back(c);

        }
		else if(counter < (hextodec(f.len)*2) - 2){
		    f.payload.push_back(c);
            

		}
		else if (counter < (hextodec(f.len)*2)){
            f.checksum.push_back(c);

            if( counter == (hextodec(f.len)*2)-1)  {

				buildTable(table,f);
				forwardFrames(table,f,l);
                //cout<<(hextodec(f.len)*2) - 2<<" "<<"len";
                
					f.destadr = "";
					f.srcadr = "";
					f.len = "";
					f.payload = "";
					f.checksum = "";
					counter = -2;
					messageNum++;
			}
		}


	   }



	return 0;
}


