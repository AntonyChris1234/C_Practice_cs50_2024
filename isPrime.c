#include <stdio.h>

int isPrime(int num);
int main(int argc, char* argv[])
{
    isPrime(15);
}

int isPrime(int num)
{
    if(num%2 == 0)
    {
        printf("%d is not a prime number", num);
        return 0;
    }
    else
    {
        for(int i = 3; i <= 9; i+=2)
        {
            if(num % i == 0)
            {
                printf("%d is not a prime number", num);
                return 0;
            }
        }
        printf("%d is a prime number", num);
        return 0;
    }
}