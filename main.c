#include <stdio.h>
#include "bigd.h"

/*
 *   a | b | c 
 *  ---+---+---
 *   d | e | f 
 *  ---+---+---
 *   g | h | i 
*/

int checkSquare(BIGD *nums);

int main(int argc, char *argv[])
{
    // Initialize square as a=1, b=2, ...
    BIGD nums[9];
    for(int i = 0; i < 9; i++)
    {
        nums[i] = bdNew();
        bdSetShort(nums[i], i + 1);
    }
    int result;
    unsigned long long try = 0;
    do
    {
        try++;
        result = checkSquare(&nums[0]);
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
        if(try % 1000000 == 0)
        {
            printf("\n%llu tries...\n", try);
            printf("Current square:\n");
            for(int i = 0; i < 9; i++)
            {
                bdPrintDecimal("", nums[i], (i + 1) % 3 == 0 ? "\n" : ",  ");
            }
        }
    } while(1);
    
    for(int i = 0; i < 9; i++)
    {
        bdFree(&nums[i]);
    }
    return 0;
}

int checkSquare(BIGD *nums)
{
    static int rowTiebreaker = 1;
    static int colTiebreaker = 0;
    
    BIGD squares[9];
    for(int i = 0; i < 9; i++)
    {
        squares[i] = bdNew();
    }
    for(int i = 0; i < 9; i++)
    {
        bdSquare(squares[i], nums[i]);
    }
    BIGD rowSums[4];
    for(int i = 0; i < 4; i++)
    {
        rowSums[i] = bdNew();
    }
    BIGD colSums[4];
    for(int i = 0; i < 4; i++)
    {
        colSums[i] = bdNew();
    }
    for(int i = 0; i < 3; i++)
    {
        for(int j = i * 3; j < i * 3 + 3; j++)
        {
            bdAdd(rowSums[i], rowSums[i], squares[j]);
        }
    }
    bdAdd(rowSums[3], squares[0], squares[4]);
    bdAdd(rowSums[3], rowSums[3], squares[8]);
    
    for(int i = 0; i < 3; i++)
    {
        for(int j = i; j < i + 9; j += 3)
        {
            bdAdd(colSums[i], colSums[i], squares[j]);
        }
    }
    bdAdd(colSums[3], squares[2], squares[4]);
    bdAdd(colSums[3], colSums[3], squares[6]);
    
    // Check if all are equal
    int rowsAreEqual = 1;
    for(int i = 0; i < 3; i++)
    {
        rowsAreEqual &= bdIsEqual_ct(rowSums[i], rowSums[i + 1]);
    }
    int colsAreEqual = 1;
    for(int i = 0; i < 3; i++)
    {
        colsAreEqual &= bdIsEqual_ct(colSums[i], colSums[i + 1]);
    }
    if(rowsAreEqual && colsAreEqual)
    {
        return 0;
    }
    
    // Figure out which entry to increment
    int minRow = 0;
    for(int i = 1; i < 4; i++)
    {
        if(bdCompare_ct(rowSums[minRow], rowSums[i]) > 0)
        {
            minRow = i;
        }
        else if(bdCompare_ct(rowSums[minRow], rowSums[i]) == 0)
        {
            rowTiebreaker = ++rowTiebreaker % 4;
            if(rowTiebreaker)
            {
                minRow = i;
            }
        }
    }
    int minCol = 0;
    for(int i = 1; i < 4; i++)
    {
        if(bdCompare_ct(colSums[minCol], colSums[i]) > 0)
        {
            minCol = i;
        }
        else if(bdCompare_ct(colSums[minCol], colSums[i]) == 0)
        {
            colTiebreaker = ++colTiebreaker % 3;
            if(colTiebreaker)
            {
                minCol = i;
            }
        }
    }
    int returnVals[4][4] = 
    {
        {1, 2, 3, 3},
        {4, 5, 6, 5},
        {7, 8, 9, 7},
        {1, 5, 9, 5}
    };
    
    
    for(int i = 0; i < 9; i++)
    {
        bdFree(&squares[i]);
    }
    for(int i = 0; i < 4; i++)
    {
        bdFree(&rowSums[i]);
    }
    for(int i = 0; i < 4; i++)
    {
        bdFree(&colSums[i]);
    }
    
    return returnVals[minRow][minCol];
}