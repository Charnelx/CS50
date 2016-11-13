#include <stdio.h>
#include <cs50.h>

int ask(void);

int main(void)
{
    
    int a = ask();
    
    if (a == 0)
    {
        return 0;
    }

    
    int b = 1;
    
    do
    {
        for (int c = 0; c < a-b; c++)
        {
            printf(" ");
        }
        
        for (int i = 0; i < b; i++)
        {
            printf("#");
        }
        
        printf("  ");
        
        for (int i = 0; i < b; i++)
        {
            printf("#");
        }
        
        
        
        printf("\n");
        b = b + 1;
    }
    while (b <= a);
    
    return 0;
}

int ask(void)
{
    printf("Height?: ");
    int a = get_int();
    
    if (a >= 0 & a <= 23)
    {
        return a;
    }
    else
    {
        do
        {
            printf("Height?: ");
            a = get_int();
        }
        while(a < 0 || a > 23);
        
        return a;
    }
}