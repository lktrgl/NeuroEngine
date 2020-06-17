#pragma once

namespace noptim
{

template<typename OUTPUT_T, typename NEURON_T>
OUTPUT_T neuron_loss (
  NEURON_T const& neuron,
  typename NEURON_T::output_t const& expected_value )
{
  auto const result = static_cast<OUTPUT_T> ( expected_value ) - static_cast<OUTPUT_T> ( neuron.get_value() );
  return result * result;
}

//-----------------------------------------------------------------------------

template<typename OUTPUT_T, typename NEURON_LINE_T>
OUTPUT_T neuron_line_loss (
  NEURON_LINE_T const& neuron_line,
  typename NEURON_LINE_T::output_array_t const& expected_value )
{
  OUTPUT_T result{};

  auto const* neuron_ptr = neuron_line.data();
  auto const* expected_value_ptr = expected_value.data();

  for ( size_t i = 0; i < neuron_line.size(); ++i, ++neuron_ptr, ++expected_value_ptr )
  {
    result += neuron_loss<OUTPUT_T> ( *neuron_ptr, *expected_value_ptr );
  }

  return result;
}

} /* namespace noptim */

