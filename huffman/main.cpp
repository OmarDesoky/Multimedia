
#include <fstream>
#include "huffman.h"

int cols;
int rows;

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
				string r=val.substr(0,x);
				string c=val.substr(x+1,val.size()-1);
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


vector<unsigned char> decorrelation(vector<unsigned char>&msg)
{
	vector<unsigned char> msg_decorr;
	for(int i=0;i<cols;i++)
	{
		for(int j=0;j<rows;j++)
		{
			if(j==0)
			{
				msg_decorr.push_back(msg[j+i*rows]);
				continue;
			}
			else
			{
				
				msg_decorr.push_back(max(msg[j+i*rows]-msg[j-1+i*rows],msg[j-1+i*rows]-msg[j+i*rows]));
			}
		}
	}
	return msg_decorr;
}

int main ()
{
	string filename;
	cin>>filename;
	vector<unsigned char>msg = read(filename);
	Huffman huffman;
	vector<Symbol>prob;
	cout<< huffman.compute_entropy(msg,&prob)<<endl;
	huffman.build_tree(prob);
	huffman.print_code_table();
	vector<unsigned int>encoded;
	cout<<huffman.encode(msg,&encoded)<<endl;

	cout<<endl;

	// decorrelation
	vector<unsigned char> msg_decorr = decorrelation(msg);
	Huffman huffman_decorr;
	vector<Symbol>prob_decorr;
	cout<< huffman_decorr.compute_entropy(msg_decorr,&prob)<<endl;
	huffman_decorr.build_tree(prob);
	huffman_decorr.print_code_table();
	vector<unsigned int>encoded_decorr;
	cout<<huffman_decorr.encode(msg_decorr,&encoded_decorr)<<endl;


	int y=0;
	return 0;

}