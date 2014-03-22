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
	LRUCache();
	~LRUCache();
	void updateList(string url);
	int pickIndexForNextDeletion();
	void insertIntoList(string url, string content);
};
