#include "FIFOCache.h"

/**
 *@file FIFOCache.h
 */

/**
 *@author Spoorthi Ravi
 */

/**
 *@brief Update the Queue
 *@param URL
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
