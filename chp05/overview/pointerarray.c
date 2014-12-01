#include<stdio.h>

int main(){
   int x[5] = { 100, 200, 300, 400, 500 };
   int *p = &x[0], i;

   for(i=0; i<5; i++){
      (*(p++))++;
   }
   for(i=0; i<5; i++){
      printf("The value of x[%d] is %d\n", i, x[i]);
   }
   return 0;
}
