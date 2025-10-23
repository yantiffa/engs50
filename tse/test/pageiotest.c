/* 
 * pageiotest.c --- 
 * 
 * Author: Tingwen Yan
 * Created: 10-21-2025
 * Version: 1.0
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pageio.h"
#include "webpage.h"

int main(){
	//normal case
	printf("testing for pageio.c\n");
	webpage_t *page = pageload(1, "../pagedir");

	if (page == NULL) {
		fprintf(stderr, "error!");
		return 1;
	}
	printf("sucessful to retrieve!\n");


	system("mkdir -p ../test/temp");
	pagesave(page, 1, "../test/temp");
	printf("success: saved retirved page!\n");

	webpage_t *copy = pageload(1, "../test/temp");
	if (copy == NULL) {
		fprintf(stderr, "error!");
		webpage_delete(page);
		return 1;
	}
	printf("success!");
	webpage_delete(page);
	webpage_delete(copy); 
	return 0;
	
}
	
