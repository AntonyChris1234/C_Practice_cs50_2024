#include "cs50.h"
#include <stdio.h>

void printColumn(int height);

int main(void){
    int h = get_int("Height: ");
    printColumn(h);
}

void printColumn(int height) {
    for (int i = 0; i < height; i++) {
        printf("#\n");
    }
}