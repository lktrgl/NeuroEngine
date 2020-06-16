#pragma once

#include <nnet/neuron.h>

#include <cstdint>
#include <array>

namespace nnet
{

template<typename INPUT_T,
         size_t INPUT_DIMENSION,
         size_t LINE_DIMENSION>
struct neuron_line_t
{
  using input_t = INPUT_T;
  using neuron_t = nnet::neuron_t<input_t, INPUT_DIMENSION>;
  using line_t = std::array<neuron_t, LINE_DIMENSION>;

  neuron_t& operator[] ( size_t k )
  {
    return line[k];
  }

  neuron_t const& operator[] ( size_t k ) const
  {
    return line[k];
  }

  void set_koefs ( size_t k, typename neuron_t::koef_array_t const& new_koef ) noexcept
  {
    line[k].set_koefs ( new_koef );
  }

  void apply ( typename neuron_t::input_array_t const& input ) noexcept
  {
    for ( auto& n : line )
    {
      n.apply ( input );
    }
  }

private:
  line_t line{};
};

} /* namespace nnet */

