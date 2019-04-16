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
	while(float(int(val))!=val)
	{
		if(val>=1)
		{
			val-=1;
			binary[i].flip();
		}
		i--;
		val=val*2;
	}
	binary[i].flip();
	return binary;
}