#include <string>
#include "AbstractCache.h"

/**
 *@author Akshata Rao
 */

/**
 *@file LMUCache.h
 */

using namespace std;

/**
 *@brief LMUCache
 */
class LMUCache : public AbstractCache {
public:
	LMUCache();
	~LMUCache();
	void updateList(string url);
	int pickIndexForNextDeletion();
	void insertIntoList(string url, string content);
};
