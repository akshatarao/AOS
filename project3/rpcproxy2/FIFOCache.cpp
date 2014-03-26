#include "FIFOCache.h"

/**
 *@brief - This file contains the implementation for the FIFO Cache Replacement Policy
 */

/**
 *@file FIFOCache.h
 */

/**
 *@author Spoorthi Ravi
 */

/**
 *@brief Update the Queue
 *@param url - URL
 *@returns none
 */
void FIFOCache::updateList(string url){
	//Dont do anything as the List already works as FIFO
	return;
}

/**
 *@brief Select next candidate entry for eviction
 *@returns Index of candidate for eviction
 */
int FIFOCache::pickIndexForNextDeletion(){
	//Pick the candidate in front of the queue
	return 0;
}

/**
 *@brief Insert URL to Back of FIFO Queue
 *@param url - URL
 *@param content - Web Content	
 *@returns none
 */
void FIFOCache::insertIntoList(string url, string content){

	//Push url to the back of queue
	URLNode urlNode(url,content);
	
	urlNode.url = url;
	
	list.push_back(urlNode);
}

/**
 *@brief Destructor
 */
FIFOCache::~FIFOCache(){

}	

/**
 *@brief Constructor
 */
FIFOCache::FIFOCache(){
	//Initialize current cache fill to 0
	cacheSize = 0;
}
