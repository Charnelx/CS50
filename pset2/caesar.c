#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <cs50.h>
#include <string.h>

int main(int argc, string argv[])
{
    int k;
    
    if (0 == argc > 1)
    {
        printf("Too much or less arguments!");
        return 1;
    }

    k = atoi(argv[1]);
    
    printf("plaintext:");
    string text = get_string();
    
    for (int i=0; i < strlen(text); i++)
    {
        if (isalpha(text[i]))
        {
            // for lowercase
            if ((int)text[i] >= 97)
            {
                text[i] = (char)((text[i] + k - 97) % 26) + 97;
            }
            // for uppercase
            else 
            {
                text[i] = (char)(((text[i] + k) - 65) % 26) + 65;
            }
        }
    }
    printf("ciphertext: %s\n", text);
    return 0;  
}