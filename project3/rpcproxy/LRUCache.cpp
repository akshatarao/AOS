#include "LRUCache.h"
#include<iostream> 
/**
 *@file LRUCache.h
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

		
	//Get the Position where the URL entry was initially stored
	int pos = find(list.begin(), list.end(), url) - list.begin();

	//Erase the entry from that position
	list.erase(list.begin() + pos);
	
	//Push the entry to the back of the queue as it's now most recently used
	list.push_back(url);
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
	list.push_back(url);

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
