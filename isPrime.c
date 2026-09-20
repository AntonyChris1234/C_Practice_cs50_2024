#include <stdio.h>

int isPrime(int num);
int main(int argc, char* argv[])
{
    for (int i = 0; i < 100; i++)
    {
        isPrime(i);
    }
}

int isPrime(int num)
{
    if(num%2 == 0 && num != 1 && num != 2)
    {
        printf("%d is not a prime number\n", num);
        return 0;
    }
        for(int i = 3; i <= 9; i+=2)
        {
            if(num % i == 0 && i != num)
            {
                printf("%d is not a prime number\n", num);
                return 0;
            }
        }
        printf("%d is a prime number\n", num);
        return 0;
}