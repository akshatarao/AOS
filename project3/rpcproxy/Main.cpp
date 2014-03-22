#include "FIFOCache.h"
#include <iostream>

using namespace std;

int main(){
	
	FIFOCache cache;
	string test = "key1";
	string testValue = "value1";
	string test2 = "key2";
	string testValue2 = "value2";
	string test3 = "key3";
	string testValue3 = "delete first";
	cache.insertIntoCache(test, testValue);
	cout << "inserted first value" << "\n"; 
	cache.insertIntoCache(test2,testValue2);
	cout << "inserted second value" << "\n";
	cout << cache.cacheSize << "\n"; 
	cache.insertIntoCache(test3,testValue3);
	//cout << cache.cacheSize << "\n";
	cout << cache.list.at(0) << "\n";
	cout << cache.cacheSize << "\n";
	//cache.deleteFromCache();
	cout << cache.map[test3] << "\n";
	cout << cache.map.size() << "\n";
	cout << "poohoo\n";
	return 0;
}
