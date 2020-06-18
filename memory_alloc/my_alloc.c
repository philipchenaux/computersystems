#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include "memory.h"
#include "llist.h"

/* Global variables
 * By declaring these outside of a function:
 *   Scope: Every function inside of this file may use them. 
 *   		No functions in other files may use them.
 *   Lifetime: They exist for the entire program - they are created
 *		when the program begins and exist until the program
 *		ends.
 */   		

/* This is the tree that holds the available memory. */
bst *avail_mem = NULL;
bst *avail_mem_addr = NULL;

/* This includes all of the functions for the memory allocator.
 * The last two functions (my_malloc and my_free) are public 
 * functions with prototypes in the header function. The rest of the
 * functions are helper functions only called by my_malloc and 
 * my_free. You must implement these helper functions. If you wish,
 * you may add helper functions we did not define.
 */


	
/* my_free
 * 
 * Function that returns the memory chunk whose usable piece starts
 * at that address back to the set of available memory so that it can
 * be reused in a subsequent free call
 */
void my_free(void *address)
{
	char* tmp = address;
	tmp -= 8;
	int size = *(int*)tmp;
	memory* new_mem = memory_new((void*)tmp, size);
	bst_insert(avail_mem,(void*)new_mem);
	bst_insert(avail_mem_addr,(void*)new_mem);
}

/* print_memory 
 *
 * Print out all available memory in ascending order by size.
 */
void print_memory()
{
	printf("avail_mem inorder_traversal:\n");
	bst_inorder_traversal(avail_mem,memory_print);
	printf("avail_mem_addr inorder_traversal:\n");
	bst_inorder_traversal(avail_mem_addr,memory_print);
}

/* create_merge_llist()
 * purpose: helper function for compact_memory()
 * inputs:
 * 	none
 * outputs:
 *	linked list with avail_mem stored in each node in order of address
 */
llist *create_merge_llist()
{
	llist* merge_llist = create_llist();
	void* item;
	for(item = bst_iterate(avail_mem_addr); item != NULL;
		item = bst_iterate(NULL))
	{
		insert_tail(merge_llist,item);
	}
	return merge_llist;
}

/* compact_memory
 *
 * Search through all available memory and attempt to merge memory
 * that is stored next to each other.
 * This uses global variable avail_mem, so it does not need any
 * input parameters. Look at the bst functions and memory functions.
 */
void compact_memory()
{
	llist* merge_llist = create_merge_llist();
	llist_node* tmp = merge_llist->head;
	//loops while the next node is not equal to null
	while (tmp->next != NULL)
	{
		memory* res_mem = merge_memory(tmp->item,
				tmp->next->item);
		//checks if a successful merge occurs
		if (res_mem != NULL)
		{
			llist_node* tmp2 = tmp->next;
			bst_delete(avail_mem,tmp2->item);
			bst_delete(avail_mem_addr,tmp2->item);
			tmp->next = tmp->next->next;
			free(tmp2);
		}
		else
		{
			tmp = tmp->next;
		}
	}
}


/* init_alloc
 *
 * Initializes the data structures. This initializes avail_mem so 
 * that, instead of being a NULL pointer, it points to a valid bst
 * struct whose root pointer is NULL.
 */
void init_alloc()
{
	avail_mem = bst_new(memory_size_cmp);
	avail_mem_addr = bst_new(memory_addr_cmp);
}

/* my_malloc
 * 
 * function that finds a piece of available memory that is at least
 * num_bytes size. A pointer to the beginning of the usable piece of
 * that chunk is returned.
 */
void *my_malloc(int num_bytes)
{
	//checks size of num_bytes and adjusts accordingly
	if (num_bytes == 0)
	{
		fprintf(stderr,"ERROR: no memory is being requested\n");
	}
	if (num_bytes < 24)
		num_bytes = 24;
	if (num_bytes%8 != 0)
		num_bytes += (8-num_bytes%8);
	if (avail_mem == NULL)
	{
		init_alloc();
		memory *new_mem = allocate_memory_page();
		avail_mem->root = node_new(new_mem);
		avail_mem_addr->root = node_new(new_mem);
	}
	//dummy item struct
	memory* dummy = memory_new(NULL,num_bytes);
	void* data = bst_item_or_successor(avail_mem,dummy);
	//checks if bst_item_or_successor returns a result
	if (data == NULL)
	{
		compact_memory();
		data = bst_item_or_successor(avail_mem,dummy);
		//checks if bst_item_or_successor returns a result after
		//compact
		if (data == NULL)
		{
			memory* new_mem_after_null = allocate_memory_page();
			bst_insert(avail_mem, new_mem_after_null);
			bst_insert(avail_mem_addr, new_mem_after_null);
			data = bst_item_or_successor(avail_mem,dummy);
		}
	}
	//cast result to a memory struct
	memory* mem = (memory*)data;
	//size of the mem struct
	int mem_size = ((memory*)data)->size;
	//delete whatever found from the tree
	bst_delete(avail_mem,data);
	bst_delete(avail_mem_addr,data);
	//if the size of mem is smaller than 2 times the amount needed
	if (mem_size < (2 * num_bytes))
	{
		char* mem_tmp = mem->addr;
		mem_tmp += 8;
		return mem_tmp;
	}
	else
	{
		void* ret_addr = split_memory(mem,num_bytes);
		bst_insert(avail_mem,mem);
		bst_insert(avail_mem_addr,mem);
		return ret_addr;
	}
}

/* test_my_malloc
 * purpose: tests my_malloc()
 * inputs:
 *	int num_bytes - num of bytes wanted
 * outputs:
 *	void* - pointer to a chunk of usable memory
 */
void* test_my_malloc(int num_bytes)
{
	printf("requested for %d bytes of memory\n", num_bytes);
	void* malloc = my_malloc(num_bytes);
	char* tmp = malloc;
	tmp -= 8;
	unsigned int size = *((int*)tmp);
	memory* mem = memory_new(tmp,size);
	printf("memory returned: \n");
	memory_print(mem);
	return malloc;
}
