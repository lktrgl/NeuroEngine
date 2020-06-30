#include <cppapp/smoke_test_all.h>

#include <cppapp/smoke_test_neuron.h>
#include <cppapp/smoke_test_find_minimum.h>
#include <cppapp/smoke_test_quick_descent.h>
#include <cppapp/smoke_test_diffsolve.h>
#include <cppapp/smoke_test_integral.h>

void test_all_the_components()
{
  test_neuron();

  test_find_minimum();

  test_quick_descent();

  test_diffsolve();

  test_integral();
};


