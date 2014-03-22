#ifndef ABSTRACT_H_
#define ABSTRACT_H_

#include <string>
#include <vector>
#include <unordered_map>

#define MAX_CACHE_SIZE 20

using namespace std;

/**
 *@author Spoorthi Ravi
 */

/**
 *@file AbstractCache.h
 */

class AbstractCache {

public:
	unsigned long cacheSize;
        unordered_map<string, string> map;
        vector<string> list;

	void insertIntoCache(string url, string content);
	void deleteFromCache();
	string getFromCache(string url);
        virtual void updateList(string url) = 0;
 	virtual int pickIndexForNextDeletion() = 0;

	virtual ~AbstractCache() = 0;
};

#endif
