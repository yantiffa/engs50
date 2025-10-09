/* 
 * test_hash.c --- 
 * 
 * Author: Tingwen Yan
 * Created: 10-07-2025
 * Version: 1.0
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "queue.h"
#include "hash.h"

void test_hopen_hclose() {
	printf("Testing hopen/hclose...\n");
	struct hashtable_t *hash = hopen(100);
	assert(hash != NULL);
	hclose(hash);
	printf("Passed basic hopen/hclose\n\n");
}


int main()
{
	test_hopen_hclose();
	return 0;
}


