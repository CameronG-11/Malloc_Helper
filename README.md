# Malloc Helper
## Introduction:
A malloc wrapper class to better utilize and understand dynamic memory allocation in c. Feel free to use this program with any program you would like, but make sure to use only the included wrapper functions and none of the original functions or else the novel functions will not work at all. 

A trivial program included as the main.c file as an example.

## Functions:

It has the main functions malloc2, realloc2, and free2 which all function just like malloc, realloc, and free but with some background work that allows for the other functions to be used. 

Two novel functions are printblocks/printblock which display information about all the blocks or a given block aswell as its payload. (The payload is is displayed byte by byte, with the literal numerical values for each byte).
The other novel function is freeall(), which hnadily frees all blocks.

The wrapper program functions by keeping a linked list of a struct of blocks, with each block containing a pointer to its corresponding malloced 'subblock'. 
