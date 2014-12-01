#include<stdio.h>

int main(){
   int a=1, b=2, c, d, e, g;
   float f=9.9999;
   c = ++a;
   printf("The value of c=%d and a=%d.\n", c, a);
   d = b++;
   printf("The value of d=%d and b=%d.\n", d, b);
   e = (int) f;
   printf("The value of f=%.2f and e=%d.\n", f, e);
   g = 'A';
   printf("The value of g=%d and g=%c.\n", g, g);
   return 0;
}
