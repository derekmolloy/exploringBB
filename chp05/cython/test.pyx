cdef extern from "math.h":
   double sin(double x)

cpdef double integrate_sin(double a, double b, int N):
   cdef double dx, s
   cdef int i
   dx = (b-a)/N
   sum = 0
   for i in range(0,N):
      sum += sin(a+i*dx)
   return sum*dx

