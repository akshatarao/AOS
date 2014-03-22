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

class URLNode
{
 public:
	string url;		
	string content;
	int contentSize;

	URLNode(string urlInput, string contentInput)
	{
		url  = urlInput;
		content = contentInput;
		contentSize = contentInput.capacity();
	}
};

class AbstractCache {

public:
	//members
	unsigned long cacheSize;
        unordered_map<string, string> map;
        vector<URLNode> list;

	//concrete implementations
	void insertIntoCache(string url, string content);
	void deleteFromCache();
	string getFromCache(string url);

	//abstract implementations
        virtual void updateList(string url) = 0;
 	virtual int pickIndexForNextDeletion() = 0;
	virtual void insertIntoList(string url, string content) = 0;

	virtual ~AbstractCache() = 0;
};

#endif
