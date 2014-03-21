#include "AbstractCache.h"

char* AbstractCache::getFromCache(string url){
	updateList(url);
	return map[url];
}

void AbstractCache::insertIntoCache(string url, char* content){
	map[url] = content;
	list.push_back(url);
}

void AbstractCache::deleteFromCache(string url){
	int index = pickIndexForNextDeletion();
	map.erase(url);
	list.erase(list.begin() + index);
}
