#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "AbstractCache.h"

using namespace std;

class LRUCache : public AbstractCache {
public:
	LRUCache();
	~LRUCache();
	void updateList(string url);
	int pickIndexForNextDeletion();
};
