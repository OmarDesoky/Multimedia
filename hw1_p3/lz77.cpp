#include "lz77.h"

int LZ77 ::encode(const vector<unsigned char>& msg,int S, int T, int A, vector<Triplet>* encoded_msg)
{
	int num_bits=0;
	string line="";
	for(int i=0;i<msg.size();i++)
	{
		line+=msg[i];
	}
	//------------------------------
	// for first element
	Triplet element;
	element.k=0;
	element.j=0;
	element.c=line[0];
	encoded_msg->push_back(element);
	//-----------------------------
	int start_look_ahead=1;
	int end_search=0;
	int start_search=0;
	for(int i=1;i<line.size();i++)
	{
		string str_search = line.substr(start_search,end_search-start_search+1);
		
		unsigned size=0;
		unsigned offset=0;
		unsigned char c;
		bool visted=false;
		//find greatest match in the search buffer
		int j=i;
		int location =-1;
		int startafter_pointer;
		while(j<line.size())
		{
			string str_look_ahead=line.substr(start_look_ahead,size+1);
			location =str_search.rfind(str_look_ahead);
			if(location ==-1)
				break;

			visted=true;
			offset=  i - (start_search+location) ;
			startafter_pointer= (start_search+location) ;
			size++;
			j++;
		}
		if(visted==false)
		{
			Triplet element;
			element.k= 0;
			element.j= 0;
			element.c=line[j];
			encoded_msg->push_back(element);
			//increment search and look ahead buffers
			start_look_ahead=j+1;
			end_search=j;
			if((j+1)<S)
				start_search=0;
			else
				start_search=j-S+1;
			continue;
		}

		if(j==line.size())
		{
			if(size==0 && offset ==0)
			element.k=size;
			element.j=offset;
			element.c=line[j-1];
			encoded_msg->push_back(element);
			break;
		}
		// see if there is a more match outside the search buffer
		int z=startafter_pointer+size;
		while(z<(i+T) && j<line.size())
		{
			if(line[j]!=line[z])
				break;
			
			j++;
			z++;
			size++;
		}
		Triplet element;
		element.k= size;
		element.j= offset;
		if(j==line.size())
		{
			if(size==0 && offset ==0)
			element.k=size;
			element.j=offset;
			element.c=line[j-1];
			//element.c='$';
			//encoded_msg->push_back(element);
			break;
		}
		if(z==(i+T))
		{
			if(size==0 && offset ==0)
			element.k=size;
			element.j=offset;
			element.c=line[j-1];
			encoded_msg->push_back(element);
			start_look_ahead=j+1;
			end_search=j;
			if((j+1)<S)
				start_search=0;
			else
				start_search=j-S+1;
			i=j;
			continue;
		}

		element.c=line[j];
		encoded_msg->push_back(element);
		//increment search and look ahead buffers
		start_look_ahead=j+1;
		end_search=j;
		if((j+1)<S)
			start_search=0;
		else
			start_search=j-S+1;
		i=j;
	}
	num_bits = (encoded_msg->size())* ( ceil( log(S)/log(2))+ceil(log(S+T)/log(2))+ ceil (log(A)/log(2)) );
	return num_bits;
}

void LZ77:: decode(const vector<Triplet>& encoded_msg,vector<unsigned char>* decoded_msg)
{
	int it=0;
	for(int i =0;i<encoded_msg.size();i++)
	{
		int j=0;
		for(j;j<encoded_msg[i].k;j++)
		{
			int x = decoded_msg->at(j+it-encoded_msg[i].j);
			decoded_msg->push_back(x);
		}
		it+=j;
		decoded_msg->push_back(encoded_msg[i].c);
		it++;
	}
}