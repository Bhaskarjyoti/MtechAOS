/*
 * virus1.c
 * Simple Virus program/payload that prints a string on the screen
 * (c) BHASKARJYOTI DAS
 * 
 * 
 */


#include <stdio.h>

int i; int j;

int main(int argc, char **argv)


{
	for (i=0; i<24; i++)
	{
		for (j=0;j<4;j++)
		{
	printf(".....BHASKAR_VIRUS..");
		}
		printf("\n");
	}
	return 0;
}

