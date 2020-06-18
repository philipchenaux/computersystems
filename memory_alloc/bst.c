#include<stdio.h>
#include<stdlib.h>
#include "bst.h"
#include "llist.h"
#include "memory.h"
#include "my_alloc.h"

//extern bst* avail_mem;

/* malloc a new node and assign the data
 * pointer to its data field
 */
node* node_new(void* data){
  node* new_node = (node*)malloc(sizeof(node));
  new_node->data = data;
  new_node->left = NULL;
  new_node->right = NULL;
  return new_node;
}

/* create a new bst, initialize its root to be NULL
 */
bst* bst_new(int (*cmp)(const void* x, const void* y)){
  bst* new_bst = (bst*)malloc(sizeof(bst));
  new_bst->root = NULL;
  new_bst->cmp = cmp;
  return new_bst;
}

/* Insert a node to to a subtree with a root node as parameter
 * Insertion is in sorted order. 
 * Return the new root of the modified subtree.  
 */
node* node_insert(node* root, void* data, 
    int (*cmp)(const void* x, const void* y)){
  if (root == NULL)
  {
	node *inserted_node = node_new(data);
	return inserted_node;
  } 
  else
  {
	if ((cmp(root->data,data))<0)
		root->right = node_insert(root->right,data,cmp);
	else
		root->left = node_insert(root->left,data,cmp);
  }
  return root;
}

/* Insert a new node to the bst
 */
void bst_insert(bst* b, void* data){
  b->root = node_insert(b->root,data,b->cmp);
}

node* successor(node* root)
{
	if (root == NULL)
		return NULL;
	node *tmp;
	for (tmp=root;tmp!=NULL&&tmp->left!=NULL;tmp=tmp->left)
		;
	return tmp;
}

/* delete a node from a subtree with a given root node
 * use the comparison function to search the node and delete 
 * it when a matching node is found. This function only
 * deletes the first occurrence of the node, i.e, if multiple 
 * nodes contain the data we are looking for, only the first node 
 * we found is deleted. 
 * Return the new root node after deletion.
 */
node* node_delete(node* root, void* data, 
    int (*cmp)(const void* x, const void* y)){
  if (root == NULL)
	return NULL;
  int comp = cmp(root->data,data);
  if (comp < 0)
	root->right = node_delete(root->right,data,cmp);
  else if (comp > 0)
	root->left = node_delete(root->left,data,cmp);
  else
  {
	if ((root->left == NULL)&&(root->right == NULL))
	{
		free(root);
		return NULL;
	}
	else if (root->left == NULL)
	{
		node *tmp = root;
		root = root->right;
		free(tmp);
	}
	else if (root->right == NULL)
	{
		node *tmp=root;
		root = root->left;
		free(tmp);
	}
	else
	{
		node* tmp = successor(root->right);
		root->data = tmp->data;
		root->right = node_delete(root->right,tmp->data,cmp);
	}
  }
  return root;
}

/* delete a node containing data from the bst
 */
void bst_delete(bst* b, void* data){
  b->root = node_delete(b->root,data,b->cmp);
}

/* Search for a node containing data in a subtree with
 * a given root node. Use recursion to search that node. 
 * Return the first occurrence of node. 
 */
void* node_search(node* root, void* data, 
    int (*cmp)(const void* x, const void* y)){
  if (root == NULL)
	return NULL;
  int comp = cmp(root->data,data);
  if (comp == 0)
	return root->data;
  else if (comp < 0)
	return node_search(root->right,data,cmp);
  else
	return node_search(root->left,data,cmp);
  return NULL;
}

/* Search a node with data in a bst. 
 */
void* bst_search(bst* b, void* data){
  void* res = node_search(b->root,data,b->cmp);
  if (res == NULL)
	printf("not found in tree\n");
  return res;
}

/* traverse a subtree with root in ascending order. 
 * Apply func to the data of each node. 
 */
void inorder_traversal(node* root, void(*func)(void* data)){
  if (root == NULL)
	return;
  else
  {
  	inorder_traversal(root->left,func);
  	func(root->data);
  	inorder_traversal(root->right,func);
  }
}

/* traverse a bst in ascending order. 
 * Apply func to the data of each node. 
 */
void bst_inorder_traversal(bst* b, void(*func)(void* data)){
  inorder_traversal(b->root,func);
}

/* free the node*/
void node_free(node* root){
  if (root == NULL)
	return;
  node_free(root->left);
  node_free(root->right);
  free(root->data);
  free(root);
}

/* free the bst with 
 */
void bst_free(bst* b){
  node_free(b->root);
  free(b);
}

/* inorder_traversal_inser_llist
 * purpose: inorder traversal of a bst while inserting each node
 	    into llist
 * inputs:
 *	node* root - pointer toroot of bst
 *	llist *list - llsit to insert into
 * outputs:
 *	outputs a llist fileld by bst
 */
void inorder_traversal_insert_llist(node* root, llist *list)
{
	if (root != NULL)
	{
		inorder_traversal_insert_llist(root->left,list);
		insert_tail(list,(root->data));
		inorder_traversal_insert_llist(root->right,list);
	}
	return;
}

/* an iterator to iterate through the bst in ascending order
 */
void* bst_iterate(bst* b){
	static llist *list = NULL;
	if (b != NULL)
	{
		list = create_llist();
		inorder_traversal_insert_llist(b->root,list);
	}
	if (list->head == NULL)
		return NULL;
	void *item = list->head->item;
	list->head = list->head->next;
	return item;
}


/* node_item_or_successor
 * purpose: given a size ofo a chunk of memory desired, it attempts to
 * find an available chunk of memory of the same size. If there is no
 * chunk of memory the same size, it finds the chunk of memory that is
 * closest to that but larger
 * inputs:
 *      node *n: a node pointer
 *      void *item: pointer to desired item
 *      int (*cmp): comparison function
 * output:
 *      a pointer to the memory struct
 */
void* node_item_or_successor(node *n, void *item, int (*cmp)(const void* x,
                             const void* y))
{       
        if (n == NULL)
        {       
                memory *new_mem = allocate_memory_page();
                n = node_new(new_mem);
        }
        int comp = cmp(item,n->data);
        if (comp == 0) 
		return n->data;
        else if (comp < 0)
        {       
                if (n->left == NULL)
			return n->data;
                else if ((cmp(n->left->data,item)) < 0)
			return n->data;
                else
			return node_item_or_successor(n->left,item,cmp);
        }
        else    
	{
                if (n->right == NULL)
			return NULL;
                else
			return node_item_or_successor(n->right,item,cmp);
        }
}

/* bst_item_or_successor
 * purpose: given a size ofo a chunk of memory desired, it attempts to
 * find an available chunk of memory of the same size. If there is no
 * chunk of memory the same size, it finds the chunk of memory that is
 * closest to that but larger
 * inputs:
 *      bst *b: a bst pointer
 *      void *item: pointer to desired item
 * outputs:
 *      a pointer to memory struct
 */
void* bst_item_or_successor(bst *b, void *item)
{
        return node_item_or_successor(b->root,item,b->cmp);
}

