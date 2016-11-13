/*  Binnary tree realization.
    Support adding, deleting and searching for values.
    Consider that right elements have less values than left. My mistake ) */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Node structure declaration
typedef struct NODE
{
    int n;
    struct NODE *left;
    struct NODE *right;
} NODE;

// Root (tree) structure declaration; Used to manage nodes
typedef struct ROOT
{
    struct NODE *head;
} ROOT;


// Functions declaration
bool findValue(int n, ROOT *root);
bool addValue(int n, ROOT *root);
bool delValue(int n, ROOT *root);
struct NODE *minSearch(NODE *parent);

// void printNodes(ROOT *root);

int main(void)
{
    // Linked list init
    struct ROOT root;
    root.head = NULL;
    
    addValue(5, &root);
    addValue(3, &root);
    addValue(1, &root);
    addValue(2, &root);
    
    bool res = findValue(3, &root);
    printf("Find result for %i: %s\n", 3, res ? "true" : "false");
    
    res = findValue(2, &root);
    printf("Find result for %i: %s\n", 2, res ? "true" : "false");
    
    res = findValue(10, &root);
    printf("Find result for %i: %s\n", 10, res ? "true" : "false");
    
    delValue(3, &root);
    
    res = findValue(3, &root);
    printf("Find result for %i: %s\n", 3, res ? "true" : "false");
    
    delValue(1, &root);
    delValue(2, &root);
    
    res = findValue(1, &root);
    printf("Find result for %i: %s\n", 1, res ? "true" : "false");
    res = findValue(2, &root);
    printf("Find result for %i: %s\n", 2, res ? "true" : "false");
    
    
    addValue(3, &root);
    addValue(4, &root);
    addValue(2, &root);
    addValue(1, &root);
    
    res = findValue(1, &root);
    printf("Find result for %i: %s\n", 1, res ? "true" : "false");
    
    delValue(3, &root);
    
    res = findValue(3, &root);
    printf("Find result for %i: %s\n", 3, res ? "true" : "false");
    
    res = findValue(4, &root);
    printf("Find result for %i: %s\n", 4, res ? "true" : "false");
    
}

// add value to tree
bool addValue(int n, ROOT *root)
{
    struct NODE *pointer = root->head;
    
    // allocate memory for new node
    struct NODE* new_node = (struct NODE*)malloc(sizeof(struct NODE));

    // init new node
    new_node->n = n;
    new_node->left = NULL;
    new_node->right = NULL;
    
    // this is the first node in list
    // make root head and tail points to this single node
    if (pointer == NULL)
    {
        root->head = new_node;
    }
    // not a first node
    // keep looking for proper place for this node
    else
    {
        while(pointer != NULL)
        {
            // node value greater than parent node
            if (new_node->n > pointer->n)
            {
                if (pointer->left != NULL)
                {
                    pointer = pointer->left;
                }
                else
                {
                    // set node to parent left-side (node value > parent value)
                    pointer->left = new_node;
                    break;
                }
            }
            // node value less than parent node
            else if (new_node->n < pointer->n)
            {
                if (pointer->right != NULL)
                {
                    pointer = pointer->right;
                }
                else
                {
                    // set node to parent right-side (node value < parent value)
                    pointer->right = new_node;
                    break;
                }
            }
        }
    }
    return true;
}

// find value in tree
bool findValue(int n, ROOT *root)
{
    NODE *pointer = root->head;
    
    if (pointer == NULL)
    {
        return false;
    }
    
    while (pointer != NULL)
    {
        if (n == pointer->n)
        {
            return true;
        }
        else if (n > pointer->n)
        {
            pointer = pointer->left;
        }
        else if (n < pointer->n)
        {
            pointer = pointer->right;
        }
    }
    return false;
}

// recursive search for min node
struct NODE *minSearch(NODE *parent)
{
    if (parent->right == NULL && parent->left == NULL)
    {
        return parent;
    }
    else if (parent->left != NULL)
    {
            return minSearch(parent->left);
    }
    return parent;
}

// delete node from tree
bool delValue(int n, ROOT *root)
{
    NODE *pointer = root->head;
    NODE *prev_pointer;
    bool FLAG = false;
    
    if (pointer == NULL)
    {
        return false;
    }
    
    while (pointer != NULL)
    {
        if (n == pointer->n && pointer == root->head)
        {
            root->head = NULL;
            return true;
        }
        else if (n == pointer->n && pointer != root->head)
        {
            // no childrens
            if (pointer->left == NULL && pointer->right == NULL)
            {
                // left against parent node
                if (FLAG == false)
                {
                    prev_pointer->left = NULL;
                    return true;
                }
                // right against parent node
                else if (FLAG == true)
                {
                    prev_pointer->right = NULL;
                    return true; 
                }
            }
            // one left child
            else if (pointer->left != NULL && pointer->right == NULL)
            {
                // left against parent node
                if (FLAG == false)
                {
                    prev_pointer->left = pointer->left;
                    return true;
                } 
                // right against parent node
                else if (FLAG == true)
                {
                    prev_pointer->right = pointer->left;
                    return true; 
                }
            }
            // one right child
            else if (pointer->right != NULL && pointer->left == NULL)
            {
                // left against parent node
                if (FLAG == false)
                {
                    prev_pointer->left = pointer->right;
                    return true;
                } 
                // right against parent node
                else if (FLAG == true)
                {
                    prev_pointer->right = pointer->right;
                    return true; 
                }
            }
            // two childrens
            else
            {
                struct NODE *new_node = minSearch(pointer);
                new_node->right = pointer->right;
                new_node->left = NULL;
                
                // left against parent node
                if (FLAG == false)
                {
                    prev_pointer->left = new_node;
                    return true;
                } 
                // right against parent node
                else if (FLAG == true)
                {
                    prev_pointer->right = new_node;
                    return true; 
                }
            }
        }
        // move left or right (to greater valuer or smaller)
        else if (n > pointer->n)
        {
            prev_pointer = pointer;
            pointer = pointer->left;
            FLAG = false;
        }
        else if (n < pointer->n)
        {
            prev_pointer = pointer;
            pointer = pointer->right;
            FLAG = true;
        }
    }
    return false;
}
