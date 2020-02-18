
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <streambuf>
#include "math.h"
#include <vector>

using namespace std;

//class to store the frames
struct bitstream{
 public:
    string destadr;
    string srcadr;
    string len;
    string payload;
    string checksum;
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

//function converter from hexadecimal to ascii
string hextoAscii(string hex){

	int len = hex.length();
	string temp;

	for(int i=0; i< len; i+=2)
	{
		string byte = hex.substr(i,2);
		char chr = (char) (int)strtol(byte.c_str(), 0, 16);
		temp.push_back(chr);
	}
    return temp;
}

//function to calculate the checksums
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

//function to print to terminal the errors if any, and the contents of the frame
void printFrames(bitstream frame, string myadr){

    if(!frame.destadr.empty()) {
        if(myadr.compare(frame.destadr)!=0) {
            cerr<<"ADDRESS MISMATCH"<<endl;
            cout << myadr << " " << frame.destadr;
            exit(0);
        }
        if(stoi(frame.checksum,0,16)!=stoi(checksum(frame.destadr+frame.srcadr+frame.len+frame.payload),0,16)) {
              cerr<<"ERROR"<<endl;
              exit(0);
        }

        cout<<frame.destadr<<" "<<frame.srcadr<<" "<<hextodec(frame.len)<<" "<<hextoAscii(frame.payload)<<endl;

   }

}

int main () {
    
    //variables to read from file
	ifstream inData;
	string path = "assignment-1-tea.input";
	inData.open(path.c_str());
	string myadr;
    
    //first line-myadress- stored in myadr
	if (inData) {
		for (int i = 0; i < 4; i++) {
			char c;
			inData >> c;
			myadr += c;
		}
	} else {
		cerr << "No frames found";
		exit(0);
	}
    
    //loop running through all the bitsequence and getting the destination adress,source adress, length and payload
	bitstream frame;
	char c;
	int counter = -1;
	while (inData >> c) {

		counter++;

		if(counter < 4) {
			frame.destadr.push_back(c);
		}
		else if (counter < 8) {
		    frame.srcadr.push_back(c);
		}
		else if ( counter < 16) {
		    frame.len.push_back(c);
		}
		else if(counter < (hextodec(frame.len)*2) - 2){
		    frame.payload.push_back(c);
		}
		else if (counter < (hextodec(frame.len)*2)){
            frame.checksum.push_back(c);
		}
        
        //prints the first frame, resets the variables of the class, continues to next frame if any
		if( counter == (hextodec(frame.len)*2) - 1)  {
			printFrames(frame,myadr);
				frame.destadr = "";
				frame.srcadr = "";
				frame.len = "";
				frame.payload = "";
				frame.checksum = "";
				counter = -1;
		}

	}

	return 0;
}
