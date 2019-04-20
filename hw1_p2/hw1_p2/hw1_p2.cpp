#include <iostream>
#include "arithmetic.h"

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

// form a encoded file
void write_encode(string filename_enc,const vector<bitset<32>>& encoded_msg)
{
	filename_enc  = filename_enc.substr(0,filename_enc.size()-4);
	ofstream outfile;
	filename_enc+=".enc";
	outfile.open(filename_enc);
	for(int i=0;i<encoded_msg.size();i++)
	{
		int j=0;
		for(j;j<encoded_msg[i].size()-1;j++)
		{
			outfile<<encoded_msg[i].at(j)<<" ";
		}
		outfile<<encoded_msg[i].at(j)<<"\n";
	}

}

//form a decoded file
void write_decode(string filename_dec,const vector<unsigned char>& decoded_msg,int xres,int yres,int max)
{
	filename_dec  = filename_dec.substr(0,filename_dec.size()-4);
	ofstream outfile;
	filename_dec+=".pgm";
	outfile.open(filename_dec);
	outfile<<"ï»¿P2\n";
	outfile<<xres<<" "<<yres<<"\n";
	outfile<<max<<"\n";
	int i=0;
	for(i;i<decoded_msg.size()-1;i++)
	{
		outfile<<int(decoded_msg[i])<<" ";
	}
	outfile<<int(decoded_msg[i])<<"\n";
}

void input_hex(bitset<32>&fraction)
{
	// get input as hex and save them in x
		long long x;
		cin >> hex >> x;
		//convert x from dec to bin(string)
		char result[50];
		_itoa_s(x,result,2);
		// put data in fraction from result arr
		
		fraction.reset();
		for(int i=0;i<32;i++)
		{
			if(result[31-i] =='1')
				fraction[i].flip();
		}
}

int main ()
{
	string operation;
	//cin>>operation;
	Arithmetic arithmetic;

	//if (operation=="-bin_to_dec")
	//{
	//	
	//	bitset<32> fraction;
	//	input_hex(fraction);
	//	cout << arithmetic.binary_to_decimal(fraction)<<endl;
	//	return 0;
	//}

	//if (operation=="-dec_to_bin")
	//{
	//	
	//	double dec;
	//	cin>>dec;
	//	bitset<32>binary= arithmetic.decimal_to_binary(dec);
	//	cout<<"0x"<<hex<<binary.to_ullong();
	//	return 0;
	//}


	vector<bitset<32>> encoded_msg; 
	//if (operation =="-encode")
	//{
		int m;
		cin>>m;
		//char arg;
		//cin>>arg;
								//if(arg != '<')
								//{
								//	return 0;
								//}
		string filename;
		cin>>filename;
		vector<unsigned char> msg = read(filename);
		int after = arithmetic.encode(msg,m,&encoded_msg);
		cout<<endl;
		cout<<"Number of symbols per tag: "<<after<<endl;
		cout<<"Compression ratio: ";
		cout<<(float(after)/(8*rows*cols))<<endl;
		write_encode(filename,encoded_msg);
	
	//}

	/*string flag;
	cin>>flag;
	if(flag=="|")
	{

	}*/

	vector<unsigned char> decoded_msg; 
	//if(operation == "-decode")
	//{
		//int m;
		cin>>m;
		//char arg;
		//cin>>arg;
		int xres,yres,max;
		cin>>xres>>yres>>max;
		//							//if(arg != '<')
		//							//{
		//							//	return 0;
		//							//}

		arithmetic.decode(encoded_msg,m,&decoded_msg);
		cout<<endl;
		write_decode(filename,decoded_msg,xres,yres,max);

	//}

}