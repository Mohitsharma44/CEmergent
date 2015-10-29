#include <boost/python.hpp>

namespace bp=boost::python;

class Matrix {
 public:
  Matrix(unsigned int x, unsigned int y) : size_x_(x), size_y_(y) {}
  unsigned int size_x() const { return size_x_; }
  unsigned int size_y() const { return size_y_; }

 private:
  unsigned int size_x_;
  unsigned int size_y_;
};

BOOST_PYTHON_MODULE(matrix_class){
  bp::class_<Matrix>("Matrix", bp::init<unsigned int, unsigned int>())
    .def("size_x", &Matrix::size_x)
    .def("size_y", &Matrix::size_y)
    ;
}
