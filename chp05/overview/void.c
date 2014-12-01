#include<stdio.h>

int main(){
   int a = 5;
   void *p = &a;
   printf("The pointer p points at address %p and value %d\n", p, *((int *)p));
   return 0;
}
