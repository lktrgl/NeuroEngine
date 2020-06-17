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
  using input_array_t = typename neuron_t::input_array_t;
  using output_array_t = std::array<typename neuron_t::output_t, LINE_DIMENSION>;

  neuron_t& operator[] ( size_t k )
  {
    return line[k];
  }

  neuron_t const& operator[] ( size_t k ) const
  {
    return line[k];
  }

  size_t size() const noexcept
  {
    return line.size();
  }

  neuron_t* data() noexcept
  {
    return line.data();
  }

  neuron_t const* data() const noexcept
  {
    return line.data();
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

  output_array_t get_value() const noexcept
  {
    output_array_t result{};

    auto it = result.begin();

    for ( auto& n : line )
    {
      *it = n.get_value();
      ++it;
    }

    return result;
  }

private:
  line_t line{};
};

} // namespace nnet

