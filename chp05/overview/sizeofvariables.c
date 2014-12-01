#include<stdio.h>
#include<stdbool.h>

int main(){
   double a = 3.14159;
   float b = 25.0;
   int c = 545;
   long int d = 123;
   char e = 'A';
   bool f = true;  // no need for definition in C++

   printf("a value %.4f and size %d bytes (@addr %p).\n", a, sizeof(a), &a);
   printf("b value %4.2f and size %d bytes (@addr %p).\n", b, sizeof(b), &b);
   printf("c value %d (oct %o, hex %x) and " \
          "size %d bytes (@addr %p).\n", c, c, c, sizeof(c), &c);
   printf("d value %d and size %d bytes (@addr %p).\n", d, sizeof(d), &d);
   printf("e value %c and size %d bytes (@addr %p).\n", e, sizeof(e), &e);
   printf("f value %5d and size %d bytes (@addr %p).\n", f, sizeof(f), &f);
}

