#include <time.h>	//	for timing
#include <fstream>	//	to read corpus, and write results
#include <string>	//	key and value type
#include <vector>	//	key container

#include "BarcodeScanner.h"

#include "ArrayMap.h"	//map implementations
#include "BSTMap.h"	
#include "HashMap.h"

//	digit string hasher that returns a hash value from 0 to m-1, works best when m is a prime
size_t numberStringHash(const std::string &s, const size_t m)
{
	size_t hv = 0;	//	hash value
	const size_t p = 11;	//	a prime near the number of character possibilities (10)
	size_t pp = 1;	//	initialize pp to p^0 (1)

	for (auto c : s)
	{
		hv += (c - '0' + 1)*pp;	//	add char value (- minimum value +1) multiplied by power of p to hv
		hv %= m;	//	modulo hv
		pp *= p;	//	multiply pp by p
		pp %= m;	//	modulo p
	}

	return hv;
}

int main()
{
	//array of sizes
	size_t sizes[] = { 1, 11, 101, 1001, 10001 };//, 100001, 1000001 };

	clock_t t;

	//	for each size, create and fill array, bst, hash table implementations with that many values
	//	also fill a vector with ~100 evenly distributed keys
	//	then search for each value associated with a stored key, logging times in a .txt
	for (size_t s : sizes)
	{
		size_t dist = s / 100;	//	index distribution
		if (dist == 0) { dist = 1; }
		//initialize filestreams
		std::ifstream ifs("upc_corpus.txt");
		std::string ofn("test");
		ofn += std::to_string(s);
		ofn += ".txt";
		std::ofstream ofs(ofn);
		
		//initialize scanners
		BarcodeScanner<BSTMap<std::string, std::string>> BSTScanner(new BSTMap<std::string, std::string>);
		BarcodeScanner<ArrayMap<std::string, std::string>> ArrayScanner(new ArrayMap<std::string, std::string>(s));
		BarcodeScanner<HashMap<std::string, std::string>> HashScanner(new HashMap<std::string, std::string>(1009, numberStringHash));
		
		//BSTMap<std::string, std::string> bbs;
		//ArrayMap<std::string, std::string> bas(s);
		//HashMap<std::string, std::string> bhs(1009, numberStringHash);	//	initialize array size to 1009, a prime, instead of 1000, 997 is another good alternative

		std::vector<std::pair<size_t,std::string>> keys;
		keys.reserve(s);
		
		//eat dummy line at top of corpus
		std::string firstline;
		std::getline(ifs, firstline);
		
		//fill containers, key vector
		for (size_t i = 0; i < s; ++i)
		{
			std::string k;
			std::string v;
			std::getline(ifs, k, ',');
			
			std::getline(ifs, v);
			
			try
			{
				//bbs.insert(k, v);
				BSTScanner.scan(k, v);
				
			}
			catch (const int &e)
			{
				if (e == 1)	//	duplicate key
				{
					--i;	//	decrement count since nothing added, and %dist values could be missed otherwise
					continue;	//	skip below inserts
				}
				else
				{
					throw e;
				}
			}
			if (i % dist == 0)
			{
				keys.push_back(std::make_pair(i,k));
			}
			//bhs.insert(k, v);
			//bas.insert(k, v);
			HashScanner.scan(k, v);
			ArrayScanner.scan(k, v);
		}
		//	data header
		ofs << "index\thash\tbst\tarr\tsec/" << CLOCKS_PER_SEC << std::endl;
		//search for keys, log times
		for (auto k: keys)
		{
			ofs << k.first << "\t";	//	print index
			t = clock();	//	start hash find clock
			//auto v1 = bhs.find(k.second);	//	find value associated with key k
			auto v1 = HashScanner.scan(k.second);
			t = clock() - t;	//	end hash find clock
			ofs << t << "\t";	//	print hash find clock to file
			t = clock();	//	start bst find clock
			//auto v2 = bbs.find(k.second);	//	find value associated with key k
			auto v2 = BSTScanner.scan(k.second);
			t = clock() - t;	//	end bst find clock
			ofs << t << "\t";	//	print bst find clock to file
			t = clock();	//	start array find clock
			//auto v3 = bas.find(k.second);	//	find value associated with key k
			auto v3 = ArrayScanner.scan(k.second);
			t = clock() - t;	//	end array find clock
			ofs << t << std::endl;	//	print array find clock to file
		}
		
		ifs.close();
		ofs.close();
	}
	
	return 0;
}