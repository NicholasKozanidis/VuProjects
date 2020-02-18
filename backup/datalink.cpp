#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <streambuf>
#include "math.h"

using namespace std;

class bitstream{
    public:
    string destadr;
    string srcadr;
    string len;
    string payload;
    string checksum;
};


string readFile(){
    
    ifstream input;
    input.open("assignment-1-tea.input");
    
    stringstream strStream;
    strStream << input.rdbuf();
    string str = strStream.str();
    
    return str;
}

string readAddress(string file){
    string temp;
    
    for(int i=0;i<4;i++){
        temp.push_back(file.at(i));
    }
    
    return temp;
    
}

int hextodec(string hex){
    
    int result = 0;
    for (int i=0; i<hex.length(); i++) {
        if (hex[i]>=48 && hex[i]<=57)
        {
            result += (hex[i]-48)*pow(16,hex.length()-i-1);
        } else if (hex[i]>=65 && hex[i]<=70) {
            result += (hex[i]-55)*pow(16,hex.length( )-i-1);
        } else if (hex[i]>=97 && hex[i]<=102) {
            result += (hex[i]-87)*pow(16,hex.length()-i-1);
        }
    }
    return result;
}

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

string checksum(string frame){
    hextodec(frame);
    int sum=0;
    string temp;
    
    for(int i=0;i<frame.length();i+=2){
        
        sum+=hextodec(temp+frame.at(i)+frame.at(i+1));
    }
    
    stringstream ss;
    ss<<hex<<sum%128;
    string s(ss.str());
    
   return s;
}

void printFrames(bitstream frame1, bitstream frame2, string myadr){
    
    if(!frame1.destadr.empty()){
        if(myadr.compare(frame1.destadr)!=0){
            cerr<<"ADDRESS MISMATCH"<<endl;
            exit(0);
        }
            if(stoi(frame1.checksum,0,16)!=stoi(checksum(frame1.destadr+frame1.srcadr+frame1.len+frame1.payload),0,16)){
                cerr<<"ERROR"<<endl;
                exit(0);
            }
        
        cout<<frame1.destadr<<" "<<frame1.srcadr<<" "<<hextodec(frame1.len)<<" "<<hextoAscii(frame1.payload)<<endl;
    }
    
     if(!frame2.destadr.empty()){
        if(myadr.compare(frame2.destadr)!=0){
            cerr<<"ADDRESS MISMATCH"<<endl;
            exit(0);
        }
         
         if(stoi(frame2.checksum,0,16)!=stoi(checksum(frame2.destadr+frame2.srcadr+frame2.len+frame2.payload),0,16)){
                cerr<<"ERROR"<<endl;
                exit(0);
            }
         
         cout<<frame2.destadr<<" "<<frame2.srcadr<<" "<<hextodec(frame2.len)<<" "<<hextoAscii(frame2.payload)<<endl;
    }
}



int main(){
    string frames = readFile().erase(0,6);
    string myadr = readAddress(readFile());
    bitstream frame1;
    bitstream frame2;
    
    
    for(int i=0;i<frames.length();i++){
        
        for(;i<4;i++){
            frame1.destadr.push_back(frames.at(i));
        }
        
        for(;i>=4 && i<8;i++){
            frame1.srcadr.push_back(frames.at(i));
        }
        
        for(;i>=8 && i<16;i++){
            frame1.len.push_back(frames.at(i));
        }
        
        for(;i>=16 && i<(hextodec(frame1.len)*2)-2;i++){
            frame1.payload.push_back(frames.at(i));
        }
        
        for(;i>=(hextodec(frame1.len)*2)-2 && i<(hextodec(frame1.len)*2);i++){
            frame1.checksum.push_back(frames.at(i));
        }
        
        if((hextodec(frame1.len)*2)<=frames.length()){
            break;
        }
        
        for(;i>=(hextodec(frame1.len)*2) && i<(hextodec(frame1.len)*2)+4;i++){
            frame2.destadr.push_back(frames.at(i));
        }
        
        for(;i>=(hextodec(frame1.len)*2)+4 && i<(hextodec(frame1.len)*2)+8;i++){
            frame2.srcadr.push_back(frames.at(i));
        }
        
        for(;i>=(hextodec(frame1.len)*2)+8 && i<(hextodec(frame1.len)*2)+16;i++){
            frame2.len.push_back(frames.at(i));
        }
        
       for(;i>=(hextodec(frame1.len)*2)+16 && i<((hextodec(frame1.len)*2)+(hextodec(frame2.len)*2))-2;i++){
            frame2.payload.push_back(frames.at(i));
        }
        
        for(;i>=((hextodec(frame1.len)*2)+(hextodec(frame2.len)*2))-2 && i<((hextodec(frame1.len)*2)+(hextodec(frame2.len)*2));i++){
            frame2.checksum.push_back(frames.at(i));
        }
        
        
    }
    
    
    printFrames(frame1,frame2,myadr);
}