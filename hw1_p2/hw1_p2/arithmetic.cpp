// Your includes here
#include "arithmetic.h"

// Your implementations here

double Arithmetic:: binary_to_decimal(const bitset<32>& fraction)
{
	double deciaml=0;
	for (int i=1;i<=fraction.size();i++)
	{
		deciaml+= pow(2,-i)*fraction[fraction.size()-i];
	}
	return deciaml;
}

bitset<32> Arithmetic:: decimal_to_binary(double fraction)
{
	bitset<32> binary;
	binary.reset();
	double val=fraction;
	int i=31;
	val=fraction*2;
	while((float(int(val))!=val) && (i>=0) )
	{
		if(val>=1)
		{
			val-=1;
			binary[i].flip();
		}
		i--;
		val=val*2;
	}
	if(i!=-1)
		binary[i].flip();
	return binary;
}

int Arithmetic :: encode(const vector<unsigned char>& msg, int num_symbols_per_block, vector<bitset<32> >* encoded_msg)
{
	// getting pdf
	int i=0;
	for(i;i<msg.size();i++)
	{
		cdf[msg[i]]++;
	}
	for(map<unsigned char,double> ::iterator it = cdf.begin(); it !=cdf.end() ;it++)
	{
		// div by num_symbols to get probability of each symbols (PDF)
		it->second = it->second/(i);
		// calcuating CDF
		if(it!=cdf.begin())
		{
			map<unsigned char,double> ::iterator previous;
			previous = it;
			previous--;
			it->second = it->second + previous->second;
		}
	}
	//performing arthimetic encoding
	int num_bits=0;
	//to loop through msg
	int k=0;
	for(int i=0;i<msg.size();i+=num_symbols_per_block)
	{
		double u ,l;
		for(int j=0;j<num_symbols_per_block && k<msg.size(); j++)
		{
			map<unsigned char,double> ::iterator it= cdf.find(msg[k++]);
			if(j==0)//intialize l and u
			{
				u=it->second;
				if(cdf.begin()->first == it->first)    // first symbol
				{
					l = 0;
				}
				else
				{
					it--;
					l =it->second;
					it++;
				}
				continue;
			}
			double l_old=l;
			double u_old=u;
			u = l_old + (u_old-l_old)* (it->second);
			// getting cdf_prev
			if(cdf.begin()->first != it->first) // not first symbol
			{
				it--;
				l= l_old + (u_old-l_old)* (it->second);
			}
		}
		double tag=(u+l)/2;
		num_bits+=32;
		bitset<32> encoded_block = decimal_to_binary(tag);
		encoded_msg->push_back(encoded_block);
		for(int e=0;e<32;e++)
		{
			if(encoded_block[e]!=1)
			{
				num_bits --;
			}
			else 
			{
				break;
			}
		}
	}

	return num_bits;
}

void Arithmetic :: decode(const vector<bitset<32>>& encoded_msg, int num_symbols_per_block, vector<unsigned char>* decoded_msg)
{
	for(int i=0;i<encoded_msg.size();i++)
	{
		double tag = binary_to_decimal(encoded_msg[i]);
		double l=0;
		double u=1;
		for(int j=0;j<num_symbols_per_block;j++)
		{
			double t;
			if(u==l)
				t=u;
			else
				t = (tag-l)/(u-l);
			map<unsigned char,double> ::iterator it;
			for(it =cdf.begin() ;it!=cdf.end();it++)
			{
				if(t<=it->second)
				{
					decoded_msg->push_back(it->first);
					break;
				}
			}
			//calculating new l and u
			double l_old=l;
			double u_old=u;
			u=l_old + (u_old-l_old)*(it->second);
			if(it->first != cdf.begin()->first)
			{
				it--;
				l=l_old + (u_old-l_old)*(it->second);
			}
		}
	}
}