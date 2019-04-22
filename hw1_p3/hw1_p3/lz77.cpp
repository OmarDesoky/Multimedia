#include "lz77.h"

int LZ77 :: encode(const vector<unsigned char>& msg,int S, int T, int A, vector<Triplet>* encoded_msg)
{
	int num_bits=0;

		// for a single line
		
		// to be able to use string operations
		string line="";
		for(int j=0;j<msg.size();j++)
		{
			line+=msg[j];
		}
		//------------------------------
		// for first element
		Triplet element;
		element.k=0;
		element.j=0;
		element.c=line[0];
		encoded_msg->push_back(element);
		//-----------------------------
		int start_search_buffer=0; //start from index =0
		int end_search_buffer =0;  //index of end
		int start_look_ahead=1; // start from index 1
		//pointers to traverse
		int la_pointer =start_look_ahead;
		int search_pointer =start_search_buffer;
		//perform lz77
		while(la_pointer < msg.size())
		{
			//find greatest match in the search buffer
			bool first_time=true;//to check if the block exist in the search buffer
			unsigned offset=0;
			unsigned size=0;
			unsigned char c;
			while((la_pointer+size)<line.size())
			{
				string str_to_test = line.substr(start_look_ahead,size+1);
				string temp_sr_buffer=line.substr(start_search_buffer,end_search_buffer-start_look_ahead+1);// temp search buffer							to do //start =s-end if -ve so zero
				int location=temp_sr_buffer.rfind(str_to_test);
				if(location==-1)
					break;
				first_time=false;
				offset=(temp_sr_buffer.size() -location);
				size++;
			}
			if(!first_time)
			{
				// see if there is a more match outside the search buffer
				la_pointer+=size;
				search_pointer = la_pointer-offset;
				while(search_pointer<(T+la_pointer-size)&& ( (la_pointer+size)<line.size() ) ) // search pointer does not exceed look ahead buffer
				{
					if(line[la_pointer]==line[search_pointer])
					{
						size++;
						la_pointer++;
						search_pointer++;
						continue;
					}
					break;
				}
			}
			if(la_pointer > msg.size() ) // reached end of line
				break;
			if(la_pointer==msg.size())
				c='$';
			else
				c = line[la_pointer];
			la_pointer++;
			// slide search and look ahead
			end_search_buffer+=size+1;//+1 because we take unmatched one
			if((end_search_buffer+1) >=S)
			{
				start_search_buffer=end_search_buffer-S+1;
			}
			start_look_ahead+=size+1;
			element.j=offset;
			element.k=size;
			element.c=c;
			encoded_msg->push_back(element);
			if(encoded_msg->size()==304)
				int x =1;
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
			decoded_msg->push_back(decoded_msg->at(j+it-encoded_msg[i].j));
		}
		it+=j;
		if(encoded_msg[i].j!=0 && encoded_msg[i].k !=0 && encoded_msg[i].c =='$')
			continue;
		//if(encoded_msg[i].c=='$')
		//	continue;
		decoded_msg->push_back(encoded_msg[i].c);
		it++;
	}
}