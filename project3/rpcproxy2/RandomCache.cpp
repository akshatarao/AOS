#include "RandomCache.h"
#include <cstdlib>


/**
 *@brief - This file contains the implementation for the Random Cache replacement policy
 */

/**
 *@file RandomCache.cpp
 */

/**
 *@author Akshata Rao
 */


/**
 *@brief Update the Queue
 *@param URL
 *@returns none
 */
void RandomCache::updateList(string url)
{
   //Dont do anything
   return;
}

/**
 *@brief Select next candidate for eviction
 *@returns Index for candidate for eviction
 */
int RandomCache::pickIndexForNextDeletion(){

	int nextIndex = 0;

	if(list.size() != 0)
	{
		nextIndex = rand() % list.size();
	}

	return nextIndex;
}

/**
 *@brief Insert URL to back of the Queue
 *@param url
 *@param content - Web Content
 *@returns none
 */
void RandomCache::insertIntoList(string url, string content)
{
	//Push url to the back of queue
	URLNode urlNode(url,content);
	
	urlNode.url = url;
	
	list.push_back(urlNode);
}

/**
 *@brief Random Cache Destructor
 */
RandomCache::~RandomCache(){
}

/**
 *@brief Random Cache Constructor
 */
RandomCache::RandomCache(){
	cacheSize = 0;
}
