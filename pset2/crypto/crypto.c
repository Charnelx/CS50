#define _XOPEN_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>

char *crypto(const char *key, const char *salt);

int main(int argc, string argv[])
{
    if (0 == argc > 1)
    {
        printf("Wrong number of arguments.\nEnter only hashed password!");
        return 1;
    }
    
    string h_password = argv[1];
    
    char salt[3];
    
    // getting salt from hashed password (first two chars)
    for (int i = 0; i < 2; i++)
    {
        salt[i] = h_password[i];
    }
    
    // open a file and read it line by line
    FILE* file = fopen("dictionary.txt", "r");
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        /* encrypt words from dictionary and compare
           it with given password hash */
        string h_word = crypto(strtok(line, "\n"), salt);
        
        if(strcmp(h_password, h_word) == 0)
        {
        	printf("Password found: %s (hash: %s)\n", line, h_word);
        	return 0;
        }
    }
    fclose(file);
}

// little encrypting function. Nothing to see here. Really.
char *crypto(const char *key, const char *salt)
{
    string h_word = crypt(key, salt);
    return h_word;
}

