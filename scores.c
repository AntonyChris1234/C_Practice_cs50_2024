#include "cs50.h"
#include <stdio.h>

int main(void) {
    int scores[3];
    scores[0] = 23;
    scores[1] = 35;
    scores[2] = 50;

    printf("Average: %f.00\n", (scores[0] + scores[1] + scores[2])/ 3.0);
}