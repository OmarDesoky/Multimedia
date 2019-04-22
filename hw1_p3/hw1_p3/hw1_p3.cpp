#include "lz77.h"

int rows;
int cols;

vector<unsigned char> read(string filename)
{
	string base="data/";
	base+=filename;
	ifstream myfile;
	myfile.open(base);
	int need_values=0; // start reading after max intensity input
	string val;
	getline(myfile,val);
	while(val!="255")	//ignoring inputs until max intensity 
	{
		if(val[0] =='#')
		{
			val="";
			continue;
		}
		else
		{
			int x=val.find(" ");
			if(x != -1)
			{
				string c=val.substr(0,x);
				string r=val.substr(x+1,val.size()-1);
				stringstream str_to_intr(r);
				str_to_intr>>rows;
				stringstream str_to_intc(c);
				str_to_intc>>cols;
		
			}
		}
		getline(myfile,val);
	}
	val="";
	vector<unsigned char> symbols;
	while(myfile)
	{
		int val;
		myfile>>val;
		symbols.push_back(unsigned char(val) );
	}
	myfile.close();
	symbols.pop_back();
	return symbols;
}

// form a encoded file
void write_encode(string filename_enc,const vector<Triplet>& encoded_msg)
{
	filename_enc  = filename_enc.substr(0,filename_enc.size()-4);
	ofstream outfile;
	filename_enc+=".enc";
	outfile.open(filename_enc);
	for(int i=0;i<encoded_msg.size();i++)
	{
		outfile<<encoded_msg[i].c<<" "<<encoded_msg[i].j<<" "<<encoded_msg[i].k<<"\n";
	}
}

//form a decoded file
void write_decode(string filename_dec,const vector<unsigned char>& decoded_msg,int xres,int yres,int max)
{
	filename_dec  = filename_dec.substr(0,filename_dec.size()-4);
	ofstream outfile;
	filename_dec+=".pgm";
	outfile.open(filename_dec);
	outfile<<"﻿P2\n";
	outfile<<xres<<" "<<yres<<"\n";
	outfile<<max<<"\n";
	int i=0;
	for(i;i<decoded_msg.size()-1;i++)
	{
		outfile<<int(decoded_msg[i])<<" ";
	}
	outfile<<int(decoded_msg[i])<<"\n";
}





int main(int argc, char* argv[]) {

  string operation;
 // cin>>operation;
  
  LZ77 lz77;
  vector<unsigned char> msg;
  
  int S,T,A;
  cin>>S>>T>>A;
  string filename;
  cin>>filename;
  msg = read(filename);
  vector<Triplet>encoded_msg;
  //if(operation == "-encode")
  //{
	  int num_bits = lz77.encode(msg, S, T, A,&encoded_msg);
	  cout<<"number of bits:"<<num_bits<<endl<<endl;
	  cout<<"symbol goback size"<<endl;
	 // for(int i=0;i<encoded_msg.size();i++)
	 // {
	//	  cout<<int(encoded_msg[i].c)<<'\t'<<encoded_msg[i].j<<'\t'<<encoded_msg[i].k<<endl;
	 // }
	  write_encode(filename,encoded_msg);
  //}
  vector<unsigned char> decoded_msg;
  //if(operation == "-decode")
  //{
  	  int xres,yres,max;
	  cin>>xres>>yres>>max;
	  lz77.decode(encoded_msg,&decoded_msg);
	  write_decode(filename,decoded_msg,xres,yres,max);
  //}
  return 0;
}