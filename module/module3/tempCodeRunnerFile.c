#include <stdio.h>

int main() {
    int a = 42;
    int *ptr_array[5];
    ptr_array[0] = &a;

    printf("ptr_array = %p\n", (void*)ptr_array);       // address of ptr_array[0]
    printf("*ptr_array = %p\n", (void*)*ptr_array);    // address of a
    printf("**ptr_array = %d\n", **ptr_array);         // value of a

    return 0;
}