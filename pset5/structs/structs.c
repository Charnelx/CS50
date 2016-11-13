#include <stdio.h>


// typedef - creates alias with name 'node' for unnamed data-type
typedef struct node
{
    int n;
    struct node *next;
}
node;

void getStruct(node *s, char text[]);

int main(void)
{
    node nd;
    nd.n = 10;
    
    // Realy need to define node as struct?
    struct node *md = &nd;
    
    printf("Accessing struct member with 'member access operator': %i\n", nd.n);
    
    char text1[] = "Sending pointer variable to function";
    getStruct(md, text1);
    
    char text2[] = "Sending pointer to function";
    getStruct(&nd, text2);
}

void getStruct(node *s, char text[])
{
    printf("%s: %i\n", text, s->n);
}