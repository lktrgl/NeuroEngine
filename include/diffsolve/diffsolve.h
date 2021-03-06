#pragma once

#include <utils/tuple_utils.h>

#include <type_traits>
#include <tuple>
#include <array>
#include <cmath>

namespace diffsolve
{

enum class diffsolve_method
{
  euler,
  runge_kutta_4th,
  runge_kutta_felberga_7th
};

namespace diffsolve_details
{
template<typename T>
constexpr bool always_false()
{
  return false;
}

template<diffsolve_method METHOD_ENUM,
         size_t SYSTEM_RANK,
         typename FUNCT_ARG,
         typename ... ARGS>
struct diffsolve_traits
{
  using ret_type_t = FUNCT_ARG;
  using funct_arg_t = FUNCT_ARG;
  using funct_args_t = tuple_utils::funct_args_t<ARGS...>;
  using target_function_t = tuple_utils::target_nonstationary_function_t<funct_arg_t, ARGS...>;
  using target_function_array_t = tuple_utils::target_function_array_t<target_function_t, SYSTEM_RANK>;

  static funct_args_t
  evaluate_gradient ( target_function_array_t const& f,
                      funct_arg_t t,
                      funct_args_t const& y,
                      funct_arg_t tau )
  {
    static_assert ( always_false<FUNCT_ARG>(), "A specialization should be implemented instead !" );
    return {};
  }
};

template<size_t SYSTEM_RANK,
         typename FUNCT_ARG,
         typename ... ARGS>
struct diffsolve_traits<diffsolve_method::euler, SYSTEM_RANK, FUNCT_ARG, ARGS...>
{
  using ret_type_t = FUNCT_ARG;
  using funct_arg_t = FUNCT_ARG;
  using funct_args_t = tuple_utils::funct_args_t<ARGS...>;
  using target_function_t = tuple_utils::target_nonstationary_function_t<funct_arg_t, ARGS...>;
  using target_function_array_t = tuple_utils::target_function_array_t<target_function_t, SYSTEM_RANK>;

  static funct_args_t
  evaluate_gradient ( target_function_array_t const& f,
                      funct_arg_t t,
                      funct_args_t const& y,
                      funct_arg_t tau )
  {
    return evaluate_gradient_impl ( f,
                                    t,
                                    y,
                                    tau, std::make_index_sequence<SYSTEM_RANK>() );
  }

private:
  template <size_t ... Indexes>
  static funct_args_t
  evaluate_gradient_impl ( target_function_array_t const& f,
                           funct_arg_t t,
                           funct_args_t const& y,
                           funct_arg_t tau,
                           std::index_sequence<Indexes...> )
  {
    funct_args_t result{};

    ( ( std::get<Indexes> ( result ) = tau * f[Indexes] ( t, y ) ), ... );

    return result;
  }
};

template<size_t SYSTEM_RANK,
         typename FUNCT_ARG,
         typename ... ARGS>
struct diffsolve_traits<diffsolve_method::runge_kutta_4th, SYSTEM_RANK, FUNCT_ARG, ARGS...>
{
  using ret_type_t = FUNCT_ARG;
  using funct_arg_t = FUNCT_ARG;
  using funct_args_t = tuple_utils::funct_args_t<ARGS...>;
  using target_function_t = tuple_utils::target_nonstationary_function_t<funct_arg_t, ARGS...>;
  using target_function_array_t = tuple_utils::target_function_array_t<target_function_t, SYSTEM_RANK>;

  static funct_args_t
  evaluate_gradient ( target_function_array_t const& f,
                      funct_arg_t t,
                      funct_args_t const& y,
                      funct_arg_t tau )
  {
    return evaluate_gradient_impl ( f,
                                    t,
                                    y,
                                    tau, std::make_index_sequence<SYSTEM_RANK>() );
  }

private:
  template <size_t ... Indexes>
  static funct_args_t
  evaluate_gradient_impl ( target_function_array_t const& f,
                           funct_arg_t t,
                           funct_args_t const& y,
                           funct_arg_t tau,
                           std::index_sequence<Indexes...> )
  {
    funct_args_t result{};

    ( ( std::get<Indexes> ( result ) = evaluate_partial_delta_impl<Indexes> ( f, t, y, tau ) ), ... );

    return result;
  }

  template <size_t Index>
  static funct_arg_t
  evaluate_partial_delta_impl ( target_function_array_t const& f,
                                funct_arg_t t,
                                funct_args_t const& y,
                                funct_arg_t tau )
  {
    using namespace tuple_utils;

    auto const k0 = tau * f[Index] ( t, y );
    auto const k1 = tau * f[Index] ( t + tau / 2.0, y + k0 / 2.0 );
    auto const k2 = tau * f[Index] ( t + tau / 2.0, y + k1 / 2.0 );
    auto const k3 = tau * f[Index] ( t + tau, y + k2 );

    return  ( k0 + 2.0 * k1 + 2.0 * k2 + k3 ) / 6.0;
  }
};

template<size_t SYSTEM_RANK,
         typename FUNCT_ARG,
         typename ... ARGS>
struct diffsolve_traits<diffsolve_method::runge_kutta_felberga_7th, SYSTEM_RANK, FUNCT_ARG, ARGS...>
{
  using ret_type_t = FUNCT_ARG;
  using funct_arg_t = FUNCT_ARG;
  using funct_args_t = tuple_utils::funct_args_t<ARGS...>;
  using target_function_t = tuple_utils::target_nonstationary_function_t<funct_arg_t, ARGS...>;
  using target_function_array_t = tuple_utils::target_function_array_t<target_function_t, SYSTEM_RANK>;

  static funct_args_t
  evaluate_gradient ( target_function_array_t const& f,
                      funct_arg_t t,
                      funct_args_t const& y,
                      funct_arg_t tau )
  {
    return evaluate_gradient_impl ( f,
                                    t,
                                    y,
                                    tau, std::make_index_sequence<SYSTEM_RANK>() );
  }

private:
  template <size_t ... Indexes>
  static funct_args_t
  evaluate_gradient_impl ( target_function_array_t const& f,
                           funct_arg_t t,
                           funct_args_t const& y,
                           funct_arg_t tau,
                           std::index_sequence<Indexes...> )
  {
    funct_args_t result{};

    ( ( std::get<Indexes> ( result ) = evaluate_partial_delta_impl<Indexes> ( f, t, y, tau ) ), ... );

    return result;
  }

  template <size_t Index>
  static funct_arg_t
  evaluate_partial_delta_impl ( target_function_array_t const& f,
                                funct_arg_t t,
                                funct_args_t const& y,
                                funct_arg_t tau )
  {
    using namespace tuple_utils;

    auto const k1 = tau * f[Index] ( t, y );
    auto const k2 = tau * f[Index] ( t + ( 1.0 / 4.0 ) * tau, y + ( 1.0 / 4.0 ) * k1 );
    auto const k3 = tau * f[Index] ( t + ( 3.0 / 8.0 ) * tau, y + ( 3.0 / 32.0 ) * k1 + ( 9.0 / 32.0 ) * k2 );
    auto const k4 = tau * f[Index] ( t + ( 12.0 / 13.0 ) * tau,
                                     y + ( 1932.0 / 2197.0 ) * k1 - ( 7200.0 / 2197.0 ) * k2 + ( 7296.0 / 2197.0 ) * k3 );
    auto const k5 = tau * f[Index] ( t + tau, y + ( 439.0 / 216.0 ) * k1 - 8.0 * k2 + ( 3680.0 / 513.0 ) * k3 -
                                     ( 845.0 / 4104.0 ) * k4 );
    auto const k6 = tau * f[Index] ( t + ( 1.0 / 2.0 ) * tau,
                                     y - ( 8.0 / 27.0 ) * k1 + 2.0 * k2 - ( 3544.0 / 2565.0 ) * k3 + ( 1859.0 / 4104.0 ) * k4 -
                                     ( 11.0 / 40.0 ) * k5 );

    return  ( 16.0 / 135.0 ) * k1
            + ( 6656.0 / 12825.0 ) * k3
            + ( 28561.0 / 56430.0 ) * k4
            - ( 9.0 / 50.0 ) * k5
            + ( 2.0 / 55.0 ) * k6;
  }
};

} // namespace diffsolve_details

// taken from
// https://habr.com/en/post/418139/
template<diffsolve_method METHOD_ENUM,
         size_t SYSTEM_RANK,
         typename FUNCT_ARG,
         typename ... ARGS>
struct diffsolve
{
  static constexpr auto const system_rank = SYSTEM_RANK;

  using ret_type_t = FUNCT_ARG;
  using funct_arg_t = FUNCT_ARG;
  using funct_args_t = tuple_utils::funct_args_t<ARGS...>;

  using target_function_t = tuple_utils::target_nonstationary_function_t<funct_arg_t, ARGS...>;
  using target_function_array_t = tuple_utils::target_function_array_t<target_function_t, system_rank>;

  diffsolve ( funct_arg_t const& step,
              funct_args_t const& min_point,
              target_function_array_t const& funct )
    : step ( step )
    , min_point ( min_point )
    , funct ( funct )

  {
    static_assert ( std::is_arithmetic<funct_arg_t>::value, "FUNCT_ARG should be an arithmetic type" );
    static_assert ( ( std::is_arithmetic<ARGS>::value && ... ), "ARGS should be arithmetic types" );
  }

  funct_args_t evaluate_gradient ( funct_arg_t t,
                                   funct_args_t const& y ) const
  {
    constexpr auto const evaluate_gradient_funct_ptr =
      diffsolve_details::diffsolve_traits<METHOD_ENUM, SYSTEM_RANK, FUNCT_ARG, ARGS...>::evaluate_gradient;

    return  evaluate_gradient_funct_ptr ( funct, t, y, step );
  }

  funct_args_t from_too ( funct_arg_t const t0,
                          funct_arg_t const t1,
                          funct_args_t const& y0 ) const
  {
    return from_too_impl ( t0, t1, y0, std::make_index_sequence<sizeof... ( ARGS ) >() );
  }

private:

  template<size_t ... Indexes>
  funct_args_t from_too_impl ( funct_arg_t const t0,
                               funct_arg_t const t1,
                               funct_args_t const& y0,
                               std::index_sequence<Indexes...> ) const
  {
    auto result = y0;

    for ( auto t = t0; t < t1; t += step )
    {
      auto const delta = evaluate_gradient ( t, result );
      ( ( std::get<Indexes> ( result ) += std::get<Indexes> ( delta ) ), ... );
    }

    return result;
  }

private:
  funct_arg_t const step;
  funct_args_t const min_point;
  target_function_array_t const funct;
};

} // namespace diffsolve
