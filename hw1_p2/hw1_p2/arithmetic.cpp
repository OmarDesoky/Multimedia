// Your includes here
#include "arithmetic.h"

// Your implementations here

bool compare(pair<unsigned char ,double>x ,pair<unsigned char,double>y)
{
	return x.second<y.second;
}

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


int Arithmetic ::encode(const vector<unsigned char>& msg, int num_symbols_per_block, vector<bitset<32> >* encoded_msg)
{
	// initalizing frequency of uniq symbols
	map<unsigned char,double> propability;
	int i=0;
	for(i;i<msg.size();i++)
	{
		propability[msg[i]]++;
	}
	for(map<unsigned char,double> ::iterator it = propability.begin(); it !=propability.end() ;it++)
	{
		// div by num_symbols to get probability of each symbols (PDF)
		it->second = it->second/(i);
		prob_table.push_back(make_pair(it->first,it->second));
	}

	sort(prob_table.begin(),prob_table.end(),compare);
	
	for(int j=0;j<prob_table.size();j++)
	{
		// calcuating CDF
		if(j!=0)
		{
			prob_table[j].second+=prob_table[j-1].second;
		}
	}
		
	int num_bits =0;

	for(int i=0;i<msg.size();i=i+num_symbols_per_block)
	{
		double l,u;
		
		for(int j=0;j<num_symbols_per_block && i<msg.size();j++)
		{
			double cdf_prev,cdf;
			for(int b=0;b<prob_table.size();b++)
			{
				if(msg[i]==prob_table[b].first)
				{
					if(b==0)
					{
						cdf_prev=0;
						cdf=prob_table[0].second;
						break;
					}
					else
					{
						cdf_prev=prob_table[b-1].second;
						cdf=prob_table[b].second;
						break;
					}
				}
			}
			if(j==0)// first time to intialize l and u
			{
				l=cdf_prev;
				u=cdf;
				continue;
			}
			double l_old=l;
			double u_old=u;
			l=l_old + (u_old-l_old)*cdf_prev;
			u = l_old + (u_old-l_old)* cdf;
		}
		double  tag = (l+u)/2.0;
		cout<<endl;
		cout<<tag<<endl;
		bitset<32> encoded_block = decimal_to_binary(tag);
		encoded_msg->push_back(encoded_block);

		num_bits+=32;
		for(int e=31;e>=0;e--)
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


//int Arithmetic :: encode(const vector<unsigned char>& msg, int num_symbols_per_block, vector<bitset<32> >* encoded_msg)
//{
//	// initalizing frequency of uniq symbols for each block
//	for(int i=0;i<msg.size();i+=num_symbols_per_block)
//	{
//		map<unsigned char,double> dic_frequency;
//		int j=0;
//		for(j;j<num_symbols_per_block && (j+i) < msg.size() ;j++)
//		{
//			dic_frequency[msg[j+i]]++;
//		}
//
//		for(map<unsigned char,double> ::iterator it = dic_frequency.begin(); it !=dic_frequency.end() ;it++)
//		{
//			// div by num_symbols to get probability of each symbols (PDF)
//			it->second = it->second/(j);
//			// calcuating CDF
//			if(it!=dic_frequency.begin())
//			{
//				map<unsigned char,double> ::iterator previous;
//				previous = it;
//				previous--;
//				it->second = it->second + previous->second;
//			}
//		}
//
//		dictionary.push_back(dic_frequency);
//	}
//	int num_bits =0;
//	// for looping through msg
//	int k=0;
//	// performing arthimetic encoding
//	for(int i=0;i<dictionary.size();i++)
//	{
//		double l,u;
//		if(msg[k] == dictionary[i].begin()->first)// first symbol
//		{
//			l = 0;
//		}
//		else
//		{
//			map<unsigned char,double> ::iterator it= dictionary[i].find(msg[k]);
//			l = it->second;
//			it--;
//			l -=it->second;
//		}
//		u =dictionary[i].at(msg[k++]);
//		for(int j=1;j<num_symbols_per_block && k<msg.size();j++)
//		{
//			map<unsigned char,double> ::iterator cdf;
//			cdf = dictionary[i].find(msg[k]);
//			double l_old=l;
//			double u_old=u;
//			u = l_old + (u_old-l_old)* cdf->second;
//			cdf--;
//			// if not the first symbol in the dictionary
//			if( msg[k] != dictionary[i].begin()->first)
//				l = l_old + (u_old-l_old)* cdf->second;
//			k++;
//		}
//		double  tag = (l+u)/2.0;
//		tag = floor(tag * 10000.) / 10000.;
//		bitset<32> encoded_block = decimal_to_binary(tag);
//		encoded_msg->push_back(encoded_block);
//
//		num_bits+=32;
//		for(int e=31;e>=0;e--)
//		{
//			if(encoded_block[e]!=1)
//			{
//				num_bits --;
//			}
//			else 
//			{
//				break;
//			}
//		}
//	}
//	return num_bits;
//}


//void Arithmetic :: decode(const vector<bitset<32>>& encoded_msg, int num_symbols_per_block, vector<unsigned char>* decoded_msg)
//{
//	for(int i=0;i<encoded_msg.size();i++)
//	{
//		double tag = binary_to_decimal(encoded_msg[i]);
//		
//		double l_old=0;
//		double u_old=1;
//		for(int j=0;j<num_symbols_per_block;j++)
//		{
//			double t = (tag-l_old)/(u_old-l_old);
//			map<unsigned char,double> ::iterator it;
//			for(it =propability.begin() ;it!=propability.end();it++)
//			{
//				if(it->second >= t)
//				{
//					decoded_msg->push_back(it->first);
//					break;
//				}
//			}
//			double u=l_old + (u_old-l_old)*(it->second);
//			u_old=u;
//			if(it!=propability.begin())
//			{
//				it--;
//				double l=l_old + (u_old-l_old)*(it->second);
//				l_old=l;
//			}
//		}
//	}
//}

//void Arithmetic :: decode(const vector<bitset<32>>& encoded_msg, int num_symbols_per_block, vector<unsigned char>* decoded_msg)
//{
//	for(int i=0;i<encoded_msg.size();i++)
//	{
//		double tag = binary_to_decimal(encoded_msg[i]);
//
//		double l_old=0;
//		double u_old=1;
//
//		for(int j=0;j<num_symbols_per_block;j++)
//		{
//			double t = (tag-l_old)/(u_old-l_old);
//			map<unsigned char,double> ::iterator it;
//			for(it =dictionary[i].begin() ;it!=dictionary[i].end();it++)
//			{
//				if(it->second >= t)
//				{
//					decoded_msg->push_back(it->first);
//
//					break;
//				}
//			}
//			double u=l_old + (u_old-l_old)*(it->second);
//			u_old=u;
//			if(it!=dictionary[i].begin())
//			{
//				it--;
//				double l=l_old + (u_old-l_old)*(it->second);
//				l_old=l;
//			}
//			//if (l_old == 0 && u_old==1)
//			//{
//			//	break;
//			//}
//		}
//	}
//}