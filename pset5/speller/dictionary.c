/**
 * Implements a dictionary's functionality.
 */

#include "dictionary.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>

// node structure for hashtable
typedef struct NODE
{
    char word[LENGTH + 1];
    struct NODE* next;
}
NODE;

// hash function declaration
unsigned long hasher(const char *str);

// word's dictionary size
#define h_size 143091

// hashtable length (dictionary length)
int dict_len = 0;

// hashtable with size of h_size
struct NODE *hashtable[h_size];

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // TODO
    
    struct NODE *temp = hashtable[hasher(word)];
    
    for (struct NODE *node = temp; node != NULL; node = node->next)
    {
        // compare lowercase words; return 0 if equel; -1/1 if less or greater
        if (strcasecmp(node->word, word) == 0)
        {
            return true;
        }
    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
 
// look at - http://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)HashTables.html?highlight=(CategoryAlgorithmNotes)
bool load(const char *dictionary)
{
    // TODO
    
    // open dictionary file
    FILE* file = fopen(dictionary, "r");
    if(file == NULL)
    {
        return false;
    }
    
    while(true)
    {
        /* create and allocate memory for node(word)
           malloc used because we need a node but it's empty at this time
           so we create pointer that address some memory space for further work with it */
        struct NODE *new_node = malloc(sizeof(NODE));
        
        // if node is null, unload malloc and close file to prevent memory leaks
        if (new_node == NULL)
        {
            unload();
            fclose(file);
            return false;
        }
        
        // read word from dictionary into node
        // if word length not in range of 0 - 45 chars, than ignore it
        if (fscanf(file, "%45s", new_node->word) != 1)
        {
            free(new_node);
            break;
        }
        
        // hash word using hash function
        int hash = hasher(new_node->word);
        
        /* make new node points to node appeared in 
           same hashtable index if exists, or to 
           hashtable index itself if empty (new node become related to this index in hashtable) */
        new_node->next =  hashtable[hash];
        
        // make hashtable with index [hash] points to new node
        hashtable[hash] = new_node;
        
        // increment size of hash table (word's count)
        dict_len++; 
    }
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    // just return hashtable (dict) current length; return zero if no dict loaded
    return dict_len;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // no word in hashtable; nothing to do here, go away immidiate
    if (dict_len == 0)
    {
        return false;
    }
    
    // loop through hashtable indexes
    for (int i = 0; i < h_size; i++)
    {
        // pointer to first node related to current index in hashtable
        struct NODE *node = hashtable[i];
        
        // work until last node in chain
        while (node != NULL)
        {
            // store pointer to next node in chain ( remember linked lists :3 )
            struct NODE *next = node->next;
            
            // free memory from current element
            free(node);
            
            // update current node to point to next node in chain
            node = next;
            
            // decrease word's counter
            dict_len--;
        }
    }
    return true;
}

/* djb2 hash function mod
   altered from here - http://www.cse.yorku.ca/~oz/hash.html
   also read this - http://stackoverflow.com/questions/14409466/simple-hash-functions
   and this - http://stackoverflow.com/a/3613423 */
unsigned long hasher(const char *str)
{
    // magic number ()
    unsigned long hash = 5381;
    int c;
    
    for(c = *str; *str != '\0'; str++)
    {   

        hash = (((hash << 3) + hash) + tolower(c))/5; /* hash * 33 + c */
        // uncomment for testing purpose
        // printf("Contribution of char [%c] = %lu\n", *str, hash);
    }

    return (hash % h_size);
}
