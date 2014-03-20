#ifndef URLSEARCHTREE_H_
#define URLSEARCHTREE_H_

bool search(struct URLNode **root, const char* url, struct URLNode** parent, struct URLNode **x);

void insert(struct URLNode **root, const char* url, const char* content);

void inorder(struct URLNode *node);

const char* retrieveContent(struct URLNode *node, const char* url);

void deleteURLNode(struct URLNode **root, const char* url);

bool search(struct URLNode **root, const char* url, struct URLNode** parent, struct URLNode **x);

#endif



