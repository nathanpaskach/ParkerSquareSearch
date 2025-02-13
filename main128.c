#include <stdio.h>
#include <stdlib.h>

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

#if (__SIZEOF_INT128__ == 16)
#define BIGINT __int128
#else
#define BIGING int64
#endif

unsigned BIGINT tmp;
unsigned BIGINT tmp2;
unsigned BIGINT digitalSum;
unsigned BIGINT remain;

int checkSquare(unsigned BIGINT *seeds, unsigned BIGINT *nums);
int isSquare(unsigned BIGINT n);
unsigned BIGINT int_sqrt(unsigned BIGINT n);
void print_bigint(unsigned BIGINT n);

int main(int argc, char *argv[])
{
    printf("Using %d bit integers\n", sizeof(unsigned BIGINT) * 8);
    // Initialize global unsigned BIGINT variables
    tmp = 0;
    tmp2 = 0;
    digitalSum = 0;
    remain = 0;
    
    // Initialize local unsigned BIGINT variables
    unsigned BIGINT seedLevel = 0;
    unsigned BIGINT abLimit = 0;
    unsigned BIGINT seeds[3];
    unsigned BIGINT nums[9];
    
    // Initialize seeds as a=1, b=2, c=3
    for(int i = 0; i < 3; i++)
    {
        seeds[i] = i;
    }
    
    for(int i = 0; i < 9; i++)
    {
        nums[i] = 0;
    }
    
    seedLevel = 3;
    if(argc > 1)
    {
        char *p;
        printf("setting C to %s^2\n", argv[1]);
        seedLevel = strtol(argv[1], &p, 10);
        seeds[2] = seedLevel * seedLevel;
    }
    abLimit = (seeds[2] / 2) + (seeds[2] % 2);
    
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
                print_bigint(nums[i]);
                printf("%c", (i + 1) % 3 == 0 ? "\n" : ",  ");
            }
            printf("\nSeeds:\n");
            for(int i = 0; i < 3; i++)
            {
                print_bigint(seeds[i]);
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
                print_bigint(seeds[i]);
                printf("\n");
            }
        }
        seeds[0]++;
        if(seeds[0] == seeds[1])
        {
            int bIsValid = 0;
            while(!bIsValid && seeds[1] != abLimit)
            {
                seeds[1]++;
                
                tmp2 = seeds[2] - seeds[1];
                bIsValid |= isSquare(tmp2);
                if(!bIsValid)
                {
                    continue;
                }
                tmp2 = seeds[2] + seeds[1];
                bIsValid &= isSquare(tmp2);
            }
            seeds[0] = 1;
        }
        if(seeds[1] == abLimit)
        {
            seedLevel++;
            seeds[0] = 1;
            seeds[1] = 2;
            seeds[2] = seedLevel * seedLevel;
            abLimit = (seeds[2] / 2) + (seeds[2] % 2);
        }
    } while(1);
    
    return 0;
}

int checkSquare(unsigned BIGINT *seeds, unsigned BIGINT *nums)
{
    // First, check to make sure all conditions for the seeds are met.
    // If one is not met, return -1 to indicate an invalid set of seeds.
    if(seeds[0] == 0 || seeds[1] == 0 || seeds[2] == 0)
    {
        return 0;
    }
    if(seeds[0] >= seeds[1])
    {
        return 0;
    }
    if(seeds[0] >= seeds[2])
    {
        return 0;
    }
    if(seeds[1] >= seeds[2] - seeds[0])
    {
        return 0;
    }
    if(seeds[1] == seeds[0] + seeds[0])
    {
        return 0;
    }
    
    // Fill in the magic square with the values calculated
    // from the seeds being careful to never have a negative
    // number as an intermediate value.
    // Check if they're squares along the way so we can
    // return as soon as one isn't a square
    int allAreSquares = 1;
    unsigned BIGINT apb = 0;
    apb = seeds[0] + seeds[1];
    
    nums[1] = seeds[2] + apb;
    if(!isSquare(nums[1]))
    {
        goto notAllSquares;
    }
    
    nums[2] = seeds[2] - seeds[0];
    if(!isSquare(nums[2]))
    {
        goto notAllSquares;
    }
    
    nums[3] = seeds[2] + seeds[1] - seeds[0];
    if(!isSquare(nums[3]))
    {
        goto notAllSquares;
    }
    
    nums[5] = seeds[2] + seeds[0] - seeds[1];
    if(!isSquare(nums[5]))
    {
        goto notAllSquares;
    }
    
    nums[6] = seeds[2] + seeds[0];
    if(!isSquare(nums[6]))
    {
        goto notAllSquares;
    }
    
    nums[7] = seeds[2] - apb;
    if(!isSquare(nums[7]))
    {
        goto notAllSquares;
    }
    goto allSquares;
    
    notAllSquares:
    allAreSquares = 0;
    
    allSquares:
    
    // We already checked these three, so just put them in the grid
    nums[0] = seeds[2] - seeds[1];
    nums[4] = seeds[2];
    nums[8] = seeds[2] + seeds[1];
    
    return allAreSquares;
}

int isSquare(unsigned BIGINT n)
{
    // Check least significant digit first
    int digit = n % 10;
    if(digit == 2 || digit == 3 || digit == 7 || digit == 8)
    {
        return 0;
    }
    
    // Check digital summation
    tmp = n;
    do
    {
        digitalSum = 0;
        while(tmp != 0)
        {
            remain = tmp % 10;
            tmp /= 10;
            digitalSum += remain;
        }
        tmp = digitalSum;
    } while(digitalSum > 9);
    
    digit = digitalSum % 10;
    if(digit == 0 || digit == 2 || digit == 3 || digit == 5 || digit == 6 || digit == 8)
    {
        return 0;
    }
    
    // Now check with sqrt and square functions
    tmp = int_sqrt(n);
    tmp = tmp * tmp;
    return tmp == n;
}

unsigned BIGINT int_sqrt(unsigned BIGINT n)
{
    if (n == 0)
    {
        return 0;
    }
    unsigned BIGINT low = 1, high = n, ans = 0;
    while (low <= high) {
        unsigned BIGINT mid = low + (high - low) / 2;
        if (mid <= n / mid)
        {
            ans = mid;
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return ans;
}

void print_bigint(unsigned BIGINT n)
{
    if (n == 0)
    {
        printf("0");
        return;
    }

    char buffer[40];
    int index = 39;
    buffer[index--] = '\0';

    unsigned BIGINT u = n;
    while (u > 0) {
        buffer[index--] = '0' + (u % 10);
        u /= 10;
    }
    printf("%s", &buffer[index + 1]);
}