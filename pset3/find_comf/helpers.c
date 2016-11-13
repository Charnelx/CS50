/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    int min = 0;
	int max = n;

	while(max >= min)
	{
		int mid = (max+min)/2;

		if(values[mid] < value)
		{
			min = mid + 1;
		}
		else if(values[mid] > value)
		{
			max = mid - 1;
		}
		else
		{
			return true;
		}
	}
	return false;
}

/**
 * Sorts array of n values.
 */
 
/*
    Quick sort algorythm
    complexity:
        worst - O(n^2)
        best/middle - O(n log n)
*/
void sort(int values[], int n)
{
    // bitwise shift to right to get middle element of array
    int mid = values[n>>1];
    
    int i = 0, j = n - 1;
    
    do 
    {
        while ( values[i] < mid ) i++;
        while ( values[j] > mid ) j--;
        
        if (i <= j) 
        {
          int temp = values[i]; values[i] = values[j]; values[j] = temp;
          i++; j--;
        }
    } while ( i<=j );
        
        if ( j > 0 ) sort(values, j);
        if ( n > i ) sort(values+i, n-i);
        
    return;
}
