#include <iostream>
#include "arithmetic.h"
#include <string.h>
#include <stdio.h>
#include <iomanip>
#include <stdlib.h>
#include <wchar.h>

int rows;
int cols;

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

// form a encoded file
void write_encode(const vector<bitset<32>>& encoded_msg)
{
	cout<<encoded_msg.size()<<endl;
	for(int i=0;i<encoded_msg.size();i++)
	{
		int j=0;
		for(j;j<encoded_msg[i].size()-1;j++)
			cout<<encoded_msg[i][j]<<" ";
		cout<<encoded_msg[i][j]<<endl;
	}
}

// form encoded vector
vector<bitset<32>> read_encode()
{
	string val;
	getline(cin,val);
	getline(cin,val);
	vector<bitset<32>> encoded_msg;
	int size ;
	cin>>size;
	for(int j=0;j<size;j++)
	{
		bitset<32> temp;
		temp.reset();
		for(int i=0;i<32;i++)
		{
			char x;
			cin>>x;
			if(x=='1')
			{
				temp[i].flip();
			}
		}
		encoded_msg.push_back(temp);
	}
	return encoded_msg;
}
map<unsigned char,double> read_cdf()
{
	map<unsigned char,double> mp;
	int size ;
	cin>>size;
	for(int i=0;i<size;i++)
	{
		int x;
		double y;
		cin>>x>>y;
		mp[x]=y;
	}
	return mp;
}
//form a decoded file
void write_decode(const vector<unsigned char>& decoded_msg,int xres,int yres,int max)
{
	cout<<"P2\n";
	cout<<xres<<" "<<yres<<"\n";
	cout<<max<<"\n";
	int i=0;
	for(i;i<decoded_msg.size()-1;i++)
	{
		cout<<int(decoded_msg[i])<<" ";
	}
	cout<<int(decoded_msg[i])<<"\n";
}

int main (int argc, char* argv[])
{
	Arithmetic arithmetic;
	if (strncmp(argv[1], "-bin_to_dec", 11) == 0)
	{
		string s = argv[2];
		long long x = stoll(s,nullptr,0);
		bitset<32> fraction = bitset<32>(x);
		cout << arithmetic.binary_to_decimal(fraction)<<endl;
		return 0;
	}
	if (strncmp(argv[1], "-dec_to_bin", 11) == 0)
	{
		double dec = atof(argv[2]);
		bitset<32>binary= arithmetic.decimal_to_binary(dec);
		cout<<"0x"<<hex<<binary.to_ullong()<<endl;
		return 0;
	}
	vector<bitset<32>> encoded_msg; 
	if(argc ==3)
	{
		if (strncmp(argv[1], "-encode", 7) == 0)
		{
			string x =argv[2];
			stringstream str_to_int(x);
			int m ;
			str_to_int>>m;
			vector<unsigned char> msg = read();
			int after = arithmetic.encode(msg,m,&encoded_msg);
			cout<<"Number of symbols per tag: "<<after<<endl;
			cout<<"Compression ratio: ";
			cout<<(float(after)/(8*rows*cols))<<endl;
			write_encode(encoded_msg);
			arithmetic.print_cdf();
		}
		return 0;
	}
	vector<unsigned char> decoded_msg;
	if(argc ==6)
	{
		if(strncmp(argv[1], "-decode",7) == 0)
		{
			string x =argv[2];
			stringstream str_to_int1(x);
			int m ;
			str_to_int1>>m;

			string y=argv[3];
			stringstream str_to_int2(y);
			int xres;
			str_to_int2>>xres;

			string z=argv[4];
			stringstream str_to_int3(z);
			int yres;
			str_to_int3>>yres;
			string w=argv[5];
			stringstream str_to_int4(w);
			int max;
			str_to_int4>>max;
			encoded_msg = read_encode(); 
			arithmetic.set_cdf(read_cdf());
			arithmetic.decode(encoded_msg,m,&decoded_msg);
			write_decode(decoded_msg,xres,yres,max);
		}
	}
}
