#include<string>
#include<unordered_map>
#include<vector>
#include "AbstractCache.h"
using namespace std;

class FIFOCache : public AbstractCache {

	FIFOCache() {
	}
	void updateList(string url){
		return;
	}

	int pickIndexForNextDeletion(){
		return 0;
	}	
};
