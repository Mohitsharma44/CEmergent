# include <iostream>
# include <string>
# include <boost/python.hpp>

char const* greet()
{
  return "Hello World!";
}

BOOST_PYTHON_MODULE(hello_ext)
{
  using namespace boost::python;
  def("greet", greet);
}
