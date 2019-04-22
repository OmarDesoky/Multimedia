#ifndef _LZ77_H
#define _LZ77_H

using namespace std;
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

// Struct to hold the triplet for each entry
struct Triplet{
  // Starting position of the longest match in the previous string
  // i.e. number of symbols to look back.
  unsigned int j;
  
  // Length of the longest match i.e. number of symbols to copy
  // starting at the match position.
  unsigned int k;
  
  // Uncompressed code of the symbol following the match. The number
  // of bits here will depend on the size of the alphabet in use.
  unsigned char c;
};

class LZ77 {
public:
  // Constructor and destructor

  // A function to encode the input message using LZ77.
  // 
  // Inputs:
  // - the input message
  // - S the size of the match buffer i.e. the maximum distance to look
  //   backward for a match
  // - T the size of the lookahead buffer   
  // - A the size of the alphabet
  //
  // The encoded message contains the triplets output from the input msg.
  //
  // The function should return the number of bits in the encoded 
  // message.
  int encode(const vector<unsigned char>& msg,
             int S, int T, int A,
             vector<Triplet>* encoded_msg);
             
  // A function to decode a message encoded using LZ77.
  void decode(const vector<Triplet>& encoded_msg,
              vector<unsigned char>* decoded_msg);
private:
  // Any data members here ...
  
};

#endif