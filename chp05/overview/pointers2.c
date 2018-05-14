#include<stdio.h>

int main(){
   int y  = 1000, z;
   int *p = &y;
   printf("The pointer  p has the value %d and address: %p\n", *p, p);
   // Let z = 1000 + 5 and the increment p and y to 1001 -- wrong!!!
   z = *p++ + 5;
   printf("The pointer  p has the value %d and address: %p\n", *p, p);
   printf("The variable z has the value %d\n", z);
   return 0;
}

