#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int multiplyer = 12;
    
    printf("Your minuts in bath?: ");
    int mt = get_int();
    
    if (mt > 0)
    {
        printf("Bottles: %d\n", mt * multiplyer);
    }
    else
    {
        printf("Wrong input!");
    }
    
    return 0;
}