#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// create a type of unsigned integers of length of 8 bits (1 byte)
typedef uint8_t BYTE;

// define blocksize
#define BLOCKSIZE 512

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover <finame>\n");
        return 1;
    }   
    
    char *infile = argv[1];
    
    // open input file 
    FILE *file = fopen(infile, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    
    // open output file but do nothing yet
    FILE *outfile;
    outfile = NULL;
    
    // FLAG indicates startup of writing jpg's
    bool FLAG = false;
    int jpgnum = 0;
    
    while (1)
    {
        // buffer that will store every new block from dump file
        BYTE block[BLOCKSIZE] = {};
        
        // read from (file) part of (BLOCKSIZE) bytes to (block -> pointer to block memory location) for (1) time(s)
        size_t eof = fread(&block, sizeof(BLOCKSIZE), 1, file);
        
        // read blocks until EOF or error occure
        if (eof != 1)
        {
            break;
        }
        
        // check either given block contains jpg signature
        if ((block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff) && (block[3] == 0xe0 || block[3] == 0xe1))
        {
            if (outfile != NULL)
            {
                fclose(outfile);
            }
            
            
            FLAG = true;
            
            // array for filename
            char filename[8];
            
            // set filename
            sprintf(filename, "%03d.jpg", jpgnum);
            jpgnum++;
            
            if ((outfile = fopen(filename, "w")) == NULL)
            {
                printf("Could not write image.\n");
                return 1;
            }
        }   
        
        // write block to file
        if (FLAG == true)
        {
            fwrite(&block, sizeof(BLOCKSIZE), 1, outfile);
        }
    }
    
    if (outfile != NULL)
    {
        fclose(outfile);
    }
    
    fclose(file);
    return 0;
}