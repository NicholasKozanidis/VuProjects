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

struct IncGenerator {
    int current_;
    IncGenerator (int start) : current_(start) {}
    int operator() () { return current_++; }
};




//add source address & port on table
void buildTable(vector<addresses> &table, frame frame){
   if(table.size()==0){
       table.push_back(addresses(frame.port,frame.srcadr));
   }
    
    else if(find(table.begin(),table.end(),addresses(frame.port,frame.srcadr)) == table.end()){
        table.push_back(addresses(frame.port,frame.srcadr));
    }
}





//function to flood frames, forward to specific port or no port at all
void forwardFrames(vector<addresses> &table, frame frame,int ports){
    vector<int> links(ports);
    IncGenerator g (0);
    generate( links.begin(), links.end(), g);
    
    if(find(table.begin(),table.end(),addresses(frame.port,frame.destadr))!= table.end()){
                    
        cout<<frame.destadr<<frame.srcadr<<frame.len<<frame.payload<<frame.checksum<<endl;
                    
            }
    
        else if(find(table.begin(),table.end(),frame.destadr) == table.end()){
         cout<<frame.destadr<<frame.srcadr<<frame.len<<frame.payload<<frame.checksum<<" ";
        
            for(int i=0; i<links.size();i++){
                if(links[i]!=frame.port)
                cout<<links[i]<<" ";
            }
            cout<<endl;
        
        }
    
            else{
    
                for(int i=0;i<table.size();i++){
                    if(frame.destadr == table[i].srcadr){
                        cout<<frame.destadr<<frame.srcadr<<frame.len<<frame.payload<<frame.checksum<<" "<<table[i].port<<endl;
                            break;
                            }
                    }
            }
}







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






int main(){
    
	ifstream input;
	string file = "switch-case.input";
	input.open(file.c_str());
    int l;
    int m;
    vector<addresses> table;
    
    
	if (input) {
		input >> l;
        input >> m;
    }
	 
    
	frame frame;
	char c;
	int counter = -2;
	while (input >> c) {
        
		counter++;
        
        if(c=='\n')
        cout<<counter<<endl;
    

		if(counter == -1) {
			frame.port = c-48;
            //cout<<frame.port<<" port"<<endl;
            
		}
		else if (counter < 4) {
		    frame.destadr.push_back(c);
            //cout<<frame.destadr<<" destadr"<<endl;
            
		}
		else if ( counter < 8) {
		    frame.srcadr.push_back(c);
            //cout<<frame.srcadr<<" srcadr"<<endl;
            
		}
        else if( counter < 12){
            frame.len.push_back(c);
            //cout<<frame.len<<" len"<<endl;
            
        }
		else if(counter < (hextodec(frame.len)*2) - 2){
		    frame.payload.push_back(c);
            //cout<<frame.payload<<" payload"<<endl;
            
		}
		else if (counter < (hextodec(frame.len)*2)){
            frame.checksum.push_back(c);
            //cout<<frame.checksum<<" checksum"<<endl;
            
		
       
        //prints the first frame, resets the variables of the class, continues to next frame if any
		if( counter == (hextodec(frame.len)*2)-1)  {
            
            
            
           
            buildTable(table,frame);
            forwardFrames(table,frame,l);
            
            
        
            
           
				frame.destadr = "";
				frame.srcadr = "";
				frame.len = "";
				frame.payload = "";
				frame.checksum = "";
				counter = -2;
		}

	   }
   
    }

        
    
    
    
    

	return 0;
}
    
    
