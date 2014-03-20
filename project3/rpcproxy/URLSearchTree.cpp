#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

struct URLNode
{
	struct URLNode *left;
	struct URLNode *right;

	const char* url;
	const char* content;
};
bool search(struct URLNode **root, const char* url, struct URLNode** parent, struct URLNode **x);

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

void inorder(struct URLNode *node)
{
	if(node != NULL)
	{
		inorder(node->left);
		printf(" %s",node->url);
		inorder(node->right);
	}
} 

const char* retrieveContent(struct URLNode *node, const char* url)
{
	struct URLNode *root;
	const char* content = NULL;

	root = node;

	while(root != NULL)
	{
		if(strcmp(root->url, url) == 0)
		{
			content = root->content;
			return content;
		}

		if(strcmp(root->url, url) > 0)
		{
			root = root->left;
		}
		else
		{
			root = root->right;
		}
	}

}

void deleteURLNode(struct URLNode **root, const char* url)
{
	bool found = false;

	struct URLNode *parent, *x, *successor;

	if(*root == NULL)
	{
		printf("\nTree is empty");
		return;
	}
	
	parent = x = NULL;

	found = search(root, url, &parent, &x);

	printf("\nFOund: %d", found);
		
	if(found == false)
	{
		printf("\nURL was not found in tree");
		return;
	}
	else
	{
		if(x->left != NULL && x->right != NULL)
		{
			printf("Both Children");
			parent = x;
			successor = x->right;
			
			while(successor->left != NULL)
			{
				parent = successor;
				successor = successor->left;
			}

			x->url = successor->url;
			x->content = successor->content;

		}

		if(x->left == NULL && x->right == NULL)
		{
			printf("No Children");
			if(parent->right == x)
				parent->right = NULL;
			else
				parent->left = NULL;
		
			return;	
		}

		if(x->left == NULL && x->right != NULL)
		{
			printf("\nRight Child");
			if(parent->left == x)
				parent->left = x->right;
			else
				parent->right = x->right;
			return;
		}

		if(x->left != NULL && x->right == NULL)
		{
			printf("\nLeft Child");
			if(parent->left == x)
				parent->left = x->left;
			else
				parent->right = x->left;
			return;
		}
		
	}	
		
}

bool search(struct URLNode **root, const char* url, struct URLNode** parent, struct URLNode **x)
{
	struct URLNode *current;
	bool found = false;

	current = *root;
	*parent = NULL;

	while(current != NULL)
	{
		if(strcmp(current->url, url) == 0)
		{
			found = true;
			*x  = current;
			return found;
		}

		*parent = current;
		
		if(strcmp(current->url, url) > 0)
		{
			current = current->left;
		}
		else
		{
			current = current->right;
		}
	}	
	
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
	const char* output = retrieveContent(node, trial);
	printf("\nOutput: %s ", output);

	const char* trial2 = "trial";
	deleteURLNode(&node, trial);

}


