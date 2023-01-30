#include "foo.h"

#include <iostream>
// #include <Eigen/Dense>

/*****************************************
 * 
 * C++ library goes here
 * 
 *****************************************/
class MyClass {
  public:
    int num;
};

int cppFoo() {
  MyClass my_obj;
  my_obj.num = 1;

  // Eigen::MatrixXd m(2,2);
  // m(0,0) = 1;
  // m(1,0) = 2;
  // m(0,1) = 3;
  // m(1,1) = 4;
  // std::cout << m << std::endl;

  return 1;
}


/*****************************************
 * 
 * C API for C++ library goes here
 * 
 *****************************************/
extern "C" {

int cFoo() {
  return cppFoo();
}

}
