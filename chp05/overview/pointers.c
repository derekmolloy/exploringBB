#include<stdio.h>

int main(){
   int y = 1000;
   int *p;
   p = &y;
   printf("The variable has value %d and the address %p.\n", y, &y);
   printf("The pointer stores %p and points at value %d.\n", p, *p);
   printf("The pointer itself has address %p and size %d.\n", &p, sizeof(p));
   return 0;
}
