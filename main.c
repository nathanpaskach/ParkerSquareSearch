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
    BIGD remainder = bdNew();
    BIGD seeds[3];
    // Initialize seeds as a=1, b=2, c=3
    for(int i = 0; i < 3; i++)
    {
        seeds[i] = bdNew();
        bdSetShort(seeds[i], i);
    }
    BIGD nums[9];
    for(int i = 0; i < 9; i++)
    {
        nums[i] = bdNew();
    }
    int result;
    unsigned long long try = 0;
    unsigned long long gigaTry = 0;
    do
    {
        try++;
        result = checkSquare(seeds, nums);
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
            try = 0;
            gigaTry++;
            printf("\n%llu million tries...\n", gigaTry);
            printf("\nCurrent seeds:\n");
            for(int i = 0; i < 3; i++)
            {
                bdPrintDecimal("", seeds[i], "\n");
            }
        }
        bdIncrement(seeds[0]);
        if(bdIsEqual_ct(seeds[0], seeds[1]))
        {
            BIGD checker = bdNew();
            BIGD goal = bdNew();
            bdSetZero(checker);
            bdSetZero(goal);
            int bIsValid = 0;
            while(!bIsValid && !bdIsEqual_ct(seeds[1], abLimit))
            {
                bdIncrement(seeds[1]);
                
                bdSubtract(goal, seeds[2], seeds[1]);
                bdSqrt(checker, goal);
                bdSquare_s(checker, checker);
                bIsValid |= bdIsEqual_ct(checker, goal);
                if(!bIsValid)
                {
                    continue;
                }
                bdAdd(goal, seeds[2], seeds[1]);
                bdSqrt(checker, goal);
                bdSquare_s(checker, checker);
                bIsValid &= bdIsEqual_ct(checker, goal);
            }
            bdFree(&checker);
            bdFree(&goal);
            bdSetShort(seeds[0], 1);
        }
        if(bdIsEqual_ct(seeds[1], abLimit))
        {
            bdIncrement(seedLevel);
            bdSetShort(seeds[0], 1);
            bdSetShort(seeds[1], 2);
            bdSquare(seeds[2], seedLevel);
            bdShortDiv(abLimit, remainder, seeds[2], 2);
            bdAdd(abLimit, abLimit, remainder);
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
        return 0;
    }
    if(bdCompare(seeds[0], seeds[1]) >= 0)
    {
        return 0;
    }
    if(bdCompare(seeds[0], seeds[2]) >= 0)
    {
        return 0;
    }
    BIGD cma = bdNew();
    bdSubtract(cma, seeds[2], seeds[0]);
    if(bdCompare(seeds[1], cma) >= 0)
    {
        bdFree(&cma);
        return 0;
    }
    bdFree(&cma);
    BIGD apa = bdNew();
    bdAdd(apa, seeds[0], seeds[0]);
    if(bdIsEqual_ct(seeds[1], apa))
    {
        bdFree(&apa);
        return 0;
    }
    bdFree(&apa);
    BIGD checker = bdNew();
    bdSqrt(checker, nums[4]);
    bdSquare_s(checker, checker);
    if(!bdIsEqual_ct(checker, nums[4]))
    {
        bdFree(&checker);
        return 0;
    }
    
    // Fill in the magic square with the values calculated
    // from the seeds being careful to never have a negative
    // number as an intermediate value.
    int allAreSquares = 1;
    BIGD apb = bdNew();
    bdAdd(apb, seeds[0], seeds[1]);
    
    bdSubtract(nums[0], seeds[2], seeds[1]);
    bdSqrt(checker, nums[0]);
    bdSquare_s(checker, checker);
    if(!bdIsEqual_ct(checker, nums[0]))
    {
        goto notAllSquares;
    }
    
    /* We already checked these two */
    // bdAdd(nums[8], seeds[2], seeds[1]);
    // bdSqrt(checker, nums[8]);
    // bdSquare_s(checker, checker);
    // if(!bdIsEqual_ct(checker, nums[8]))
    // {
        // goto notAllSquares;
    // }
    
    // bdAdd(nums[1], seeds[2], apb);
    // bdSqrt(checker, nums[1]);
    // bdSquare_s(checker, checker);
    // if(!bdIsEqual_ct(checker, nums[1]))
    // {
        // goto notAllSquares;
    // }
    
    bdSubtract(nums[2], seeds[2], seeds[0]);
    bdSqrt(checker, nums[2]);
    bdSquare_s(checker, checker);
    if(!bdIsEqual_ct(checker, nums[2]))
    {
        goto notAllSquares;
    }
    
    bdAdd(nums[3], seeds[2], seeds[1]);
    bdSubtract(nums[3], nums[3], seeds[0]);
    bdSqrt(checker, nums[3]);
    bdSquare_s(checker, checker);
    if(!bdIsEqual_ct(checker, nums[3]))
    {
        goto notAllSquares;
    }
    
    bdSetEqual(nums[4], seeds[2]);
    bdSqrt(checker, nums[4]);
    bdSquare_s(checker, checker);
    if(!bdIsEqual_ct(checker, nums[4]))
    {
        goto notAllSquares;
    }
    
    bdAdd(nums[5], seeds[2], seeds[0]);
    bdSubtract(nums[5], nums[5], seeds[1]);
    bdSqrt(checker, nums[5]);
    bdSquare_s(checker, checker);
    if(!bdIsEqual_ct(checker, nums[5]))
    {
        goto notAllSquares;
    }
    
    bdAdd(nums[6], seeds[2], seeds[0]);
    bdSqrt(checker, nums[6]);
    bdSquare_s(checker, checker);
    if(!bdIsEqual_ct(checker, nums[6]))
    {
        goto notAllSquares;
    }
    
    bdSubtract(nums[7], seeds[2], apb);
    bdSqrt(checker, nums[7]);
    bdSquare_s(checker, checker);
    if(!bdIsEqual_ct(checker, nums[7]))
    {
        goto notAllSquares;
    }
    goto allSquares;
    
    notAllSquares:
    allAreSquares = 0;
    
    allSquares:
    bdFree(&apb);
    bdFree(&checker);
    
    return allAreSquares;
}