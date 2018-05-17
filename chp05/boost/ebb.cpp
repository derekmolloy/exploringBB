#include<string>
#include<boost/python.hpp>           // .hpp convention for c++ headers
using namespace std;                 // just like cpp for source files

namespace exploringbb{               // keep the global namespace clean

   string hello(string name) {       // e.g., returns "Hello Derek!"
      return ("Hello " + name + "!");
   }

   double integrate(double a, double b, int n) {     // same as before
      double sum=0, dx = (b-a)/n;
      for(int i=0; i<n; i++){  sum += sin((a+i)*dx);  }
      return sum*dx;
   }
}

BOOST_PYTHON_MODULE(ebb){             // the module is called ebb
   using namespace boost::python;     // require the boost.python namespace
   using namespace exploringbb;       // bring in custom namespace
   def("hello", hello);               // make hello() visible to Python
   def("integrate", integrate);       // make integrate() also visible 
}

