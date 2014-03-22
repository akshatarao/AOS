#include <string>
#include <unordered_map>
#include <vector>
#include "AbstractCache.h"

using namespace std;

class FIFOCache : public AbstractCache {
public:
	~FIFOCache();
	FIFOCache();
	void updateList(string url);
	int pickIndexForNextDeletion();
};
