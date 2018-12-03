#pragma once

#include "BSTMap.h"	//	inner hash table container

/*
	A BST-Array implementation of a hash map
*/
template <class K, class V>
class HashMap
{
private:
	const size_t arrSize;	//	size of hash table, initialized on construction
	size_t sz = 0;	//	number of elements in table

	BSTMap<K, V> *arr;	//	hash table

	
	size_t(*hash)(const K&, size_t);	//	a hashing function that takes a key and a size_t, and returns a hash value from 0 to that size_t-1

public:
	HashMap(size_t as, size_t(*h)(const K&, size_t));	//	constructor, takes hash table size and hashing function pointer
	~HashMap() { clear(); delete[] arr;	arr = nullptr;}	//	destructor, clears BSTs and frees hash table

	void insert(const K &k, const V &v) { arr[hash(k, arrSize)].insert(k, v); ++sz; }	//	insert value into hash table
	//void erase(const K &k);	//	
	const V &find(const K &k) const { return arr[hash(k, arrSize)].find(k); }	//	return value associated with key k

	bool empty() const { return sz == 0; }
	size_t size() const { return sz; }
	void clear();	//	clears BSTs, sets size to 0
};
/*
template<class K, class V>
inline size_t HashMap<K, V>::hash(const K & k) const
{
	return 0;
}
*/
template<class K, class V>
inline HashMap<K, V>::HashMap(size_t as, size_t(*h)(const K&, size_t)):
	arrSize(as), arr(new BSTMap<K,V>[as]), hash(h)
{
	
}

template<class K, class V>
inline void HashMap<K, V>::clear()
{
	for (size_t i = 0; i < arrSize; ++i)
	{
		arr[i].clear();
	}

	sz = 0;
}
