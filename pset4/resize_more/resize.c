/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy infile outfile\n");
        return 1;
    }

    // remember filenames
    float scale = atof(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    int o_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    int old_biWidth = bi.biWidth;
    int old_biHeight = bi.biHeight;
    int mult, n;
    
    // setting initial variables
    if (scale < 1.0)
    {
        n = (int) roundf((bi.biWidth / scale) / 3);
        bi.biWidth = ((old_biWidth * abs(old_biHeight)) / (int) roundf(bi.biWidth / scale));
        bi.biHeight = ((old_biWidth * abs(old_biHeight)) / (int) roundf(abs(bi.biHeight) / scale));
        mult = (old_biWidth / bi.biWidth);
        n = 1;
    }
    else if (scale == 1.0)
    {
        n = 1;
        bi.biWidth *= n;
        bi.biHeight *= n;
        mult = 0;
    }
    else
    {
        n = (int) roundf(scale);
        bi.biWidth *= n;
        bi.biHeight *= n;
        mult = 0;
    }
    
    int n_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // int o_SizeImage = bi.biSizeImage;
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + n_padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    //write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // allocate memoryfor buffer depending on scale up or down
    RGBTRIPLE *buffer;
    if (mult > 0)
    {
        buffer = malloc((sizeof(RGBTRIPLE) * old_biWidth));
    }
    else
    {
        buffer = malloc((sizeof(RGBTRIPLE) * bi.biWidth));
    }
    
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(old_biHeight); i < biHeight-mult; i++)
    {
        int buff_idx = 0;
        
        // iterate over pixels in scanline
        for (int j = 0; j < old_biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
            for(int c = 0; c < n; c++)
            {
                // another way to iterate throught array elements
                *(buffer+(buff_idx)) = triple;
                buff_idx++;
            }
        }

        // skip over padding, if any
        fseek(inptr, o_padding, SEEK_CUR);
        
        for (int l = 0; l < n; l++)
        {
            // 
            fwrite((buffer), sizeof(RGBTRIPLE), bi.biWidth, outptr);
            
            // add recalculated padding
            for (int k = 0; k < n_padding; k++)
            {
                fputc(0x00, outptr);
            }
        }
    }
    
    //free memory after malloc
    free(buffer);
    
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // exit normally
    return 0;
}
