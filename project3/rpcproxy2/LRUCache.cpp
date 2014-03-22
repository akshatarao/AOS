#include "LRUCache.h"
#include<iostream>
#include<cstring> 
/**
 *@file LRUCache.cpp
 */

/**
 *@author Spoorthi Ravi
 */

/**
 *@brief Update the List
 *@param url - URL
 *@returns none
 */
void LRUCache::updateList(string url){

	int pos = 0;
	URLNode urlNode("","");

       //Get the Position where the URL entry was initially stored
	
	for(vector<URLNode>::size_type i = 0; i != list.size(); i++)
	{
		urlNode = list[i];
	
	
		if(url.compare(urlNode.url) == 0)
		{
			pos = i;
			break;
		}
	}
			

	//Erase the entry from that position
	list.erase(list.begin() + pos);
	
	//Push the entry to the back of the queue as it's now most recently used
	list.push_back(urlNode);
}

/**
 *@brief Select Next Candidate for Eviction
 *@returns index of next candidate for eviction
 */
int LRUCache::pickIndexForNextDeletion(){

	//Pick the candidate in front of the list which's the least recently used
	return 0;
}

/**
 *@brief Insert URL to back of the Queue
 *@param url
 *@param content - Web Content
 *@returns none
 */
void LRUCache::insertIntoList(string url, string content)
{
	//As it is the most recently used, push to the back of the queue

	//Push url to the back of queue
	URLNode urlNode(url, content);
	urlNode.url = url;
	
	list.push_back(urlNode);

}
 
/**
 *@brief LRU Destructor
 */
LRUCache::~LRUCache(){

}

/**
 *@brief LRU Constructor
 */
LRUCache::LRUCache(){
	cacheSize = 0;
}	
