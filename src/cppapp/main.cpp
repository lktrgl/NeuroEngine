#include <nnet/neuron.h>

#include <cassert>
#include <iostream>

int main ( [[maybe_unused]]int argc, [[maybe_unused]]char* argv[] )
{
  {
    // smoke test for the nnet::neuron_t<>

    using my_neuron_t = nnet::neuron_t<int, 3>;

    my_neuron_t neuron;

    constexpr my_neuron_t::koef_array_t const koefs = {1, 2, 3};
    constexpr my_neuron_t::input_array_t const inputs = {1, 1, 1};

    neuron.set_koefs ( koefs );
    neuron.apply ( inputs );

    assert ( 6 == neuron.get_value() );
  }

  return 0;
}
