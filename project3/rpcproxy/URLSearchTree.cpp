#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<URLSearchTree.h>
/**
*@author Akshata Rao
*/

/**
 *@file centralized_mpi.c
 */

/**
*@brief Structure of URL Node
*/
struct URLNode
{
	struct URLNode *left; //left child
	struct URLNode *right;//right child

	const char* url; //URL
	const char* content; //WebContent
};

/**
 *@brief Insert URL Node
 *@param root - Root of the tree
 *@param url - Web URL
 *@param content - URL Content
 *@returns none
 */
void insert(struct URLNode **root, const char* url, const char* content)
{
	//If empty tree, create the tree
	if(*root == NULL)
	{
		
		*root = (struct URLNode *)malloc(sizeof(struct URLNode));
		(*root)->left = NULL;
		(*root)->right = NULL;
		(*root)->url = url;
	 	(*root)->content = content;
	}
	else
	{
		//Recursively find the parent of the node to be inserted
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

/**
 *@brief Inorder Display
 *@param node - Pointer to Root of the tree
 *@returns none
*/
void inorder(struct URLNode *node)
{
	printf("\n");
	if(node != NULL)
	{
		inorder(node->left);
		printf(" %s",node->url);
		inorder(node->right);
	}
} 

/**
 *@brief Retrieve the Content for the URL
 *@param node - Root of the BST
 *@param url - URL
 *@returns none
*/
const char* retrieveContent(struct URLNode *node, const char* url)
{
	struct URLNode *root;
	const char* content = NULL;

	root = node;

	while(root != NULL)
	{
		//If URL matches Return content
		if(strcmp(root->url, url) == 0)
		{
			content = root->content;
			return content;
		}

		//Root URL is greater than the searched URL
		if(strcmp(root->url, url) > 0)
		{
			root = root->left;
		}
		else//Root URL is lesser than the searched URL
		{
			root = root->right;
		}
	}

}

/**
 *@brief Delete a URL Node
 *@param root - Root of the BST
 *@param url - URL whose webcontent must be deleted
 *@returns none
**/
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

	if(found == false)
	{
		printf("\nURL was not found in tree");
		return;
	}
	else
	{
		//If the Root Node is the node to be deleted
		if(parent == NULL)
		{
			//Has 2 children, Adjust left children to the right subtree
			if(x->right != NULL && x->left != NULL)
			{
				struct URLNode* current = x->right;

				while(current->left != NULL)
				{
					current = current->left;
				}
				
				current->left = x->left;
				*root = x->right;
			}
			
			//Has Left Child, Reassign root to left child
			if(x->right == NULL && x->left != NULL)
			{
				*root = x->left;
			}

			//Has Right Child, reassign root to right child
			if(x->right != NULL && x->left == NULL)
			{
				*root = x->right;
			}

			//Has No Children, mark as NULL
			if(x->left == NULL && x->right == NULL)
			{
				*root = NULL;
			}
			
			//free(x);
			return;
		}
	
		if(x->left != NULL && x->right != NULL)
		{
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
			if(parent->right == x)
				parent->right = NULL;
			else
				parent->left = NULL;
		
			return;	
		}

		if(x->left == NULL && x->right != NULL)
		{
			if(parent->left == x)
				parent->left = x->right;
			else
				parent->right = x->right;
			return;
		}

		if(x->left != NULL && x->right == NULL)
		{
			if(parent->left == x)
				parent->left = x->left;
			else
				parent->right = x->left;
			return;
		}
		
	}	
		
}

/**
 *@brief - Search for the Parent node and the node to be deleted
 *@param - root - Root Node
 *@param - url - URL to be searched
 *@param - x - Pointer to the current node
 *@returns Found
*/
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

//Change to int main() if running solo
//int main()
void testCode()
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
	printf("\nRetrieved Content: %s ", output);

	const char* trial2 = "trial";
	deleteURLNode(&node, trial2);

	inorder(node);
}

//TODO: Add Comments
//TODO: Free Memory after deletion
