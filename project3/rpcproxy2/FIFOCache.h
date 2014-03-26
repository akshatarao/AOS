#include <string>
#include <unordered_map>
#include <vector>
#include "AbstractCache.h"

/**
 *@file FIFOCache.h
 */

/**
 *@author Spoorthi Ravi
 */

using namespace std;

/**
 *@brief FIFOCache
 */
class FIFOCache : public AbstractCache 
{
  public:

	//!destructor
	~FIFOCache();

	//!constructor
	FIFOCache();
	
	//!Update the Webcontent Eviction Candidate Queue
	void updateList(string url);

	//!Select the next candidate to be evicted
	int pickIndexForNextDeletion();

	//!Insert the URL, Webcontent into the Cache
	void insertIntoList(string url, string content);
};
