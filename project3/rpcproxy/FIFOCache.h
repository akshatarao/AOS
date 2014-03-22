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
 *@brief FIFO Cache
 */
class FIFOCache : public AbstractCache 
{
  public:
	~FIFOCache();
	FIFOCache();
	void updateList(string url);
	int pickIndexForNextDeletion();
};
