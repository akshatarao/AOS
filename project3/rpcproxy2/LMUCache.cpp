#include "LMUCache.h"
#include<iostream>
#include<cstring>

/**
 *@brief - This file contains the implementation for the Largest Size First algorithm (MAXS)
 */

/**
 *@file LMUCache.cpp
 */

/**
 *@author Akshata Rao
 */

/**
 *@brief Update the List
 *@param url - URL
 *@returns none
 */
void LMUCache::updateList(string url){
	return;
}

/**
 *@brief Select Next Candidate for Eviction
 *@returns index of next candidate for eviction
 */
int LMUCache::pickIndexForNextDeletion(){

	//Pick the candidate in front of the list which's the least recently used
	return 0;
}

/**
 *@brief Insert URL to back of the Queue
 *@param url
 *@param content - Web Content
 *@returns none
 */
void LMUCache::insertIntoList(string url, string content)
{
	int contentSize = content.capacity();

	int pos = list.size();
	URLNode urlNode("","");	
	for(vector<URLNode>::size_type i = 0; i != list.size(); i++)
	{
		urlNode = list[i];
	

		if(urlNode.contentSize < contentSize)
		{
			pos = i;
			break;		
		}
	}

	URLNode newNode(url,content);

	if(pos == list.size())
	{
		list.push_back(newNode);
	}
	else
	{
		vector<URLNode>::iterator it = list.begin();
		list.insert(it + pos, newNode);
		
	}
}

/**
 *@brief LMU Destructor
 */
LMUCache::~LMUCache(){

}

/**
 *@brief LMU Constructor
 */
LMUCache::LMUCache(){
	cacheSize = 0;
}	
