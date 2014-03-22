#include <string>
#include <unordered_map>
#include <vector>
#include "AbstractCache.h"

/**
 *@file RandomCache.h
 */

/**
 *@author Akshata Rao
 */

using namespace std;

/**
 *@brief RandomCache
 */

class RandomCache : public AbstractCache
{
 	public:
		~RandomCache();
		RandomCache();
		
		void updateList(string url);
		int pickIndexForNextDeletion();
		void insertIntoList(string url, string content);
};	
