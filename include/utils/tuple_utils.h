#pragma once

#include <cstddef>
#include <type_traits>
#include <tuple>
#include <utility>
#include <functional>

#include <cmath>

namespace tuple_utils
{

namespace tuple_utils_details
{

template<typename TUPLE_TYPE,
         size_t ... Indexes>
TUPLE_TYPE operator_minus_impl ( TUPLE_TYPE const& a,
                                 TUPLE_TYPE const& b,
                                 std::index_sequence<Indexes...> )
{
  auto result{a};

  ( ( std::get<Indexes> ( result ) -= std::get<Indexes> ( b ) ), ... );

  return result;
}

template<typename TUPLE_TYPE,
         typename T,
         size_t ... Indexes>
TUPLE_TYPE operator_minus_impl ( TUPLE_TYPE const& a,
                                 T const& b,
                                 std::index_sequence<Indexes...>,
                                 [[maybe_unused]]std::enable_if_t<std::is_arithmetic<T>::value, int>* dummy = nullptr )
{
  auto result{a};

  ( ( std::get<Indexes> ( result ) -= b ), ... );

  return result;
}

template<typename TUPLE_TYPE,
         size_t ... Indexes>
TUPLE_TYPE operator_plus_impl ( TUPLE_TYPE const& a,
                                TUPLE_TYPE const& b,
                                std::index_sequence<Indexes...> )
{
  auto result{a};

  ( ( std::get<Indexes> ( result ) += std::get<Indexes> ( b ) ), ... );

  return result;
}

template<typename TUPLE_TYPE,
         typename T,
         size_t ... Indexes>
TUPLE_TYPE operator_plus_impl ( TUPLE_TYPE const& a,
                                T const& b,
                                std::index_sequence<Indexes...>,
                                [[maybe_unused]]std::enable_if_t<std::is_arithmetic<T>::value, int>* dummy = nullptr )
{
  auto result{a};

  ( ( std::get<Indexes> ( result ) += b ), ... );

  return result;
}

template<typename RET_TYPE,
         typename TUPLE_TYPE,
         size_t ... Indexes>
RET_TYPE get_normus_impl ( TUPLE_TYPE const& a, std::index_sequence<Indexes...> )
{
  RET_TYPE result{};

  ( ( result += std::get<Indexes> ( a ) * std::get<Indexes> ( a ) ), ... );

  return sqrt ( result );
}

}  // namespace tuple_utils_details


template<typename ... ARGS>
using funct_args_t = std::tuple<ARGS...>;

template<typename RET_TYPE, typename ... ARGS>
using target_function_t = std::function<RET_TYPE ( funct_args_t<ARGS...> const& ) >;

template <typename T, typename ... ARGS>
using target_nonstationary_function_t = std::function<T ( T, funct_args_t<ARGS...>const& ) >;

template<typename RET_TYPE,
         typename ... ARGS>
auto get_normus ( std::tuple<ARGS...> const& a, std::tuple<ARGS...> const& b )->RET_TYPE
{
  auto const delta = tuple_utils_details::operator_minus_impl ( a, b,
                     std::make_index_sequence<sizeof... ( ARGS ) >() );

  return tuple_utils_details::get_normus_impl<RET_TYPE> ( delta,
         std::make_index_sequence<sizeof... ( ARGS ) >() );
}

template<typename ARITHMETIC_TYPE,
         typename ... ARGS>
auto operator+ ( std::tuple<ARGS...> const& a, ARITHMETIC_TYPE b )->std::tuple<ARGS...>
{
  static_assert ( std::is_arithmetic<ARITHMETIC_TYPE>::value );

  return tuple_utils_details::operator_plus_impl ( a, b,
         std::make_index_sequence<sizeof... ( ARGS ) >() );
}

template<typename ARITHMETIC_TYPE,
         typename ... ARGS>
auto operator- ( std::tuple<ARGS...> const& a, ARITHMETIC_TYPE b )->std::tuple<ARGS...>
{
  static_assert ( std::is_arithmetic<ARITHMETIC_TYPE>::value );

  return tuple_utils_details::operator_minus_impl ( a, b,
         std::make_index_sequence<sizeof... ( ARGS ) >() );
}

}  // namespace tuple_utils

