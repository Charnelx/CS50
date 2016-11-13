#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Node structure declaration
typedef struct NODE
{
    int n;
    struct NODE *next;
} NODE;

// Root structure declaration; Used to manage nodes
typedef struct ROOT
{
    struct NODE *head;
    struct NODE *tail;
} ROOT;


// Functions declaration
bool findVal(int n, ROOT *root);
bool addValue(int n, ROOT *root);
bool delValue(int n, ROOT *root);
void printNodes(ROOT *root);

int main(void)
{

    // Linked list init
    struct ROOT root;
    root.head = NULL;
    root.tail = NULL;
    
    // local vars
    bool res;
    int n;
    
    // add first node to list
    addValue(1, &root);
    
    // this would be the second value to add
    n = 2;
    
    // print current list values
    printNodes(&root);
    
    // search for value in list
    res = findVal(n, &root);
    printf("Find result for %i: %s\n", n, res ? "true" : "false");
    
    // add second value
    addValue(n, &root);
    
    printNodes(&root);
    
    res = findVal(n, &root);
    printf("Find result for %i: %s\n", n, res ? "true" : "false");
    
    // add few more values
    addValue(3, &root);
    addValue(4, &root);
    addValue(5, &root);
    
    printNodes(&root);
    
    // delete value from list
    delValue(4, &root);
    
    printNodes(&root);
    
    n = 4;
    res = findVal(n, &root);
    printf("Find result for %i: %s\n", n, res ? "true" : "false");
    
    // try to delete nonexisting value
    res = delValue(6, &root);
    printf("Value [%i] was deleted: %s\n", 6, res ? "true" : "false");
}


bool findVal(int n, ROOT *root)
{
    // Small optimization.
    // In sorted list given n cannot be greater than tail node value
    if (n > root->tail->n || n < root->head->n)
    {
        return false;
    }
    
    NODE *pointer = root->head;
    
    while (pointer != NULL)
    {
        if (pointer->n == n)
        {
            return true;
        }
        pointer = pointer->next;
    }
    return false;
}

bool addValue(int n, ROOT *root)
{
    struct NODE *pointer = root->head;
    struct NODE *prev_pointer;
    
    // allocate memory for new node
    struct NODE* new_node = (struct NODE*)malloc(sizeof(struct NODE));

    // init new node
    new_node->n = n;
    new_node->next = NULL;
    
    // this is the first node in list
    // make root head and tail points to this single node
    if (pointer == NULL)
    {
        // new_node->next = pointer;
        root->head = new_node;
        root->tail = new_node;
    }
    // new node value is less than 'head' node value of the list
    // link list head to this node
    else if (n < pointer->n)
    {
        new_node->next = pointer;
        root->head = new_node; 
    }
    else
    {
      while(true)
        {
          // new node value is equel to value of current node
          // also current node is not in the end of list
          if (n == pointer->n && pointer != root->tail)
          {
              new_node->next = pointer->next;
              pointer->next = new_node;
              break;
          }
          // new node value is equel to value of current node
          // also current node is the last in list so we need to reatach tail
          // to new node
          else if (n == pointer->n && pointer == root->tail)
          {
              new_node->next = pointer->next;
              pointer->next = new_node;
              root->tail = new_node;
              break;
          }
          // new node is less than current one so it goes between previous and current node
          else if (n < pointer->n)
          {
              prev_pointer->next = new_node;
              new_node->next = pointer;
              break;
          }
          // new node value is greater than last node in list
          // we'll reatach tail to new node
          else if (n > pointer->n && pointer == root->tail)
          {
              pointer->next = new_node;
              root->tail = new_node;
              break;
          }
          // node pointed by current pointer is not a place for new node; Need to search further
          else
          {
              prev_pointer = pointer;
              pointer = pointer->next;
          }
        } 
    }
    return true;
}

bool delValue(int n, ROOT *root)
{   
    // Small optimization.
    // In sorted list given n cannot be greater than tail node value
    if (n > root->tail->n || n < root->head->n)
    {
        return false;
    }
    else
    {
        struct NODE *pointer = root->head;
        struct NODE *prev_pointer;
        
        while (pointer != NULL)
        {
            if (pointer->n == n)
            {
                if (pointer == root->tail)
                {
                    prev_pointer->next = NULL;
                    root->tail = prev_pointer;
                    return true;
                }
                else if (pointer == root->head)
                {
                    root->head = pointer->next;
                    return true;
                }
                else
                {
                    prev_pointer->next = pointer->next;
                    return true;
                }
            }
            
            prev_pointer = pointer;
            pointer = pointer->next;
        }
        return false;
    }
}

void printNodes(ROOT *root)
{
    NODE *ptr = root->head;
    
    printf("Linked list: [ ");
    while (ptr != NULL)
    {
        if (ptr != root->tail)
        {
            printf("%i, ", ptr->n);
        }
        else
        {
            printf("%i ", ptr->n);
        }
        
        ptr = ptr->next;
    }
    printf("]\n");
}





