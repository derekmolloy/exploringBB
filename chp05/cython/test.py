from math import sin
def integrate_sin(a,b,N):
   dx = (b-a)/N
   sum = 0
   for i in range(0,N):
      sum += sin(a+i*dx)
   return sum*dx

