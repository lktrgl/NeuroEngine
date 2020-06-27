#include <cppapp/smoke_test_neuron.h>
#include <cppapp/smoke_test_find_minimum.h>
#include <cppapp/smoke_test_diffsolve.h>

int main ( [[maybe_unused]]int argc, [[maybe_unused]]char* argv[] )
{
  smoke_test_neuron();

  smoke_test_neuron_line();

  smoke_test_find_minimum_dichotomie();

  smoke_test_find_minimum_gold_ratio();

  smoke_test_quick_descent_dichotomie();

  smoke_test_quick_descent_gold_ratio();

  smoke_test_euler();

  smoke_test_runge_kutta_4th();

  smoke_test_runge_kutta_felberga_7th();

  return 0;
}
