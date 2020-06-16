#include <nnet/neuron.h>
#include <nnet/neuron_line.h>

#include <cassert>
#include <iostream>

int main ( [[maybe_unused]]int argc, [[maybe_unused]]char* argv[] )
{
  {
    // smoke test for the nnet::neuron_t<>

    using input_t = int;

    constexpr size_t input_dimension = 3;
    constexpr input_t expected_result = 6;

    using my_neuron_t = nnet::neuron_t<input_t, input_dimension>;

    my_neuron_t neuron;

    constexpr my_neuron_t::koef_array_t const koefs = {1, 2, 3};
    constexpr my_neuron_t::input_array_t const inputs = {1, 1, 1};

    neuron.set_koefs ( koefs );
    neuron.apply ( inputs );

    assert ( expected_result == neuron.get_value() );
  }

  {
    // smoke test for the nnet::neuron_line_t<>

    using input_t = int;

    constexpr size_t input_dimension = 3;
    constexpr size_t line_dimension = 3;
    constexpr input_t expected_result = 6;

    using my_neuron_line_t = nnet::neuron_line_t<input_t, input_dimension, line_dimension>;

    my_neuron_line_t neuron_line;

    constexpr my_neuron_line_t::neuron_t::koef_array_t const koefs = {1, 2, 3};
    constexpr my_neuron_line_t::neuron_t::input_array_t const inputs = {1, 1, 1};

    for ( size_t i = 0; i < line_dimension; ++i )
    {
      neuron_line[i].set_koefs ( koefs );
    }

    neuron_line.apply ( inputs );

    for ( size_t i = 0; i < line_dimension; ++i )
    {
      assert ( expected_result == neuron_line[i].get_value() );
    }
  }

  return 0;
}
