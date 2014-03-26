#include "AbstractCache.h"
#include <iostream>

/**
 *@author Spoorthi Ravi
 */

/**
 *@file AbstractCache.cpp
 */
 
/**
 *@brief Get Web Content from Cache
 *@param URL
 *@returns Web Content
 */
string AbstractCache::getFromCache(string url){

	string webcontent = map[url];

	//If the content exists in the cache	
	if(!webcontent.empty())
	{
		//Update location of the url in the queue based on replacement policy
		updateList(url);
	}
	return map[url];
}

/**
 *@brief Insert Into Cache
 *@param url - URL
 *@param Web Content 
 */
void AbstractCache::insertIntoCache(string url, string content){	

	//Check if Already Exists
	string webcontent = getFromCache(url);

	//If Content is Empty
	if(webcontent.empty())	
	{

		//If content fits into cache
		if(cacheSize + content.capacity() < maxCacheSize){
	
			//Insert the URL and Web Content into the Map
			map[url] = content;

			//Insert the URL into the Queue
			insertIntoList(url, content);


			//Update Cache Size
			cacheSize = cacheSize + content.capacity();
		}
		else{//Content Does not fit into cache

			//Delete from Cache
			deleteFromCache();

			//Insert URL and Web Content into Cache
			insertIntoCache(url,content);			
		}
	}

}

/**
 *@brief Delete Web Content From Cache
 *@returns none
 */
void AbstractCache::deleteFromCache(){

	//Obtain the next candidate for deletion
	int index = pickIndexForNextDeletion();

	//Get the URL and web content from the queue
	URLNode urlNode = list[index];
	string url = urlNode.url;
	
	//Update Cache Size
	cacheSize = cacheSize - map[url].capacity();

	//Remove entry from Queue and Hashmap
	map.erase(url);

	list.erase(list.begin() + index);

}

/**
 *@brief Set Maximum Cache Size
 *@param - maximumCacheSize - Maximum Cache Size
 *@returns none
 */
void AbstractCache::setMaxCacheSize(long maximumCacheSize)
{
	maxCacheSize = maximumCacheSize;
}

//Empty Destructor
AbstractCache::~AbstractCache(){

}
