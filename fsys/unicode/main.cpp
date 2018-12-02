#include <iostream>
#include <vector>
#include <cstdint>

#include <bitset>

using namespace std;

#define byte_4_mask 0b11111000	
#define byte_3_mask 0b11110000	
#define byte_2_mask 0b11100000	
#define byte_1_mask 0b10000000	

#define byte_4_pattern 0b11110000		//11110xxx
#define byte_3_pattern 0b11100000		//1110xxxx
#define byte_2_pattern 0b11000000		//110xxxxx
#define byte_1_pattern 0b00000000		//1xxxxxxx

#define N 100000

vector<uint8_t> to_utf8(vector<uint32_t> &);
vector<uint32_t> from_utf8(vector<uint8_t> &);

int number_of_bytes(uint8_t);
int need_bytes(uint32_t);


int main(int argc, char const *argv[])
{
	vector<uint32_t> test_vect_ucod_in;
	vector<uint32_t> test_vect_ucod_out;
	vector<uint8_t> test_vect_utf;

	int i, k, miss = 0;


	for(i = 0; i < N; i++)
	{
		while(0b111111111111111111111 < (k = rand()));
		test_vect_ucod_in.push_back(k);
	}

	test_vect_ucod_out = from_utf8(test_vect_utf = to_utf8(test_vect_ucod_in));

	for(i = 0; i < N; i++)
		if(test_vect_ucod_in[i] != test_vect_ucod_out[i])
			miss++;

	cout << miss << endl;

	return 0;
}

vector<uint8_t> to_utf8(vector<uint32_t> &unicode_vector)  //it works ok
{
	int size = unicode_vector.size();
	int i, n;

	uint32_t symbol;

	vector<uint8_t> utf_vector;

	for (i = 0; i < size; i++)
	{
		symbol = unicode_vector[i];

		n = need_bytes(symbol);

		if(1 == n)
			utf_vector.push_back(0b00000000 + (0b01111111 & symbol));		//00000000 00000000 00000000 1xxxxxxx  ||00..00 0xxxxxxx
		else
			if(2 == n)
			{	
				utf_vector.push_back(0b11000000 + ((0b00011111000000 & symbol) >> 6));
				utf_vector.push_back(0b10000000 +        (0b00111111 & symbol));		//00000000 00000000 110xxxxx 10xxxxxx  ||00..00xxx xxxxxxxx
			}
			else
				if(3 == n)
				{
					utf_vector.push_back(0b11100000 + ((0b00001111000000000000 & symbol) >> 12)); //00000000 1110xxxx 10xxxxxx 10xxxxxx || 00..00 xxxxxxxx xxxxxxxx
					utf_vector.push_back(0b10000000 +       ((0b00111111000000 & symbol) >> 6));
					utf_vector.push_back(0b10000000 +              (0b00111111 & symbol));
				}
				else
					if(4 == n)
					{
						utf_vector.push_back(0b11110000 + ((0b00000111000000000000000000 & symbol) >> 18));	//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx || 000xxxxx xxxxxxxx xxxxxxxx
						utf_vector.push_back(0b10000000 +       ((0b00111111000000000000 & symbol) >> 12));
						utf_vector.push_back(0b10000000 +             ((0b00111111000000 & symbol) >> 6));
						utf_vector.push_back(0b10000000 +                    (0b00111111 & symbol));
				}
				else
					{
						cout << "to utf error" << endl;
						exit(-1);//error
					}
	}

	return utf_vector;
}

vector<uint32_t> from_utf8(vector<uint8_t> &utf_vector)
{
	int size = utf_vector.size();
	int i, j, n;

	vector<uint32_t> unicode_vector;

	for(i = 0; i < size; i++)
	{
		n = number_of_bytes(utf_vector[i]);

		if(1 == n)
			unicode_vector.push_back((uint32_t)(0b01111111 & (utf_vector[i])));
		else
			if((2 == n) && ((i + 1) < size))
			{
				unicode_vector.push_back((uint32_t) 
					((0b00011111 & ((uint32_t)utf_vector[i])) << 6) +	//<< 6 bits
					 (0b00111111 & utf_vector[i + 1]));			
				i += 1;
			}
			else			
				if((3 == n) && ((i + 2) < size))
				{
					unicode_vector.push_back((uint32_t) 
						((0b00001111 & ((uint32_t)utf_vector[i])) << 12) +	//<< 12 bits
						((0b00111111 & ((uint32_t)utf_vector[i + 1])) << 6)  +		//<< 6 bits 
						 (0b00111111 & utf_vector[i + 2]));					
					i += 2;	
				}
				else
				if((4 == n) && ((i + 3) < size))
					{
						unicode_vector.push_back((uint32_t) 
							((0b00000111 & ((uint32_t)utf_vector[i])) << 18) +	//<< 18 bits
							((0b00111111 & ((uint32_t)utf_vector[i + 1])) << 12) +	//<< 12 bits 
							((0b00111111 & ((uint32_t)utf_vector[i + 2])) << 6)  +	//<< 6 bits 
							 (0b00111111 & utf_vector[i + 3]));					
						i += 3;
					}
					else
						{
							cout <<"from utf8 error" << endl; //error
							exit(-1);
						}
	}

	return unicode_vector;
}

int number_of_bytes(uint8_t byte)								//if it  is 1st byte func return number of bytes
{																//else ret 0
	if(byte_4_pattern == (byte & byte_4_mask))
		return 4;
	else
		if(byte_3_pattern == (byte & byte_3_mask))
			return 3;
		else
			if (byte_2_pattern == (byte & byte_2_mask))
				return 2;
			else
				if (byte_1_pattern == (byte & byte_1_mask))
					return 1;
				else
					return 0;			
}

int need_bytes(uint32_t symbol)
{
	if(0b01111111 >= symbol)				//1xxxxxxx
		return 1;
	else
		if(0b0000011111111111 >= symbol)	//110xxxxx 10xxxxxx
			return 2;
		else
			if(0b000000001111111111111111 >= symbol)	//1110xxxx 10xxxxxx 10xxxxxx
				return 3;
			else
				if(0b00000000000111111111111111111111 >= symbol)		//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
					return 4;
				else
					return 0;	//error
}