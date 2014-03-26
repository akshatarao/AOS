#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "AbstractCache.h"

/**
 *@author Spoorthi Ravi
 */

/**
 *@file LRUCache.h
 */
using namespace std;

/**
 *@brief LRUCache
 */
class LRUCache : public AbstractCache {
public:

	//!constructor
	LRUCache();

	//!destructor
	~LRUCache();

	//!Update the Candidate Queue with URL
	void updateList(string url);

	//!Select the next candidate for eviction
	int pickIndexForNextDeletion();

	//!Insert the web content eviction candidate into queue
	void insertIntoList(string url, string content);
};
