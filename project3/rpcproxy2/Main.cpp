#include "LRUCache.h"
#include "FIFOCache.h"
#include "LMUCache.h"
#include "RandomCache.h"
#include <iostream>

using namespace std;

int trial(){
	
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


void testLMUInsertOneElement()
{
	LMUCache cache;
	
	string key = "one";	
	string value = "1234567";
	
	cache.insertIntoCache(key, value);
	
	cout << "\nTest 1";
	cout << "\nValue at Head: " << cache.list.at(0).url;
	cout << "\nValue at Key: " << cache.getFromCache(key) ;
	cout << "\nCache Size " <<cache.cacheSize;
	cout << "\n-------------------";
}

void testLMUInsertTwoElements()
{
	LMUCache cache;

	cout << "\nTest 2";

	string key1 = "one";
	string value1="1234";

	cache.insertIntoCache(key1, value1);
	cout << "\nValue at Head: " << cache.list.at(0).url;
	cout << "\nValue at Key: " << cache.getFromCache(key1) ;
	cout << "\nCache Size " <<cache.cacheSize;

	string key2 = "two";
	string value2="123";

	cache.insertIntoCache(key2, value2);
	
	cout << "\nValue at Head: " << cache.list.at(0).url;
	cout << "\nValue at Key: " << cache.getFromCache(key2) ;
	cout << "\nCache Size " <<cache.cacheSize;
	cout << "\n-------------------";

}

void testLMUInsertThreeElements()
{
	LMUCache cache;

	cout << "\nTest 2";

	string key1 = "one";
	string value1="123";

	cache.insertIntoCache(key1, value1);
	cout << "\nValue at Head: " << cache.list.at(0).url;
	cout << "\nValue at Key: " << cache.getFromCache(key1) ;
	cout << "\nCache Size " <<cache.cacheSize;

	string key2 = "two";
	string value2="12345";

	cache.insertIntoCache(key2, value2);
	
	cout << "\nValue at Head: " << cache.list.at(0).url;
	cout << "\nValue at Key: " << cache.getFromCache(key2) ;
	cout << "\nCache Size " <<cache.cacheSize;

	string key3 = "three";
	string value3="123456";

	cache.insertIntoCache(key3, value3);
	
	cout << "\nValue at Head: " << cache.list.at(0).url;
	cout << "\nValue at Key: " << cache.getFromCache(key3) ;
	cout << "\nCache Size " <<cache.cacheSize;



	cout << "\n-------------------";

}	
void testLMU()
{
	testLMUInsertOneElement();
	testLMUInsertTwoElements();
	testLMUInsertThreeElements();
}
int main()
{
	testLMU();
	return 0;
}
