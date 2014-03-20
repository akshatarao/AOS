#include<stdio.h>
#include<string.h>
#include<stdlib.h>


struct URLNode
{
	struct URLNode *left;
	struct URLNode *right;

	const char* url;
	const char* content;
};

void insert(struct URLNode **root, const char* url, const char* content)
{
	if(*root == NULL)
	{
		printf("Im here");
		
		*root = (struct URLNode *)malloc(sizeof(struct URLNode));
		(*root)->left = NULL;
		(*root)->right = NULL;
		(*root)->url = url;
	 	(*root)->content = content;
	}
	else
	{
		const char* rootString = (*root)->url;

		if(strcmp(rootString,url) > 0)
		{
			insert(&((*root)->left), url, content);
		}
		else
		{
			insert(&((*root)->right), url, content);
		}
	}
}

void display(struct URLNode *node)
{
	if(node != NULL)
	{
		inorder(node->left);
		printf(" %s",node->url);
		inorder(node->right);
	}
} 

char* retrieveContent(struct URLNode *node, char* url)
{
	struct URLNode *root;

	


}
 
int main()
{
	struct URLNode *node;
	
	node = NULL;

	const char* trial = "whatsi";
	const char* hey = "ehy";
	const char* so = "so";
	
	insert(&node, trial, hey);
	insert(&node, so, hey);

	inorder(node);
}


