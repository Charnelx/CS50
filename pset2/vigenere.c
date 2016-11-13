#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>

char caesar(char chr, int k);

int main(int argc, string argv[])
{
    string key;
    
    // check input arguments
    if (0 == argc > 1)
    {
        printf("Too much or less arguments!");
        return 1;
    }
    else
    {
        for(int i = 0; i < strlen(argv[1]); i++)
        {
            if(!isalpha(argv[1][i]))
            {
                printf("Incorrect input. We accept only alphabetical characters.\n");
                return 1;
            }
        }
        key = argv[1];
    }
    
    printf("plaintext: ");
    
    // define main variables
    string text = get_string();
    int key_length = strlen(key);
    int key_counter = 0;
    int text_length = strlen(text);
    char c;
    
    for (int i = 0; i < text_length; i++)
    {
        // if char is correct - move forward, else - just leave char in text as is
        if(isalpha(text[i]))
        {
            c = caesar(text[i], toupper(key[key_counter])-65);
            text[i] = c;
            
            /* if text index is greater than key index than
            reverse key indexing to beginning 
            (plain_text_length > key_length) */
            
            if(key_counter < key_length - 1)
            {
                key_counter++;
            }
            else
            {
                key_counter = 0;
            }
        }
    }

    printf("ciphertext: %s\n", text);
    return 0;
}

char caesar(char chr, int k)
{
    // for lowercase
    if ((int)chr >= 97)
    {
        return ((chr + k - 97) % 26) + 97;
    }
    // for uppercase
    else 
    {
        return (((chr + k) - 65) % 26) + 65;
    }
}