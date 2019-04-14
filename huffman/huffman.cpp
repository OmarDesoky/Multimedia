#include "huffman.h"


// Your implementations here

bool compare(Node* a,Node* b)
{
	return a->symbol.probability > b->symbol.probability;
}

float Huffman :: compute_entropy(const vector<unsigned char>& msg,vector<Symbol>* prob)
{
	map<unsigned char,int>counter;
	for(int i=0;i < msg.size();i++)
	{
		counter[msg[i]]++;
	}
	map<unsigned char, int>::iterator it;
	float entropy=0;
	for (it = counter.begin(); it != counter.end(); it++)
    {
		Symbol s;
		s.val=it->first;
		s.probability=(it->second)/float(msg.size());
		prob->push_back(s);
		entropy += (s.probability)*( (logf(s.probability)/logf(2)) );
    }
	entropy = - entropy;
	
	return entropy;
}


// building code table
void  build_code_table(Node* tree,map<unsigned char,unsigned int> &code_table,string s,string code,map<unsigned char,int> &leng)
{
	if(s=="left")
	{
		code+="0";
	}
	else if(s=="right")
	{
		code+="1";
	}
	if(tree==nullptr)
		return;
	build_code_table(tree->left,code_table,"left",code,leng);
	build_code_table(tree->right,code_table,"right",code,leng);
	if(tree->left==nullptr && tree->right ==nullptr )
	{
		stringstream str_to_int(code);
		int x;
		str_to_int >> x;
		leng[tree->symbol.val]=code.size();
		code_table[tree->symbol.val]=x;
	}
}

void Huffman :: build_tree(const vector<Symbol>& prob)
{
	//building tree
	vector<Node*> q;
	for(int i=0;i < prob.size();i++)
	{
		Node * n= new Node();
		n->symbol.probability=prob[i].probability;
		n->symbol.val=prob[i].val;
		q.push_back(n);
	}
	sort(q.begin(),q.end(),compare);
	while(q.size() >1)
	{
		Node* min1 = q[q.size()-1];
		q.pop_back();
		Node* min2 = q[q.size()-1];
		q.pop_back();
		
		Node* created = new Node();
		created->symbol.probability = min1->symbol.probability + min2->symbol.probability;
		created->left=min1;
		created->right=min2;
		q.push_back(created);
		sort(q.begin(),q.end(),compare);
	}
	tree = q[0] ;
	build_code_table(tree,code_table,"begin","",leng);
}

void Huffman :: print_code_table()
{
	map<unsigned char, unsigned int>::iterator it;
	for (it = code_table.begin(); it != code_table.end(); it++)
    {
		cout<<int(it->first)<<": "<<it->second<<endl;
    }
}


int Huffman :: encode(const vector<unsigned char>& msg,vector<unsigned int>* encoded_msg)
{
	int avg_code_length=0;
	for(int i=0;i<msg.size();i++)
	{
		unsigned int x =code_table[msg[i]];
		avg_code_length += leng[msg[i]];
		encoded_msg->push_back(x);
	}
	return avg_code_length;
}