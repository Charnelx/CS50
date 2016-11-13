#include <stdio.h>
#include <cs50.h>

// Lahn's algorythm for credit card number validation ("modulus 10")

int modulo(long long cc, bool odd);

int main(void)
{
    printf("Number: ");
    long long cc = get_long_long();

    int cc_sum = modulo(cc, false);
    int odd_sum = modulo(cc, true);
    int crc_result = (cc_sum + odd_sum) % 2;

    // if last digit is zero then card is valid, otherwise - call the police!
    if (crc_result != 0)
    {
        printf("INVALID\n");
    }
    else
    {
        long long result = cc;
 
        while(result >= 10)
        {
            result /= 10;
        }
        // VISA own's only first digit so need a separate check for other cards wich two digits to verify
        if (result != 4)
        {
            result = cc;
            
            while(result >= 100)
            {
                result /= 10;
            }
        }
        
        switch(result)
        {
            case 4:
                printf("VISA\n");
                break;
            case 34:
            case 37:
                printf("AMEX\n");
                break;
            case 51:
            case 52:
            case 53:
            case 54:
            case 55:
                printf("MASTERCARD\n");
                break;
            default:
                printf("INVALID\n");
                break;
        }
    }
    return 0;
}

/*
    Function takes credit card number as long long, odd flag as bool.
    
    If "odd" set to false function made computations on every second digit in card
    number from right-to-left; with "odd" set to true - computations made for every
    second digit starting from first with the same right-to-left order.
    
    Return: type int (crc code)
*/

int modulo(long long cc, bool odd)
{
    long long mod_cc = cc;
    int cc_sum = 0;
    
    if (odd == false)
    {
        int counter = 1;
        
        while(cc)
        {
            cc = cc % 10;
            
            // only every second digit from right to left pass
            if (counter == 2)
            {
                
                // if number consists of two digits - split them; else - multiply
                if ((cc * 2) >= 10)
                {
                    int mult = cc * 2;
                    int r_number = mult / 10;
                    int l_number = mult - (r_number * 10);
                    cc_sum += (r_number + l_number);
                }
                else
                {
                   cc_sum += cc * 2; 
                }
                counter = 0;
            }
            
            mod_cc /= 10;
            cc = mod_cc;
            counter++;
            
            if (mod_cc == 0)
            {
                break;
            }
        }
    }
    else    //odd=true
    {
        int counter = 0;
        
        while(cc)
        {
            cc = cc % 10;
            
            
            if (counter == 0)
            {
                cc_sum += cc;
                counter = 2;
            }
            
            mod_cc /= 10;
            cc = mod_cc;
            counter--;
            
            if (mod_cc == 0)
            {
                break;
            }
            
        }
    }
    
    return cc_sum;
}