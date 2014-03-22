#include <string>
#include <vector>
#include <unordered_map>
using namespace std;


class AbstractCache {

public:
	void insertIntoCache(string url, char* content);
	void deleteFromCache(string url);
	char* getFromCache(string url);
    virtual void updateList(string url) = 0;
	virtual int pickIndexForNextDeletion() = 0;
    unordered_map<string, char*> map;
    vector<string> list;
};
