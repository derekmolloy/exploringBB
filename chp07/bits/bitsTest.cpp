/* Bits test by Derek Molloy */

#include<iostream>
#include<stdint.h>
#include<bitset>
#include<sstream>
#include<iomanip>
using namespace std;

string display(uint8_t a) {
   stringstream ss;
   ss << setw(3) << (int)a << "(" << bitset<8>(a) << ")";
   return ss.str();
}

int main(){
   uint8_t a = 25, b = 5;
   cout << "A is " << display(a) << " and B is " << display(b) << endl;
   cout << "A & B  (AND) is " << display(a & b) << endl;
   cout << "A | B  (OR)  is " << display(a | b) << endl;
   cout << "  ~A   (NOT) is " << display(~a) << endl;
   cout << "A ^ B  (XOR) is " << display(a ^ b) << endl;
   cout << "A << 1 (LSL) is " << display(a << 1) << endl;
   cout << "B >> 1 (LSR) is " << display(b >> 1) << endl;
   cout << "1 << 8 (LSL) is " << display(1 << 8) << endl; // ignore warning!
   return 0;
}
