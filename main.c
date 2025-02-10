#include <stdio.h>
#include "bigd.h"

/*
 *  0 < a < b < c-a
 *  b =/= 2a
 *
 *     c-b   | c+(a+b) |   c-a   
 *  ---------+---------+---------
 *   c-(a-b) |    c    |  c+(a-b)  
 *  ---------+---------+---------
 *     c+a   | c-(a+b) |  c+b 
*/

int checkSquare(BIGD *seeds, BIGD *nums, BIGD *sums);

int main(int argc, char *argv[])
{
    // Initialize square as a=1, b=2, ...
	BIGD seedLevel = bdNew();
	bdSetZero(seedLevel);
	BIGD seedRow = bdNew();
	bdSetEqual(seedRow, seedLevel);
    BIGD seeds[3];
    for(int i = 0; i < 3; i++)
    {
        seeds[i] = bdNew();
		bdSetZero(seeds[i]);
    }
    BIGD nums[9];
    BIGD sums[9];
    for(int i = 0; i < 9; i++)
    {
        nums[i] = bdNew();
        sums[i] = bdNew();
    }
    int result;
    unsigned long long try = 0;
    unsigned long long gigaTry = 0;
    do
    {
        try++;
        result = checkSquare(seeds, nums, sums);
        if(result == -1)
        {
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
        if(try == 1000000)
        {
			try = 0;
			gigaTry++;
            printf("\n%llu Million tries...\n", gigaTry);
            printf("Current square:\n");
            for(int i = 0; i < 9; i++)
            {
                bdPrintDecimal("", nums[i], (i + 1) % 3 == 0 ? "\n" : ",  ");
            }
            // printf("\nCurrent sums:\n");
            // for(int i = 0; i < 8; i++)
            // {
                // bdPrintDecimal("", sums[i], "\n");
            // }
            printf("\nCurrent seeds:\n");
            for(int i = 0; i < 3; i++)
            {
                bdPrintDecimal("", seeds[i], "\n");
            }
        }
		if(bdIsEqual_ct(seedLevel, seeds[0]))
		{
			bdIncrement(seedLevel);
			bdSetZero(seeds[0]);
			bdSetZero(seeds[1]);
			bdSetEqual(seeds[2], seedLevel);
			bdSetEqual(seedRow, seedLevel);
		}
		else if(bdIsEqual_ct(seedRow, seeds[1]))
		{
			bdSetZero(seeds[1]);
			bdIncrement(seeds[0]);
			bdDecrement(seedRow);
			bdSetEqual(seeds[2], seedRow);
		}
		else
		{
			bdIncrement(seeds[1]);
			bdDecrement(seeds[2]);
		}
    } while(1);
    
    for(int i = 0; i < 3; i++)
    {
		bdFree(&seeds[i]);
    }
    for(int i = 0; i < 9; i++)
    {
        bdFree(&nums[i]);
        bdFree(&sums[i]);
    }
    return 0;
}

int checkSquare(BIGD *seeds, BIGD *nums, BIGD *sums)
{
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
		BIGD checker = bdNew();
		bdSqrt(checker, nums[i]);
		bdSquare_s(checker, checker);
		allAreSquares &= bdIsEqual_ct(checker, nums[i]);
		bdFree(&checker);
		if(!allAreSquares)
		{
			break;
		}
	}
	
    /*
	for(int i = 0; i < 8; i++)
    {
        bdSetZero(sums[i]);
    }
    for(int i = 0; i < 3; i++)
    {
        for(int j = i * 3; j < i * 3 + 3; j++)
        {
            bdAdd(sums[i], sums[i], nums[j]);
        }
    }
    for(int i = 3; i < 6; i++)
    {
        for(int j = i - 3; j < 9; j += 3)
        {
            bdAdd(sums[i], sums[i], nums[j]);
        }
    }
    bdAdd(sums[6], nums[0], nums[4]);
    bdAdd(sums[6], sums[6], nums[8]);
    
    bdAdd(sums[7], nums[2], nums[4]);
    bdAdd(sums[7], sums[7], nums[6]);
	
	// Check if all are equal
    int allAreEqual = 1;
    for(int i = 0; i < 7; i++)
    {
        allAreEqual &= bdIsEqual_ct(sums[i], sums[i + 1]);
    }
    */
	
	bdFree(&apb);
	
	return allAreSquares;// && allAreEqual;
}