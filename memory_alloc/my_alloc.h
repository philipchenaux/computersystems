#ifndef MY_ALLOC_H
#define MY_ALLOC_H
#include <stdio.h>
#include <stdlib.h>

/* init_alloc
 *
 * Initializes the data structures of the allocator.
 * This must be called prior to any my_malloc and my_free calls.
 */
void init_alloc();

/* my_malloc
 * 
 * function that finds a piece of available memory that is at least
 * num_bytes size. A pointer to the beginning of the usable piece of
 * that chunk is returned.
 */
void *my_malloc(int num_bytes);

/* print_memory()
 * prints out all memory in an inorder traversal
 */

void* test_my_malloc(int num_bytes);

void print_memory();

/* my_free
 * 
 * Function that returns the memory chunk whose usable piece starts
 * at that address back to the set of available memory so that it can
 * be reused in a subsequent free call
 */
void my_free(void *address);

#endif
