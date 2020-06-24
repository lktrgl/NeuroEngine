#include <cppapp/smoke_test_neuron.h>
#include <cppapp/smoke_test_find_minimum.h>

int main ( [[maybe_unused]]int argc, [[maybe_unused]]char* argv[] )
{
  smoke_test_neuron();

  smoke_test_neuron_line();

  smoke_test_find_minimum_dichotomie();

  smoke_test_find_minimum_gold_ratio();

  smoke_test_quick_descent();

  return 0;
}
