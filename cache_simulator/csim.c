#include "cachelab.h"
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[])
{
    int opt, s,E,b;
    char *filename;

    while(-1 != (opt = getopt(argc, argv, "s:E:b:t:"))){
        switch(opt) {
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                filename = optarg;
                break;
            default:
                fprintf(stderr, "incorrect arguments\n");
                break;
        }
    }

    /* malloc the nested array */
    int r, c, i, j;
    r = (int)pow(2,s);
    c = 1 + (3 * E);
    long unsigned int **arr = (long unsigned int**)malloc(r*sizeof(long unsigned int*));
    for (i=0;i<r;i++)
        arr[i] = (long unsigned int*)malloc(c*sizeof(long unsigned int));

    /* initialize the array */
    for (i=0; i < r; i++) {
        for (j=0; j < c; j++) {
            arr[i][j] = 0;
        }
    }

    /* Open the file */
    FILE *fp = fopen(filename,"r");
    if (fp == NULL)
    {
        printf("Could not open file %s for reading\n",filename);
		exit(2);
	}

    char identifier;
    long unsigned int address;
    int size, hitCount, missCount, evictionCount, alreadyhit;
    int min, min_loc, counter;
    hitCount = 0;
    missCount = 0;
    evictionCount=0;
    counter = 0;

    /* looping through each line of the file*/
    while(fscanf(fp," %c %lx,%d", &identifier, &address, &size)>0)
    {
        //long unsigned int offset = (address << (64 - b)) >> (64 - b);
        long unsigned int set = (address << (64 - (b+s))) >> (64 - s);
        long unsigned int tag = (address >> (b + s));
        int lru = arr[set][0];
        alreadyhit = 0;

        if (identifier != 'I'){
            counter++;

            /*looping for read or write */
            if (identifier != 'M') {
                for (i = 2;i < c; i += 3) {
                    if ((arr[set][i] == tag) && (arr[set][i-1] == 1)) {
                        arr[set][i+1] = counter;
                        hitCount ++;
                        alreadyhit=1;
                        break;
                    }
                }
                if (alreadyhit == 0) {
                    if (arr[set][(lru*3)+1] != 1) {
                        missCount ++;
                        arr[set][(lru*3)+3] = counter;
                        arr[set][(lru*3)+2] = tag;
                        arr[set][(lru*3)+1] = 1;
                    }
                    else {
                        missCount ++;
                        evictionCount ++;
                        arr[set][(lru*3)+3] = counter;
                        arr[set][(lru*3)+2] = tag;
                    }
                }
            }

            else if (identifier == 'M') {
                for (i = 0; i < 2; i++) {
                    for (j = 2; j < c; j += 3) {
                        if ((arr[set][j] == tag) && (arr[set][j-1] == 1)) {
                            arr[set][j+1] = counter;
                            hitCount ++;
                            alreadyhit=1;
                            break;
                        }
                    }
                    if (alreadyhit == 0) {
                        if (arr[set][(lru*3)+1] != 1) {
                            missCount ++;
                            arr[set][(lru*3)+3] = counter;
                            arr[set][(lru*3)+2] = tag;
                            arr[set][(lru*3)+1] = 1;
                        }
                        else {
                            missCount ++;
                            evictionCount ++;
                            arr[set][(lru*3)+3] = counter;
                            arr[set][(lru*3)+2] = tag;
                        }
                    }
                }
            }

            min = arr[set][(lru*3)+3];
            min_loc = lru;

            /* find the LRU after each line */
            for (i = 3; i < c; i += 3) {
                if (arr[set][i] < min) {
                    min = arr[set][i];
                    min_loc = (i/3) - 1;
                }
            }
            /* set the LRU counter */
            arr[set][0] = min_loc;
        }
    }

    /*freeing nested array */
    for (i=0;i<r;i++) {
        free(arr[i]);
    }
    free(arr);

    printSummary(hitCount, missCount, evictionCount);
    return 0;
}