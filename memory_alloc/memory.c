#include<stdio.h>
#include<stdlib.h>
#include "memory.h"
#include "bst.h"
#include "my_alloc.h"

//extern bst* avail_mem;

/* memory_new
 * create a new memory struct, initialze its address and size
 */
memory* memory_new(void* addr, unsigned int size){
  memory* m = (memory*)malloc(sizeof(memory));
  m->addr = addr;
  m->size = size;
  return m;
}

/* free the dynamically allocated memory struct
 */
void memory_free(void* p){
  memory* m = (memory*)p;
  free(m);
}

/* compare two memory variables x and y by address 
 * if x is less than y, return -1
 * if x is greater than y, return 1
 * if they are equal, return 0
 */
int memory_addr_cmp(const void* x, const void* y){
	memory *mx = (memory*)x;
	memory *my = (memory*)y;
	if (mx->addr < my->addr)
		return -1;
	else if (mx->addr > my->addr)
		return 1;
	else
		return 0;
}

/* compare two memory variables x and y by size 
 * if x is less than y, return -1
 * if x is greater than y, return 1
 * if they are equal, return 0
 */
int memory_size_cmp(const void* x, const void* y){
  memory *mx = (memory*)x;
  memory *my = (memory*)y;
  if (mx->size < my->size)
	return -1;
  else if (mx->size > my->size)
	return 1;
  else
	return 0;
}

/* print the memory address and size
 */
void memory_print(void* data){
  if (data == NULL) return;
  memory* m = (memory*)data;
  printf("address: %d, size: %u\n", (unsigned int)m->addr, m->size);
}

/* allocate_memory_page
 *
 * Call malloc to request a page of data - 4096 bytes. Create
 * a memory struct and initialize it to store the resulting
 * large chunk of data that was allocated. Return a pointer
 * to the memory struct.
 */
memory *allocate_memory_page(){
  memory* ret_val;
  ret_val = (memory*)malloc(sizeof(memory));
  ret_val->addr = (void*)malloc(4096);
  ret_val->size = 4088;
  *((int*)ret_val->addr) = ret_val->size;
  return ret_val;
}

/* split_memory
 *
 * Given a memory struct and a desired size of memory,
 * perform the operations necessary to remove the desired
 * size of memory from the end of the chunk and record
 * the new information for the smaller chunk. Return a 
 * pointer to the beginning of the chunk you are handing out.
 */
void *split_memory(memory* data, unsigned int size_desired){
  memory* mem = data;
  //create a pointer to advance
  char* tmp = (void*)mem->addr;
  //change the size vlaue of the memory
  data->size -= (size_desired + 8);
  //move the pointer to the 8 bits before the beginning of the memory
  tmp += (data->size + 8);
  //hide the size_desired in the first 8 bits
  *((int*)tmp) = size_desired;
  //increment to the beginning of the memory to return
  tmp += 8;
  //set ret_val to the casted tmp
  void* ret_val = tmp;
  //return pointer to beginning of splitted memory
  return ret_val;
}

/* merge_memory
 *
 * Given two memory structs, check to see if the two can be 
 * merged. They can be merged if the two are next to each other 
 * in memory with no break in between. If they can be merged,
 * return a memory struct pointer to a struct containing the information
 * for a single memory chunk containing the old two chunks.
 * If they cannot be merged (there is space between them), then
 * return NULL; 
 *
 * Make sure that you free any memory structs that you need to.
 */
memory *merge_memory(memory *first, memory *second)
{
	int mem1_size = first->size;
	int mem2_size = second->size;
	char* tmp_first = (void*)(first->addr);
	//save position of beginning of first
	void* first_size = (void*)tmp_first;
	//move to end of first memory and store position
	//this should be the same location as the beginning of second
	tmp_first += (mem1_size+8);
	void* mem1_end_addr = (void*)tmp_first;
	void* tmp_second = (second->addr);
	//check if addresses of both memories are the same
	if ((int)mem1_end_addr == (int)tmp_second)
	{
		first->size += (mem2_size + 8);
		*((int*)first_size) = first->size;
		return first;
	}
	else
		return NULL;
}
