#include "lz77.h"
#include <string.h>

int rows;
int cols;

vector<unsigned char> read()
{
	int need_values=0; // start reading after max intensity input
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
void write_encode(const vector<Triplet>& encoded_msg)
{
	cout<<encoded_msg.size()<<endl;
	for(int i=0;i<encoded_msg.size();i++)
	{
		cout<<int(encoded_msg[i].c)<<" "<<encoded_msg[i].j<<" "<<encoded_msg[i].k<<"\n";
	}
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

// form encoded vector
vector<Triplet> read_encode()
{
	string val;
	getline(cin,val);
	getline(cin,val);
	vector<Triplet> encoded_msg;
	int size ;
	cin>>size;
	for(int i=0;i<size;i++)
	{
		Triplet temp;
		int c;
		cin>>c;
		temp.c=char(c);
		cin>>temp.j>>temp.k;
		encoded_msg.push_back(temp);
	}
	return encoded_msg;
}

vector<unsigned char> decorrelation(const vector<unsigned char>&msg)
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

vector<unsigned char> inverse_decorrelation(vector<unsigned char>&msg,int xres,int yres)
{
	vector<unsigned char> msg_decorr;
	int k=0;
	for(int i=0;i<yres;i++)
	{
		for(int j=0;j<xres;j++)
		{
			if(k>=msg.size())
				break;
			k++;
			if(j==0)
			{
				msg_decorr.push_back(msg[j+i*xres]);
				continue;
			}
			else
			{
				msg[j+i*xres] = msg[j+i*xres]+msg[j-1+i*xres];
				msg_decorr.push_back(msg[j+i*xres]);
			}
		}
		if(k>=msg.size())
			break;
	}
	return msg_decorr;
}

int main(int argc, char* argv[]) 
{
	LZ77 lz77;
	bool with_diff = false;		// decorrelation if True
	if (argc > 2 && strncmp(argv[2], "-diff", 5) == 0)
	{
		with_diff = true;
	}
	vector<Triplet>encoded_msg;
	vector<unsigned char> decoded_msg;
	vector<unsigned char> msg_decorr ; 
												// encoding 
	if (strncmp(argv[1], "-encode", 7) == 0)
	{
		vector<unsigned char> msg = read();
		if(with_diff == false)
		{
			if(argc !=5)
			{
				return 0;
			}
			string x =argv[2];
			stringstream str_to_int1(x);
			int S ;
			str_to_int1>>S;

			string y =argv[3];
			stringstream str_to_int2(y);
			int T ;
			str_to_int2>>T;

			string z =argv[4];
			stringstream str_to_int3(z);
			int A ;
			str_to_int3>>A;

			int num_bits = lz77.encode(msg, S, T, A,&encoded_msg);
			cout<<"Number of bits : "<<num_bits<<endl;
			cout<<"Compresion ratio : "<<float(8*rows*cols)/num_bits<<endl;
			write_encode(encoded_msg);
		}
		else
		{
			if(argc !=6)
			{
				return 0;
			}
			string x =argv[3];
			stringstream str_to_int1(x);
			int S ;
			str_to_int1>>S;

			string y =argv[4];
			stringstream str_to_int2(y);
			int T ;
			str_to_int2>>T;

			string z =argv[5];
			stringstream str_to_int3(z);
			int A ;
			str_to_int3>>A;

			msg_decorr = decorrelation(msg);
			int num_bits =  lz77.encode(msg_decorr,S,T,A,&encoded_msg);
			cout<<"number of bits:"<<num_bits<<endl;
			cout<<"Compresion ratio : "<<float(8*rows*cols)/num_bits<<endl;
			write_encode(encoded_msg);
		}
		return 0;
	}
											/// decoding
	if(strncmp(argv[1], "-decode",7) == 0)
	{
		if(with_diff ==false)
		{
			if(argc !=5)
			{
				return 0;
			}
			string x=argv[2];
			stringstream str_to_int1(x);
			int xres;
			str_to_int1>>xres;

			string y=argv[3];
			stringstream str_to_int2(y);
			int yres;
			str_to_int2>>yres;

			string z=argv[4];
			stringstream str_to_int3(z);
			int max;
			str_to_int3>>max;
			encoded_msg = read_encode();
			lz77.decode(encoded_msg,&decoded_msg);
			write_decode(decoded_msg,xres,yres,max);
		}
		else
		{
			if(argc !=6)
			{
				return 0;
			}
			string x=argv[3];
			stringstream str_to_int1(x);
			int xres;
			str_to_int1>>xres;

			string y=argv[4];
			stringstream str_to_int2(y);
			int yres;
			str_to_int2>>yres;

			string z=argv[5];
			stringstream str_to_int3(z);
			int max;
			str_to_int3>>max;
			encoded_msg = read_encode(); 
			lz77.decode(encoded_msg,&decoded_msg);
			vector<unsigned char> decoded_msg_after = inverse_decorrelation(decoded_msg,xres,yres);
			write_decode(decoded_msg_after,xres,yres,max);
		}
		return 0;
	}
	return 0;
}
