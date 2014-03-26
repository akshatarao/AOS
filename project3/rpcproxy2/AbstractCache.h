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
	
	//!URL
	string url;

	//!Web Content		
	string content;

	//!Content Size
	int contentSize;

	//!URLNode Constructor
	/*!
		Sets the URL and webcontent for each node	
	*/
	URLNode(string urlInput, string contentInput)
	{
		url  = urlInput;
		content = contentInput;
		contentSize = contentInput.capacity();
	}
};

/**
 *@brief Abstract Cache
 *
 */
class AbstractCache {

public:
	
	//!Max Cache Size
	unsigned long maxCacheSize;

	//!Cache Size
	unsigned long cacheSize;

	//!Cache HashMap of <URL,WebContent>
        unordered_map<string, string> map;

	//!URL Priority Queue
        vector<URLNode> list;

	//!Insert URL, Webcontent into cache
	void insertIntoCache(string url, string content);

	//!Delete the candidate URL from cache
	void deleteFromCache();

	//!Get URL From Cache
	string getFromCache(string url);
	
	//!Set Maximum Size for Cache
	void setMaxCacheSize(long size);

	//!Update the Queue for Web Content Eviction Candidates
        virtual void updateList(string url) = 0;

	//!Select the next candidate for eviction
 	virtual int pickIndexForNextDeletion() = 0;

	//!Insert Eviction Candidate into Queue
	virtual void insertIntoList(string url, string content) = 0;

	//!Destructor
	virtual ~AbstractCache() = 0;

	//!Constructor
	AbstractCache()
	{
		//Set a default max cache size
		maxCacheSize = MAX_CACHE_SIZE;
	}
};

#endif
