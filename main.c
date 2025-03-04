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


BIGD tmp;
BIGD tmp2;
BIGD digitalSum;
BIGD remain;
BIGD ten;

int checkSquare(BIGD *seeds, BIGD *nums);
int isSquare(BIGD n);

int main(int argc, char *argv[])
{
    // Initialize global BIGD variables
    tmp = bdNew();
    tmp2 = bdNew();
    digitalSum = bdNew();
    remain = bdNew();
    ten = bdNew();
    bdConvFromDecimal(ten, "10");
    
    // Initialize local BIGD variables
    BIGD seedLevel = bdNew();
    BIGD abLimit = bdNew();
    BIGD seeds[3];
    BIGD nums[9];
    
    // Initialize seeds as a=1, b=2, c=3
    for(int i = 0; i < 3; i++)
    {
        seeds[i] = bdNew();
        bdSetShort(seeds[i], i);
    }
    
    for(int i = 0; i < 9; i++)
    {
        nums[i] = bdNew();
    }
    
    bdSetShort(seedLevel, 3);
    if(argc > 1)
    {
        printf("setting C to %s^2\n", argv[1]);
        bdConvFromDecimal(seedLevel, argv[1]);
        bdSquare(seeds[2], seedLevel);
        bdShortDiv(abLimit, remain, seeds[2], 2);
        bdAdd(abLimit, abLimit, remain);
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
            if(argc > 1)
            {
                printf("(Started with C = %s^2)", argv[1]);
            }
            printf("\nCurrent seeds:\n");
            for(int i = 0; i < 3; i++)
            {
                printf("%c = ", 'A' + i);
                bdPrintDecimal("", seeds[i], "\n");
            }
        }
        bdIncrement(seeds[0]);
        if(bdIsEqual_ct(seeds[0], seeds[1]))
        {
            int bIsValid = 0;
            while(!bIsValid && !bdIsEqual_ct(seeds[1], abLimit))
            {
                bdIncrement(seeds[1]);
                
                bdSubtract(tmp2, seeds[2], seeds[1]);
                bIsValid |= isSquare(tmp2);
                if(!bIsValid)
                {
                    continue;
                }
                bdAdd(tmp2, seeds[2], seeds[1]);
                bIsValid &= isSquare(tmp2);
            }
            bdSetShort(seeds[0], 1);
        }
        if(bdIsEqual_ct(seeds[1], abLimit))
        {
            bdIncrement(seedLevel);
            bdSetShort(seeds[0], 1);
            bdSetShort(seeds[1], 2);
            bdSquare(seeds[2], seedLevel);
            bdShortDiv(abLimit, remain, seeds[2], 2);
            bdAdd(abLimit, abLimit, remain);
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
    bdFree(&seedLevel);
    bdFree(&abLimit);
    bdFree(&tmp);
    bdFree(&digitalSum);
    bdFree(&remain);
    bdFree(&ten);
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
    
    // Fill in the magic square with the values calculated
    // from the seeds being careful to never have a negative
    // number as an intermediate value.
    // Check if they're squares along the way so we can
    // return as soon as one isn't a square
    int allAreSquares = 1;
    BIGD checker = bdNew();
    BIGD apb = bdNew();
    bdAdd(apb, seeds[0], seeds[1]);
    
    bdAdd(nums[1], seeds[2], apb);
    if(!isSquare(nums[1]))
    {
        goto notAllSquares;
    }
    
    bdSubtract(nums[2], seeds[2], seeds[0]);
    if(!isSquare(nums[2]))
    {
        goto notAllSquares;
    }
    
    bdAdd(nums[3], seeds[2], seeds[1]);
    bdSubtract(nums[3], nums[3], seeds[0]);
    if(!isSquare(nums[3]))
    {
        goto notAllSquares;
    }
    
    bdAdd(nums[5], seeds[2], seeds[0]);
    bdSubtract(nums[5], nums[5], seeds[1]);
    if(!isSquare(nums[5]))
    {
        goto notAllSquares;
    }
    
    bdAdd(nums[6], seeds[2], seeds[0]);
    if(!isSquare(nums[6]))
    {
        goto notAllSquares;
    }
    
    bdSubtract(nums[7], seeds[2], apb);
    if(!isSquare(nums[7]))
    {
        goto notAllSquares;
    }
    goto allSquares;
    
    notAllSquares:
    allAreSquares = 0;
    
    allSquares:
    bdFree(&apb);
    bdFree(&checker);
    
    // We already checked these three, so just put them in the grid
    bdSubtract(nums[0], seeds[2], seeds[1]);
    bdSetEqual(nums[4], seeds[2]);
    bdAdd(nums[8], seeds[2], seeds[1]);
    
    return allAreSquares;
}

int isSquare(BIGD n)
{
    // Check least significant digit first
    uint32_t digit = bdToShort(n);
    if(digit == 2 || digit == 3 || digit == 7 || digit == 8)
    {
        return 0;
    }
    
    // Check digital summation
    bdSetEqual(tmp, n);
    do
    {
        bdSetZero(digitalSum);
        while(!bdIsZero_ct(tmp))
        {
            bdDivide_s(tmp, remain, tmp, ten);
            bdAdd(digitalSum, digitalSum, remain);
        }
        bdSetEqual(tmp, digitalSum);
    } while(bdShortCmp(digitalSum, 9) > 0);
    
    digit = bdToShort(digitalSum);
    if(digit == 0 || digit == 2 || digit == 3 || digit == 5 || digit == 6 || digit == 8)
    {
        return 0;
    }
    
    // Now check with sqrt and square functions
    bdSqrt(tmp, n);
    bdSquare_s(tmp, tmp);
    return bdIsEqual_ct(tmp, n);
}