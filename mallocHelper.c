#include <stdlib.h>
#include <stdio.h>
#include <string.h> //for memcpy

/**
 * This program is a series of helper functions
 * that allow for easier use of malloc.
 * Make sure to only use malloc2 and free2 if you want to utalize this program properly,
 * as the other functions will not properly function otherwise.
 *
 * freeall frees all malloced blocks by using the
 * linked list added to and removed by free2 and malloc2
 *
 * printblocks and printblock shows an xray into all or a specific alloced block
 */

typedef struct myblock{
	struct myblock *next;
	struct myblock *prev;
	int size;
	void *subblock;
}B;



//initializing the sentinel here to avoid needing any function calls
struct myblock sntinl = {NULL, NULL, 0, 0};
B *sentinel = &sntinl;
//the sentinel allows to access the start of the chain globally, and is not used as a block itself

int init(), grow();

void *malloc2(int size);

void free2(), freeall(), printpayload(void* subblock), printblock(void* subblock), printblocks();

B *getblock(void* subblock);

//called during malloc2, just makes code look cleaner
void addToAList(B* block){ //CHANGE NAME TO ADD TO LIST!!!
	//sentinel -> NULL ----> sentinel <-> block -> NULL
	if(sentinel->next == NULL){
		sentinel->next = block;
		block->prev = sentinel;
		block->next = NULL;
	}
	//sentinel <-> next <-> ... ----> sentinel <-> block <-> next <-> ...
	else{
		block->next = sentinel->next;
		sentinel->next->prev = block;

		sentinel->next = block;
		block->prev = sentinel;
	}
}

//called during free2, just makes code look cleaner
void removeFromList(void* subblock){
	B* block = getblock(subblock);
	if(block == NULL){ //Should never happen as long as only used internally
		fprintf(stderr, "error, tried calling printblock on something that is not currently allocated by malloc2\n");
		return;
	}
	//prev -> block -> NULL ----> prev -> NULL
	if(block->next == NULL)
		block->prev->next = NULL;
	//prev <-> block <-> next ----> prev <-> next
	else{
		block->prev->next = block->next;
		block->next->prev = block->prev;
	}
}

//the malloc wrapper function which allows for use of the other functions in this helper program
void *malloc2(int size){
	void * p = malloc(size);
	if(p == NULL){
		fprintf(stderr, "error, a call to malloc (the orginal malloc) failed\n");
		return NULL;
	}

	B* block = malloc(sizeof(struct myblock)); //do I have to declare as static????? or should I malloc it???
	if(block == NULL){
		fprintf(stderr, "error, a call to malloc (the orginal malloc) failed\n");
		return NULL;
	}
	block->size = size;
	block->subblock = p;
	addToAList(block);

	return p;
}


void *realloc2(void *prev_subblock, int size){
	void* next_subblock = malloc2(size);
	if(next_subblock == NULL)
		return NULL;
	memcpy(next_subblock, prev_subblock, size); //copying the payload from the previous to the next payload
	free2(prev_subblock);
	return next_subblock;
}

//properly frees any blocks made with malloc2
void free2(void *subblock){
	B* block = getblock(subblock);
	if(block == NULL){
		fprintf(stderr, "error, tried calling printblock on something that is not currently allocated by malloc2\n");
		return;
	}
	removeFromList(subblock);
	free(block);
	free(subblock);
}

//could be used at end of a program to easily free all recourses allocated with malloc2
void freeall(){
	B *current = sentinel; //set to sentinel/head of chain
	while(current != NULL){ //continue until end of chain
		if(current != sentinel)
			free2(current->subblock); //if only using at the end of a program, could just be free
		current = current->next;
	}
}

//prints the location, size, and payload of a block. Prints the payload byte by byte, in integer format
//NOTE:Printing in the proper format would require
//knowing the type of variable that the block stores, which would require an additional argument and a different
//print statment every time, and requiring to enter the type of variable per call would be very user unfriendly.
//I believe this is still very helpful in understanding dynamic memory allocation as this is how bytes are truly stored.
void printblock(void* subblock){
	B* block = getblock(subblock);
	if(block == NULL){
		fprintf(stderr, "error, tried calling printblock on something that is not currently allocated by malloc2\n");
		return;
	}


	//doing a commical amount of work to get the correct number of tilde's,
	//purely for aesthetics.
	int tildecounter = 45;
	int tester = block->size;

	while(tester > 0)
		tester /= 10;
		tildecounter++;

	char* ch = (char*)subblock;
	for(int i = 0; i < block->size; i++){
		char c = *ch++;
		if(c > 99)
			tildecounter += 5;
		else if(c > 9)
			tildecounter += 4;
		else if(c > -1)
			tildecounter += 3;
		else if (c > -10)
			tildecounter += 4;
		else if (c > -100)
			tildecounter += 5;
		else
			tildecounter += 6;
	}

	for(int i = 0; i < tildecounter; i++)
		printf("~");
	printf("\n");

	//printing some info about the block, then the payload
	printf("size: %d, location:%ld, payload: [", block->size, (long)block->subblock);

	//printing the payload itself now
	ch = subblock;
	for(int i = 0; i < block->size; i++){
		if(i + 1 == block->size)
			printf("%d]\n", *ch++);
		else
			printf("%d, ", *ch++);
	}

	for(int i = 0; i < tildecounter; i++)
		printf("~");
	printf("\n");


}

//calls printblock for every block there is on the chain
void printblocks(){
	printf("\n");
	B *current = sentinel; //set to sentinel/head of chain
	while(current != NULL){ //continue until end of chain
		if(current != sentinel){
			printblock(current->subblock);
			printf("\n");
		}
		current = current->next;
	}
}

//obtains a block from a given subblock by checking the chain
B *getblock(void* subblock){
	B *current = sentinel; //set to sentinel/head of chain
	while(current != NULL){ //continue until end of chain
		if(current->subblock == subblock && current != sentinel) //check if subblocks match
			return current; //if so, this is the corresponding block to the given subblock
		current = current->next; //check next block
	}
	return NULL; //failure, none of the subblocks of any in the chain matched
}
