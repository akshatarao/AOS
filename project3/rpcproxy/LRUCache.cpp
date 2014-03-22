#include "LRUCache.h"

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
