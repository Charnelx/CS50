/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // leave place for blank cell
    int k = (d*d) - 1;
    
    // iterate over entire board
    for(int row = 0; row < d; row++)
    {
        for(int col = 0; col < d; col++)
        {
            // insert into board value of counter (being 'k'), then decrement counter
            board[row][col] = k--;
        }
    }
    // set blank cell in right down of the board
    board[d-1][d-1] = 95;
    
    // if even dimensions, create a solveable board by switching positions of a and 2
    if (d % 2 == 0)
    {
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for (int row = 0; row < d; row++)
    { 
        for (int col = 0; col < d; col++)
        {
            // if cell is blank - print out "_"
            if (board[row][col] == 95)
            {
                printf(" _ ");
            }
            // print cell value in two-digits format
            else
            {
                printf("%2d ", board[row][col]);
            }
        }
    // end row
    printf("\n");
    }
    
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{   
    // starts from upper row
    for(int row = 0; row < d; row++)
    {
        for(int column = 0; column < d; column++)
        {
            if (board[row][column] == tile)
            {   // top (upper) is blank
                if (board[row-1][column] == 95)
                {   
                    // swap blank and tile
                    board[row-1][column] = tile;
                    board[row][column] = 95;
                    return true;
                }
                // left is blank
                else if (board[row][column-1] == 95)
                {
                    board[row][column-1] = tile;
                    board[row][column] = 95;
                    return true;
                }
                // right is blank
                else if (board[row][column+1] == 95)
                {
                    board[row][column+1] = tile;
                    board[row][column] = 95;
                    return true;
                }
                else if (board[row+1][column] == 95)
                {
                    board[row+1][column] = tile;
                    board[row][column] = 95;
                    return true;
                }
                return false;
             }
          }
       }
       return 0;
 }

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
// TODO
    // top left of board is 1
    int count = 1;
    // iterate over board to make sure current tile is equal to current count value
    for(int row = 0; row < d; row++)
    {
        for(int column = 0; column < d; column++)
        {
            // last tile should be blank
            if (row == d-1 && column == d-1)
            {
                continue;
            }
            // all cell's should be sorted in straight order to win
            if (board[row][column] != count++) 
            {   
                return false;
            }
        }   
    }
    return true;
}
    

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}