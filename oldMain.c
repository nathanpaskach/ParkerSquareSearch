#include <stdio.h>
#include "bigd.h"

/*
 *   a | b | c 
 *  ---+---+---
 *   d | e | f 
 *  ---+---+---
 *   g | h | i 
*/

int checkSquare(BIGD *nums, BIGD *sums);

int main(int argc, char *argv[])
{
    // Initialize square as a=1, b=2, ...
    BIGD nums[9];
    int numToSet = 1;
    for(int i = 0; numToSet <= 9; i = (i + 7) % 9)
    {
        nums[i] = bdNew();
        bdSetShort(nums[i], numToSet++);
    }
    BIGD sums[9];
    for(int i = 0; i < 9; i++)
    {
        sums[i] = bdNew();
    }
    int result;
    unsigned long long try = 0;
    do
    {
        try++;
        result = checkSquare(nums, sums);
        if(result == 0)
        {
            // We found one!
            for(int i = 0; i < 9; i++)
            {
                bdPrintDecimal("", nums[i], (i + 1) % 3 == 0 ? "\n" : ",  ");
            }
            break;
        }
        int numIsRepeated;
        do
        {
            numIsRepeated = 0;
            bdIncrement(nums[result - 1]);
            for(int i = 0; i < 9; i++)
            {
                if(i == result - 1)
                {
                    continue;
                }
                numIsRepeated |= bdIsEqual_ct(nums[i], nums[result - 1]);
            }
        } while(numIsRepeated != 0);
        if(try % 100000 == 0)
        {
            printf("\n%llu tries...\n", try);
            printf("Current square:\n");
            for(int i = 0; i < 9; i++)
            {
                bdPrintDecimal("", nums[i], (i + 1) % 3 == 0 ? "\n" : ",  ");
            }
            printf("\nCurrent sums:\n");
            for(int i = 0; i < 8; i++)
            {
                bdPrintDecimal("", sums[i], "\n");
            }
        }
    } while(1);
    
    for(int i = 0; i < 9; i++)
    {
        bdFree(&nums[i]);
    }
    for(int i = 0; i < 9; i++)
    {
        bdFree(&sums[i]);
    }
    return 0;
}

int checkSquare(BIGD *nums, BIGD *sums)
{
    BIGD squares[9];
    BIGD numSums[9];
    for(int i = 0; i < 9; i++)
    {
        squares[i] = bdNew();
        bdSquare(squares[i], nums[i]);
        numSums[i] = bdNew();
        bdSetZero(numSums[i]);
    }
    BIGD allSums[8];
    for(int i = 0; i < 8; i++)
    {
        allSums[i] = bdNew();
        bdSetZero(allSums[i]);
    }
    for(int i = 0; i < 3; i++)
    {
        for(int j = i * 3; j < i * 3 + 3; j++)
        {
            bdAdd(allSums[i], allSums[i], squares[j]);
        }
    }
    for(int i = 3; i < 6; i++)
    {
        for(int j = i - 3; j < 9; j += 3)
        {
            bdAdd(allSums[i], allSums[i], squares[j]);
        }
    }
    bdAdd(allSums[6], squares[0], squares[4]);
    bdAdd(allSums[6], allSums[6], squares[8]);
    
    bdAdd(allSums[7], squares[2], squares[4]);
    bdAdd(allSums[7], allSums[7], squares[6]);
    
    // Check if all are equal
    int allAreEqual = 1;
    for(int i = 0; i < 7; i++)
    {
        allAreEqual &= bdIsEqual_ct(allSums[i], allSums[i + 1]);
    }
    if(allAreEqual)
    {
        return 0;
    }
    
    // Figure out which entry to increment
    int numSumList[9][5] =
    {
        {0, 3, 6, -1, -1},
        {0, 4, -1, -1, -1},
        {0, 5, 7, -1, -1},
        {1, 3, -1, -1, -1},
        {1, 4, 6, 7, -1},
        {1, 5, -1, -1, -1},
        {2, 3, 7, -1, -1},
        {2, 4, -1, -1, -1},
        {2, 5, 6, -1, -1}
    };
    BIGD remainder = bdNew();
    for(int i = 0; i < 9; i++)
    {
        int j = 0;
        while(numSumList[i][j] != -1)
        {
            bdAdd(numSums[i], numSums[i], allSums[numSumList[i][j]]);
            j++;
        }
        BIGD bigJ = bdNew();
        bdSetShort(bigJ, j);
        bdDivide_s(numSums[i], remainder, numSums[i], bigJ);
        bdFree(&bigJ);
    }
    bdFree(&remainder);
    // Fixing the first number as 1. Don't check it
    int minNumSumIndex = 1;
    for(int i = 2; i < 9; i++)
    {
        if(bdCompare_ct(numSums[minNumSumIndex], numSums[i]) > 0)
        {
            minNumSumIndex = i;
        }
    }
    
    for(int i = 0; i < 9; i++)
    {
        bdFree(&squares[i]);
        // bdSetEqual(sums[i], numSums[i]);
        bdFree(&numSums[i]);
    }
    for(int i = 0; i < 8; i++)
    {
        bdSetEqual(sums[i], allSums[i]);
        bdFree(&allSums[i]);
    }
    
    return minNumSumIndex + 1;
}