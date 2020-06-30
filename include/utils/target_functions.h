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

struct test_function_2nd_degree_system
{
  static constexpr size_t const system_rank = 2;

  using funct_ret_t = double;
  using funct_arg_t = double;
  using funct_args_t = tuple_utils::funct_args_t<funct_arg_t, funct_arg_t>;
  using target_function_t = tuple_utils::target_nonstationary_function_t<funct_ret_t, funct_arg_t, funct_arg_t>;
  using target_function_array_t = tuple_utils::target_function_array_t<target_function_t, system_rank>;

  static constexpr auto const t0 = 0.0;
  static constexpr auto const t1 = 4.0;
  static constexpr auto const T = 0.09;
  static constexpr auto const mju = 1.2;
  static constexpr auto const k = 1.0;

  static constexpr auto const A = 1.0;
  static constexpr auto const tImp = ( t1 - t0 ) / 2.0;

  static constexpr funct_args_t const initial_state = {0.0, 0.0};

  static constexpr funct_args_t const expected_end_value = {0.0, 0.0};


  static target_function_array_t const system_definition;
};

inline
test_function_2nd_degree_system::target_function_array_t const
test_function_2nd_degree_system::system_definition =
{
  [] ( [[maybe_unused]]funct_arg_t t, [[maybe_unused]]funct_args_t const & x )->funct_ret_t
  {
    return std::get<1> ( x );
  },

  [] ( [[maybe_unused]]funct_arg_t t, [[maybe_unused]]funct_args_t const & x )->funct_ret_t
  {
    auto my_xin = [] ( [[maybe_unused]]funct_arg_t t )->funct_ret_t
    {
      funct_ret_t result{};

      if ( t >= t0 and t <= t0 + tImp )
      {
        result = A;
      }

      return result;
    };

    return -mju / T * std::get<1> ( x ) - 1.0 / ( T * T ) * std::get<0> ( x ) + k / ( T * T ) * my_xin ( t );
  }
};

}  // namespace target_function_utils
