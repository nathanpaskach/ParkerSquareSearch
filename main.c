#include <stdio.h>
#include "bigd.h"

/*
 *  This magic square of squares search program attempts to use
 *  the following method of generating magic squares to find
 *  a magic square of squares.
 *
 *  0 < a < b < c-a
 *  b =/= 2a
 *
 *     c-b   | c+(a+b) |   c-a   
 *  ---------+---------+---------
 *   c-(a-b) |    c    |  c+(a-b)  
 *  ---------+---------+---------
 *     c+a   | c-(a+b) |  c+b 
*/

int checkSquare(BIGD *seeds, BIGD *nums);

int main(int argc, char *argv[])
{
    // Initialize BIGD variables
    BIGD seedLevel = bdNew();
    bdSetShort(seedLevel, 3);
    BIGD abLimit = bdNew();
    bdSetEqual(abLimit, seedLevel);
    BIGD seeds[3];
    for(int i = 0; i < 3; i++)
    {
        seeds[i] = bdNew();
        bdSetZero(seeds[i]);
    }
    BIGD nums[9];
    for(int i = 0; i < 9; i++)
    {
        nums[i] = bdNew();
    }
    int result;
    unsigned long long try = 0;
    do
    {
        try++;
        result = checkSquare(seeds, nums);
        if(result == -1)
        {
            // Only count valid magic squares
            try--;
        }
        if(result == 1)
        {
            // We found one!
            for(int i = 0; i < 9; i++)
            {
                bdPrintDecimal("", nums[i], (i + 1) % 3 == 0 ? "\n" : ",  ");
            }
            printf("\nSeeds:\n");
            for(int i = 0; i < 3; i++)
            {
                bdPrintDecimal("", seeds[i], "\n");
            }
            break;
        }
        if(try != 0 && try % 1000000 == 0)
        {
            printf("\n%llu tries...\n", try);
            printf("Current square:\n");
            for(int i = 0; i < 9; i++)
            {
                bdPrintDecimal("", nums[i], (i + 1) % 3 == 0 ? "\n" : ",  ");
            }
            printf("\nCurrent seeds:\n");
            for(int i = 0; i < 3; i++)
            {
                bdPrintDecimal("", seeds[i], "\n");
            }
        }
        bdIncrement(seeds[0]);
        if(bdIsEqual_ct(seeds[0], seeds[1]))
        {
            bdIncrement(seeds[1]);
            bdSetShort(seeds[0], 1);
        }
        if(bdIsEqual_ct(seeds[1], seeds[2]))
        {
            bdIncrement(seedLevel);
            bdSetShort(seeds[0], 1);
            bdSetShort(seeds[1], 2);
            bdSquare(seeds[2], seedLevel);
            // printf("Seed level: ");
            // bdPrintDecimal("", seedLevel, "\n");
        }
    } while(1);
    
    for(int i = 0; i < 3; i++)
    {
        bdFree(&seeds[i]);
    }
    for(int i = 0; i < 9; i++)
    {
        bdFree(&nums[i]);
    }
    return 0;
}

int checkSquare(BIGD *seeds, BIGD *nums)
{
    // First, check to make sure all conditions for the seeds are met.
    // If one is not met, return -1 to indicate an invalid set of seeds.
    if(bdIsZero_ct(seeds[0]) || bdIsZero_ct(seeds[1]) || bdIsZero_ct(seeds[2]))
    {
        return -1;
    }
    if(bdCompare(seeds[0], seeds[1]) >= 0)
    {
        return -1;
    }
    if(bdCompare(seeds[0], seeds[2]) >= 0)
    {
        return -1;
    }
    BIGD cma = bdNew();
    bdSubtract(cma, seeds[2], seeds[0]);
    if(bdCompare(seeds[1], cma) >= 0)
    {
        bdFree(&cma);
        return -1;
    }
    bdFree(&cma);
    BIGD apa = bdNew();
    bdAdd(apa, seeds[0], seeds[0]);
    if(bdIsEqual_ct(seeds[1], apa))
    {
        bdFree(&apa);
        return -1;
    }
    bdFree(&apa);
    BIGD checker = bdNew();
    bdSqrt(checker, nums[4]);
    bdSquare_s(checker, checker);
    if(!bdIsEqual_ct(checker, nums[4]))
    {
        bdFree(&checker);
        return -1;
    }
    
    // Fill in the magic square with the values calculated
    // from the seeds being careful to never have a negative
    // number as an intermediate value.
    BIGD apb = bdNew();
    bdAdd(apb, seeds[0], seeds[1]);
    
    bdSubtract(nums[0], seeds[2], seeds[1]);
    bdAdd(nums[1], seeds[2], apb);
    bdSubtract(nums[2], seeds[2], seeds[0]);
    
    bdAdd(nums[3], seeds[2], seeds[1]);
    bdSubtract(nums[3], nums[3], seeds[0]);
    
    bdSetEqual(nums[4], seeds[2]);
    
    bdAdd(nums[5], seeds[2], seeds[0]);
    bdSubtract(nums[5], nums[5], seeds[1]);
    
    bdAdd(nums[6], seeds[2], seeds[0]);
    bdSubtract(nums[7], seeds[2], apb);
    bdAdd(nums[8], seeds[2], seeds[1]);
    
    // Check if all nums are square numbers
    int allAreSquares = 1;
    for(int i = 0; i < 9; i++)
    {
        bdSqrt(checker, nums[i]);
        bdSquare_s(checker, checker);
        allAreSquares &= bdIsEqual_ct(checker, nums[i]);
        if(!allAreSquares)
        {
            break;
        }
    }
    
    bdFree(&apb);
    bdFree(&checker);
    
    return allAreSquares;
}