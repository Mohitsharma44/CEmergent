#include <boost/python.hpp>

double volume(double a, double b, double c, double d=1, double e=1){
  return a*b*c*d*e;
}

BOOST_PYTHON_FUNCTION_OVERLOADS(volume_overloads, volume, 3, 5);

BOOST_PYTHON_MODULE(func_overload){
  boost::python::def("volume", volume, volume_overloads());
}
