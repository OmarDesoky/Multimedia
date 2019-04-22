#ifndef _ARITHMETIC_H
#define _ARITHMETIC_H

#include<bitset>
#include<vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>
#include <iterator>
#include <sstream> 
#include <set>
#include <iomanip>
using namespace std;


class Arithmetic {
public:
  // Constructor and destructor

  // A function to convert binary fractions to decimal. The binary fraction
  // corresponds to 0.b_31 b_30 ... b_1 b_0 where b_31 is the MSB of the 
  // bitset ... b_0 is the LSB.
  //
  // [More information on bitsets, which are useful for dealing with binary
  //  data with handy operations such as shift left, right, XOR, ...
  //  can be found at: http://www.cplusplus.com/reference/bitset/bitset/]
  //
  // e.g. fraction = 0x80000000 (i.e. 1000....0) should return 0.5 in decimal
  double binary_to_decimal(const bitset<32>& fraction);
  
  // A function to convert a decimal fraction [0, 1) to binary. The MSB
  // of the binary output is the most significant bit in the fraction after
  // the fraction point.
  //
  // e.g. an input of 0.5 should return 0x80000000 or only bitset<>[31] will
  //  be 1, and the rest will be zeros.
  bitset<32> decimal_to_binary(double fraction);

  // A function to encode the input message using arithmetic coding.
  // 
  // The function is given the msg, 
  // the number of symbols per block
  //  i.e. how many symbols are in each tag.
  // The encoded message contains the binary representation of each tag
  // i.e. one entry (tag) for every block of symbols.
  //
  // The function should return the number of bits in the encoded 
  // message.
  int encode(const vector<unsigned char>& msg,
             int num_symbols_per_block,
             vector<bitset<32> >* encoded_msg);
             
  // A function to decoded a message encoded using airhtmetic coding.
  void decode(const vector<bitset<32> >& encoded_msg,
              int num_symbols_per_block,
              vector<unsigned char>* decoded_msg);
  
                
private:
  // Data members here ...


	vector<pair<unsigned char,double>> prob_table;


	//vector<map<unsigned char,double>> dictionary;
  
};

#endif