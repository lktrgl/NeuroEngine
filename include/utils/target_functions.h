#pragma once

#include <utils/tuple_utils.h>

namespace target_function_utils
{

struct test_function_parabola_t
{
  double operator() ( double x )
  {
    return A * ( x - root_x ) * ( x - root_x ) + d;
  }

  template<typename ... ARGS>
  double operator() ( tuple_utils::funct_args_t<ARGS...> const& x )
  {
    return A * ( std::get<0> ( x ) - root_x ) * ( std::get<0> ( x ) - root_x )
           + B * ( std::get<1> ( x ) - root_y ) * ( std::get<1> ( x ) - root_y )
           + d;
  };

  static constexpr auto  A = 3.0;
  static constexpr auto  B = 4.0;
  static constexpr auto  d = 10.0;
  static constexpr auto root_x = 1.0;
  static constexpr auto root_y = 1.0;

  static constexpr auto const xa = -1.0;
  static constexpr auto const xb = 2.0;
  static constexpr auto const ya = 0.0;
  static constexpr auto const yb = 6.0;

  static constexpr auto const h = 0.01;
  static constexpr auto const eps = 0.01;

  static constexpr auto const expected_x_min = test_function_parabola_t::root_x;
  static constexpr auto const expected_y_min = test_function_parabola_t::root_y;
};

struct test_function_qubic_t
{
  double operator() ( double x )
  {
    return - ( d * d * x - x * x * x );
  }

  template<typename ... ARGS>
  double operator() ( tuple_utils::funct_args_t<ARGS...> const& x )
  {
    return - ( d * d * std::get<0> ( x ) - std::get<0> ( x ) * std::get<0> ( x ) * std::get<0> ( x ) );
  };

  static constexpr auto const d = 10.0;

  static constexpr auto const xa = 0.;
  static constexpr auto const xb = test_function_qubic_t::d;
  static constexpr auto const h = 0.01;
  static constexpr auto const eps = 0.01;

  static constexpr auto const expected_x_min = test_function_qubic_t::d / sqrt ( 3.0 );
};


}  // namespace target_function_utils
