#include<string>
#include<unordered_map>
#include<vector>
#include<AbstractCache.h>
using namespace std;

class LRUCache : public AbstractCache {

	LRUCache() {
	}

	void updateList(string url){
		int pos = find(list.begin(), list.end(), url) - list.begin();
		list.erase(list.begin() + pos);
		list.push_back(url);
	}

	int pickIndexForNextDeletion(){
		return 0;
	}	
};
