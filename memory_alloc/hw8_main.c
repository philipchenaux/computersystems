#include <stdio.h>
#include <stdlib.h>
#include "my_alloc.h"
#include "memory.h"
#include "bst.h"
#include "llist.h"

extern bst* avail_mem;
extern bst* avail_mem_addr;

int main(){


//***************** TESTING PART 1 ******************

	printf("\n****** TESTING: allocate_memory_page and"
		 " split_memory() *******\n");
	memory* mem1 = allocate_memory_page();
	void* split_res1 = split_memory(mem1, 500);
	char  *tmp1_mv = (void*)split_res1;
	tmp1_mv -=8;
	printf("split_res1 size [500] : %d\n",*((int*)tmp1_mv));
	memory* split_mem1 = memory_new(tmp1_mv,*((int*)tmp1_mv));

	void* split_res2 = split_memory(mem1, 20);
	char  *tmp2_mv = (void*)split_res2;
	tmp2_mv -=8;
	printf("split_res2 size [20] : %d\n",*((int*)tmp2_mv));
	memory* split_mem2 = memory_new(tmp2_mv,*((int*)tmp2_mv));

	void* split_res3 = split_memory(mem1, 0);
	char  *tmp3_mv = (void*)split_res3;
	tmp3_mv -=8;
	printf("split_res3 size [0] : %d\n",*((int*)tmp3_mv));
	memory* split_mem3 = memory_new(tmp3_mv,*((int*)tmp3_mv));
	
	void* split_res4 = split_memory(mem1, 1000);
	char  *tmp4_mv = (void*)split_res4;
	tmp4_mv -=8;
	printf("split_res4 size [1000] : %d\n",*((int*)tmp4_mv));
	memory* split_mem4 = memory_new(tmp4_mv,*((int*)tmp4_mv));

	void* split_res5 = split_memory(mem1, 520);
	char  *tmp5_mv = (void*)split_res5;
	tmp5_mv -=8;
	printf("split_res5 size [520] : %d\n",*((int*)tmp5_mv));
	memory* split_mem5 = memory_new(tmp5_mv,*((int*)tmp5_mv));

	printf("\n************ TESTING: merge_memory() ********\n");
	memory* merge_res1 = merge_memory(split_mem2,split_mem1);
	memory_print(merge_res1);

	memory* merge_res2 = merge_memory(split_mem5,split_mem3);
	memory_print(merge_res2);
	
	memory* merge_res3 = merge_memory(split_mem4, split_mem3);
	memory_print(merge_res3);

	printf("\n******** TESTING: my_malloc() **********\n");
	void* malloc1 = test_my_malloc(48);
	
	printf("\n");

	void* malloc2 = test_my_malloc(54);
	
	printf("\n");
	
	void* malloc3 = test_my_malloc(160);
	
	printf("\n");
	
	void* malloc4 = test_my_malloc(240);
	printf("\n");
		
	void* malloc5 = test_my_malloc(201);
	printf("\n");

	void* malloc6 = test_my_malloc(120);
	printf("\n");

	printf("*********** TESTING: my_free() and bst_iterate *********\n");
	my_free(malloc1);
	my_free(malloc2);
	my_free(malloc3);
	my_free(malloc6);
	
	void* item;
	printf("bst_iterate of avail_mem\n");
	for (item = bst_iterate(avail_mem); item != NULL;
		item = bst_iterate(NULL))
	{
		memory_print((memory*)item);
	}
	printf("\nbst_iterate of avail_mem_addr\n");
	for (item = bst_iterate(avail_mem_addr); item != NULL;
		item = bst_iterate(NULL))
	{
		memory_print((memory*)item);
	}

	printf("\n******** TESTING: compact_memory() in my_malloc() *****\n");
	void* malloc7 = test_my_malloc(3300);
	printf("\n");
	
	my_free(malloc4);
	
	print_memory();
	printf("\n");
	
	void* malloc8 = test_my_malloc(3000);

	my_free(malloc5);
	
	printf("\n*********** TESTING: print_memory() **********\n");
	my_free(malloc7);
	
	print_memory();
	printf("\n");

	my_free(malloc8);
}
