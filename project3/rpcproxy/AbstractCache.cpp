#include "AbstractCache.h"
#include <iostream>

string AbstractCache::getFromCache(string url){
	updateList(url);
	return map[url];
}

void AbstractCache::insertIntoCache(string url, string content){	
	if(cacheSize + content.capacity() < MAX_CACHE_SIZE){
		map[url] = content;
		list.push_back(url);
		cacheSize = cacheSize + content.capacity();
	}
	else{
		deleteFromCache();
		insertIntoCache(url,content);			
	}

}

void AbstractCache::deleteFromCache(){
	int index = pickIndexForNextDeletion();
	string url = list[index];
	cacheSize = cacheSize - map[url].capacity();
	map.erase(url);	
	list.erase(list.begin() + index);
}

AbstractCache::~AbstractCache(){

}
