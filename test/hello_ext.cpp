# include <iostream>
# include <string>

char const* greet()
{
  return "Hello World!";
}

# include <boost/python.hpp>
BOOST_PYTHON_MODULE(hello_ext)
{
  using namespace boost::python;
  def("greet", greet);
}
