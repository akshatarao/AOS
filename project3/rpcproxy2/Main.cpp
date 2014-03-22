#include "LRUCache.h"
#include "FIFOCache.h"
#include "LMUCache.h"
#include "RandomCache.h"
#include <iostream>

using namespace std;

int main(){
	
	LMUCache cache;
	string test = "key1";
	string testValue = "value1";
	string test2 = "key2";
	string testValue2 = "value2";
	string test3 = "key3";
	string testValue3 = "delete first1";
	cache.insertIntoCache(test, testValue);
	cout << "inserted first value" << "\n"; 
	cache.insertIntoCache(test2,testValue2);
	cout << "inserted second value" << "\n";
	cout << "value looked up = \t"; 
	cout<< cache.getFromCache(test) << "\n";
	cout << cache.cacheSize << "\n";
	cout << cache.list.at(0).url<< "H\n"; 
	cache.insertIntoCache(test3,testValue3);
	cout << cache.cacheSize << "\n";
	cout << cache.list.at(0).url <<"L\n"; 	
	cout << "poohoo\n";
	return 0;
}
