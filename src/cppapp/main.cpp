#include <nnet/neuron.h>
#include <nnet/neuron_line.h>
#include <noptim/metrics.h>
#include <noptim/extreme.h>

#include <cassert>
#include <cmath>

// smoke test for the nnet::neuron_t<>
void smoke_test_neuron()
{
  constexpr size_t input_dimension = 3;

  using input_t = int;
  using my_neuron_t = nnet::neuron_t<input_t, input_dimension>;

  constexpr input_t expected_result = 6;
  constexpr input_t expected_loss = 0;
  constexpr input_t fake_expected_result = 2;
  constexpr input_t fake_expected_loss = ( expected_result - fake_expected_result ) *
                                         ( expected_result - fake_expected_result );

  constexpr my_neuron_t::koef_array_t const koefs = {1, 2, 3};
  constexpr my_neuron_t::input_array_t const inputs = {1, 1, 1};

  my_neuron_t neuron;

  neuron.set_koefs ( koefs );
  neuron.apply ( inputs );

  assert ( expected_result == neuron.get_value() );

  assert ( expected_loss == noptim::neuron_loss<input_t> ( neuron, expected_result ) );
  assert ( fake_expected_loss == noptim::neuron_loss<input_t> ( neuron, fake_expected_result ) );
}

// smoke test for the nnet::neuron_line_t<>
void smoke_test_neuron_line()
{
  constexpr size_t input_dimension = 3;
  constexpr size_t line_dimension = 3;

  using input_t = int;
  using my_neuron_line_t = nnet::neuron_line_t<input_t, input_dimension, line_dimension>;

  constexpr input_t expected_result = 6;
  constexpr input_t expected_loss = 0;
  constexpr input_t fake_expected_result_value = 2;
  constexpr input_t fake_expected_loss = line_dimension * ( expected_result - fake_expected_result_value )
                                         * ( expected_result - fake_expected_result_value );

  constexpr my_neuron_line_t::neuron_t::koef_array_t const koefs = {1, 2, 3};
  constexpr my_neuron_line_t::neuron_t::input_array_t const inputs = {1, 1, 1};
  constexpr my_neuron_line_t::output_array_t const expected_results = {expected_result, expected_result, expected_result};
  constexpr my_neuron_line_t::output_array_t const fake_expected_results = {fake_expected_result_value, fake_expected_result_value, fake_expected_result_value};

  my_neuron_line_t neuron_line;

  for ( size_t i = 0; i < line_dimension; ++i )
  {
    neuron_line[i].set_koefs ( koefs );
  }

  neuron_line.apply ( inputs );

  assert ( expected_results == neuron_line.get_value() );

  assert ( expected_loss == noptim::neuron_line_loss<input_t> ( neuron_line, expected_results ) );
  assert ( fake_expected_loss == noptim::neuron_line_loss<input_t> ( neuron_line, fake_expected_results ) );
}

// smoke test for the noptim::find_minimum_dichotomie<>(.)
void smoke_test_find_minimum_dichotomie()
{
  {
    auto my_f = [] ( double x )->double
    {
      return ( x - 1.0 ) * ( x - 1.0 );
    };

    auto const xa = -1.0;
    auto const xb = 2.0;
    auto const eps = 0.01;

    auto const expected_x_min = 1.0;

    noptim::find_minimum_t stat;
    auto const x_min = noptim::find_minimum_dichotomie ( xa, xb, eps, my_f, &stat );

    assert ( fabs ( x_min - expected_x_min ) <= eps );
  }

  {
    constexpr auto const d = 10.0;

    auto my_f = [] ( double x )->double
    {
      return - ( d* d * x - x* x * x );
    };

    auto const xa = 0.;
    auto const xb = d;
    auto const eps = 0.01;

    constexpr auto const expected_x_min = d / sqrt ( 3.0 );

    noptim::find_minimum_t stat;
    auto const x_min = noptim::find_minimum_dichotomie ( xa, xb, eps, my_f, &stat );

    assert ( fabs ( x_min - expected_x_min ) <= eps );
  }
}

// smoke test for the noptim::find_minimum_gold_ratio<>(.)
void smoke_test_find_minimum_gold_ratio()
{
  {
    auto my_f = [] ( double x )->double
    {
      return ( x - 1.0 ) * ( x - 1.0 );
    };

    auto const xa = -1.0;
    auto const xb = 2.0;
    auto const eps = 0.01;

    auto const expected_x_min = 1.0;

    noptim::find_minimum_t stat;
    auto const x_min = noptim::find_minimum_gold_ratio ( xa, xb, eps, my_f, &stat );

    assert ( fabs ( x_min - expected_x_min ) <= eps );
  }

  {
    constexpr auto const d = 10.0;

    auto my_f = [] ( double x )->double
    {
      return - ( d* d * x - x* x * x );
    };

    auto const xa = 0.;
    auto const xb = d;
    auto const eps = 0.01;

    constexpr auto const expected_x_min = d / sqrt ( 3.0 );

    noptim::find_minimum_t stat;
    auto const x_min = noptim::find_minimum_gold_ratio ( xa, xb, eps, my_f, &stat );

    assert ( fabs ( x_min - expected_x_min ) <= eps );
  }
}

int main ( [[maybe_unused]]int argc, [[maybe_unused]]char* argv[] )
{
  smoke_test_neuron();

  smoke_test_neuron_line();

  smoke_test_find_minimum_dichotomie();

  smoke_test_find_minimum_gold_ratio();

  return 0;
}
