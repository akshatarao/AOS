#include <string>
#include <vector>
#include <unordered_map>
#define MAX_CACHE_SIZE 20
using namespace std;

class AbstractCache {

public:
	unsigned long cacheSize;
	void insertIntoCache(string url, string content);
	void deleteFromCache();
	string getFromCache(string url);
    virtual void updateList(string url) = 0;
	virtual int pickIndexForNextDeletion() = 0;
    unordered_map<string, string> map;
    vector<string> list;
	virtual ~AbstractCache() = 0;
};
