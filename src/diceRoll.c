#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// Random 1-6 diceRoll generator with C code.

int main() 
{

	int i;
	int diceRoll;

	for(i=0; i<20; i++) 
	{
	   diceRoll = ( rand()%6 ) + 1;
	   printf("%d \n",  diceRoll);
	}
	return 0;
}
