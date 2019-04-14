#ifndef _HUFFMAN_H
#define _HUFFMAN_H
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <iterator>
#include <sstream> 
using namespace std;



// Define a symbol struct here to hold the symbol (grayscale value)
// and its probability/frequency
struct Symbol {
  // Grayscale value of the symbol
  unsigned char val;
  float probability;
    Symbol()
  {
	  probability=0;
  }
};

// Define the tree node struct here ...
struct Node {

  Symbol symbol;
  Node* left;
  Node* right;
  Node()
  {
	  left = nullptr;
	  right = nullptr;
  }
};

class Huffman {
public:
  // Constructor and destructor

  // A function to compute the entropy of the input message and
  // stores the probabilities of the different symbols in
  // the input array.
  float compute_entropy(const vector<unsigned char>& msg,
                        vector<Symbol>* prob);
  
  // A function to build the Huffman tree given the computed
  // symbol probabilities.
  void build_tree(const vector<Symbol>& prob);
  
  // A function to print the code table computed from the build_tree above.
  void print_code_table();
  
  // A function to encode the input message using the computed tree 
  // and store the output in the output array, where each entry
  // corresponds to the codeword for an input symbol in the message.
  //
  // The function should return the number of bits in the encoded 
  // message.
  int encode(const vector<unsigned char>& msg,
             vector<unsigned int>* encoded_msg);
              
private:
  // Data members here ... for example to store the tree and the codeword 
  // hash table
  Node* tree;
  map<unsigned char,unsigned int> code_table;
};

#endif