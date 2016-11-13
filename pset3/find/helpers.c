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
    if (value < 0)
    {
        return false;
    }
    for (int i = 0; i <= n; i++)
    {
        if (value == values[i])
        {
            return true;
        }
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    for (int i = 0; i < n; i++)
    {
        int min = values[i];
        int index = i;
        
        for (int j = i + 1; j < n; j++)
        {
            if (min > values[j])
            {
                min = values[j];
                index = j;
            }
        }
        
        values[index] = values[i];
        values[i] = min;
    }
    return;
}
