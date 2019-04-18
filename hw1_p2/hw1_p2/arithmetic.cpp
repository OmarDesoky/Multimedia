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
	// initalizing frequency of uniq symbols for each block
	for(int i=0;i<msg.size();i+=num_symbols_per_block)
	{
		map<unsigned char,double> dic_frequency;
		int j=0;
		for(j;j<num_symbols_per_block && (j+i) < msg.size() ;j++)
		{
			dic_frequency[msg[j+i]]++;
		}
		
		for(map<unsigned char,double> ::iterator it = dic_frequency.begin(); it !=dic_frequency.end() ;it++)
		{
			// div by num_symbols to get probability of each symbols (PDF)
			it->second = it->second/(j);
			// calcuating CDF
			if(it!=dic_frequency.begin())
			{
				map<unsigned char,double> ::iterator previous;
				previous = it;
				previous--;
				it->second = it->second + previous->second;
			}
		}
		dictionary.push_back(dic_frequency);
	}
	int num_bits =0;
	// for looping through msg
	int k=0;
	// performing arthimetic encoding
	for(int i=0;i<dictionary.size();i++)
	{
		double l = 0;
		double u = dictionary[i].at(msg[k++]);

		for(int j=1;j<num_symbols_per_block && k<msg.size();j++)
		{
			map<unsigned char,double> ::iterator cdf;
			cdf = dictionary[i].find(msg[k]);
			double l_old=l;
			double u_old=u;
			u = l_old + (u_old-l_old)* cdf->second;
			cdf--;
			// if not the first symbol in the dictionary
			if( msg[k] != dictionary[i].begin()->first)
				l = l_old + (u_old-l_old)* cdf->second;
			k++;
		}
		double  tag = (l+u)/2.0;
		tag = floor(tag * 10000.) / 10000.;
		bitset<32> encoded_block = decimal_to_binary(tag);
		encoded_msg->push_back(encoded_block);
		for(int e=0;e<encoded_block.size();e++)
		{
			if(encoded_block[e]==1)
			{
				num_bits += 32-e;
				break;
			}
		}
	}
	return num_bits;
}