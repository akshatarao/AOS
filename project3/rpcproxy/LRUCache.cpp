#include "LRUCache.h"

LRUCache::LRUCache() {

}

void LRUCache::updateList(string url){
	int pos = find(list.begin(), list.end(), url) - list.begin();
	list.erase(list.begin() + pos);
	list.push_back(url);
}

int LRUCache::pickIndexForNextDeletion(){
	return 0;
}	
