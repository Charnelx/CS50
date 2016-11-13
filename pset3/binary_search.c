#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void quickSortR(int* arr, int n);

int main(void)
{
    int arr[7] = {10, 9, 7, 6, 2, 3, 11};
    int n = sizeof(arr) / sizeof(int);
    
    quickSortR(arr, n);
    
    for (int i = 0; i < n; i++)
    {
        printf("%i ", arr[i]);
    }
}

void quickSortR(int arr[], int n)
{
    int mid = arr[n>>1];
    
    int i = 0, j = n - 1;
    
    do
    {
        while (arr[i] <= mid) i++;
        while (arr[j] > mid) j--;
        
        // printf("%i and %i n - %i\n", i, j, mid);
        
        if (arr[i] > arr[j])
        {
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
        }
    }
    while(i <= j);
    
    // if (n > i) quickSortR(arr+i, n-i);
    // if ( j > 0 ) quickSortR(arr, j);
}