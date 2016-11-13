#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>


int main(void)
{
    string s = get_string();
    bool flag = true;
    
    for (int i = 0, l = strlen(s); i < l; i++)
    {
        if (isspace(s[i]))
        {
            flag = true;
        }
        else
        {
            if (flag == true)
            {
                printf("%c", toupper(s[i]));
                flag = false;
            }
        }
    }
    printf("\n");
    
    // return 0;
}