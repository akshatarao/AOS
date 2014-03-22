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
	~FIFOCache();
	FIFOCache();
	void updateList(string url);
	int pickIndexForNextDeletion();
	void insertIntoList(string url, string content);
};
