#include <stdlib.h>
#include <stdio.h>
#include "mallocHelper.c"

int main(int argc, char **argv){

	void * bb = malloc2(6);
	char* cc = bb;
	cc[0] = 1;
	cc[1] = 2;
	cc[2] = 3;
	bb = realloc2(bb, 4);

	printf("malloc2 result: %ld\n", (long)bb);
	printf("sentinel's next %ld == %ld getblock(bb)\n", (long)sentinel->next, (long)getblock(bb));


	void * bb2 = malloc2(sizeof(long)*10);
	long* ll = bb2;
	ll[0] = 1621626;
	ll[1] = 22222;
	ll[2] = 222222222;
	printf("malloc2 result: %ld\n", (long)bb2);
	printf("sentinel's next %ld == %ld getblock(bb2)\n", (long)sentinel->next, (long)getblock(bb2));



	void * bb3 = malloc2(50);
	char* ss = bb3;
	ss[0] = '1';
	ss[1] = 50;
	ss[2] = '3';
	printf("malloc2 result: %ld\n", (long)bb3);
	printf("sentinel's next %ld == %ld getblock(bb3)\n", (long)sentinel->next, (long)getblock(bb3));


	printf("calling printblocks: \n");
	printblocks();

	freeall();

	return EXIT_SUCCESS;
}