/*
Name		: MERLIN MARIYAM REJI
Date		: 02/04/2024
Description	: Program to print a pattern of entered height
Sample input	:
Sample output	:
 */

#include <stdio.h>

int main()
{
    int row, column, n;
    printf("Enter the height of the pattern: ");
    scanf("%d", &n);

    for (row=n; row>=1; row--)
    {
	for (column=1; column<=n; column++)
	{
	    if (row + column <= n+1)
	    {
		printf("%d ",(row+column)-1);
	    }
	}
	printf("\n");
    }
}
