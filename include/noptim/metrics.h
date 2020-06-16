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

} /* namespace noptim */

