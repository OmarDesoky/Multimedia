
#include <fstream>
#include "huffman.h" 
#include <string.h> 

int cols;
int rows;

vector<unsigned char> read()
{
	string val;
	getline(cin,val);
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
		getline(cin,val);
	}
	val="";
	vector<unsigned char> symbols;
	for(int i=0;i<rows*cols;i++)
	{
		int val;
		cin>>val;
		symbols.push_back((unsigned char)val);
	}
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
				
				msg_decorr.push_back(msg[j+i*rows]-msg[j-1+i*rows]);
			}
		}
	}
	return msg_decorr;
}



int main(int argc, char* argv[])
{
	vector<unsigned char>msg = read();
	Huffman huffman;
	bool with_diff = false;		// decorrelation if True
	vector<Symbol>prob;
	Huffman huffman_decorr;
	vector<Symbol>prob_decorr;
	vector<unsigned char> msg_decorr = decorrelation(msg);
	if (argc > 2 && strncmp(argv[2], "-diff", 5) == 0)
	{
		with_diff = true;
	}
	if (strncmp(argv[1], "-entropy", 8) == 0)
	{ 
		if(with_diff == false)
			cout<< huffman.compute_entropy(msg,&prob)<<endl;
		else
			cout<< huffman_decorr.compute_entropy(msg_decorr,&prob)<<endl;
		return 0;
	}
	if (strncmp(argv[1], "-tree", 5) == 0)
	{
		if(with_diff == false)
		{
			huffman.compute_entropy(msg,&prob);
			huffman.build_tree(prob);
			cout<<"code table:"<<endl;
			huffman.print_code_table();
		}
		else
		{
			huffman_decorr.compute_entropy(msg_decorr,&prob);
			huffman_decorr.build_tree(prob);
			cout<<"code table:"<<endl;
			huffman_decorr.print_code_table();
		}
		return 0;
	}
	if(strncmp(argv[1], "-encode",7) == 0)
	{
		if(with_diff==false)
		{
			huffman.compute_entropy(msg,&prob);
			huffman.build_tree(prob);
			cout<<"code table:"<<endl;
			huffman.print_code_table();
			vector<unsigned int>encoded;
			cout<<"num_bits:"<<" ";
			cout<<huffman.encode(msg,&encoded)<<endl;
		}
		else
		{
			huffman_decorr.compute_entropy(msg_decorr,&prob);
			huffman_decorr.build_tree(prob);
			cout<<"code table:"<<endl;
			huffman_decorr.print_code_table();
			vector<unsigned int>encoded_decorr;
			cout<<"num_bits:"<<" ";
			cout<<huffman_decorr.encode(msg_decorr,&encoded_decorr)<<endl;
		}
		return 0;
	}

}
