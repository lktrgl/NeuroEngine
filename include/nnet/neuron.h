#pragma once

#include <cstdint>
#include <array>
#include <numeric>

namespace nnet
{

template<typename INPUT_T,
         size_t INPUT_DIMENSION>
struct neuron_t
{
  using input_t = INPUT_T;
  using output_t = input_t;
  using koef_t = input_t;
  using koef_array_t = std::array<koef_t, INPUT_DIMENSION>;
  using input_array_t = std::array<input_t, INPUT_DIMENSION>;

  koef_array_t& get_koefs () noexcept
  {
    return koef;
  }

  koef_array_t const& get_koefs () const noexcept
  {
    return koef;
  }

  void set_koefs ( koef_array_t const& new_koef ) noexcept
  {
    koef = new_koef;
  }

  void apply ( input_array_t const& input ) noexcept
  {
    value = std::inner_product ( koef.cbegin(), koef.cend(),
                                 input.cbegin(),
                                 output_t{} );
  }

  output_t get_value() const noexcept
  {
    return value;
  }

private:
  output_t value{};
  koef_array_t koef{};
};

} /* namespace nnet */

