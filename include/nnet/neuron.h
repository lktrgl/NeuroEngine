#pragma once

#include <cstdint>
#include <array>
#include <numeric>

namespace nnet
{

template<typename input_t, size_t INPUT_DIMMENSION>
struct neuron_t
{
  using output_t = input_t;
  using koef_t = input_t;
  using koef_array_t = std::array<koef_t, INPUT_DIMMENSION>;
  using input_array_t = std::array<input_t, INPUT_DIMMENSION>;

  constexpr koef_array_t const& get_koefs () const noexcept
  {
    return koef;
  }

  constexpr void set_koefs ( koef_array_t const& new_koef ) noexcept
  {
    koef = new_koef;
  }

  constexpr void apply ( input_array_t const& input ) noexcept
  {
    value = std::inner_product ( koef.cbegin(), koef.cend(),
                                 input.cbegin(),
                                 output_t{} );
  }

  constexpr output_t get_value() const noexcept
  {
    return value;
  }

private:
  output_t value{};
  koef_array_t koef{};
};

} /* namespace nnet */

