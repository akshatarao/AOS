#include "LRUCache.h"

void LRUCache::updateList(string url){
	int pos = find(list.begin(), list.end(), url) - list.begin();
	list.erase(list.begin() + pos);
	list.push_back(url);
}

int LRUCache::pickIndexForNextDeletion(){
	return 0;
}

LRUCache::~LRUCache(){

}

LRUCache::LRUCache(){
	cacheSize = 0;
}	
