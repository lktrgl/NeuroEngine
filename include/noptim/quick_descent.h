#pragma once

#include <noptim/extreme.h>

#include <utility>
#include <tuple>
#include <functional>
#include <type_traits>
#include <cmath>

namespace noptim
{

namespace quick_descent_details
{


template<typename ... ARGS>
using funct_args_t = std::tuple<ARGS...>;

template<typename RET_TYPE, typename ... ARGS>
using target_function_t = std::function<RET_TYPE ( funct_args_t<ARGS...> const& ) >;

template<typename T, size_t ... Indexes>
T operator_minus_impl ( T const& a, T const& b, std::index_sequence<Indexes...> )
{
  auto result{a};

  ( ( std::get<Indexes> ( result ) -= std::get<Indexes> ( b ) ), ... );

  return result;
}

template<typename RET_TYPE, typename T, size_t ... Indexes>
RET_TYPE get_normus_impl ( T const& a, std::index_sequence<Indexes...> )
{
  RET_TYPE result{};

  ( ( result += std::get<Indexes> ( a ) * std::get<Indexes> ( a ) ), ... );

  return sqrt ( result );
}

}  // namespace quick_descent_details

template<find_minimum_method METHOD_ENUM,
         typename RET_TYPE,
         typename ... ARGS>
struct quick_descent
{
  static constexpr auto find_minimum_method = METHOD_ENUM;
  using funct_ret_t = RET_TYPE;
  using funct_arg_t = RET_TYPE;
  using funct_args_t = quick_descent_details::funct_args_t<ARGS... >;
  using target_function_t = quick_descent_details::target_function_t<funct_ret_t, ARGS... >;

  using funct_gradient_t = funct_args_t;

  static constexpr size_t const funct_args_count = std::tuple_size<funct_args_t>::value;

  quick_descent ( funct_arg_t const& step,
                  funct_arg_t const& eps,
                  funct_args_t const& min_point,
                  funct_args_t const& max_point,
                  target_function_t funct )
    : step ( step )
    , eps ( eps )
    , min_point ( min_point )
    , max_point ( max_point )
    , funct ( funct )
  {
    static_assert ( std::is_arithmetic<funct_ret_t>::value, "RET_TYPE should be an arithmetic type" );
    static_assert ( std::is_arithmetic<funct_arg_t>::value, "RET_TYPE should be an arithmetic type" );
    static_assert ( ( std::is_arithmetic<ARGS>::value && ... ), "ARGS should be arithmetic types" );
  }

  funct_gradient_t get_gradient ( funct_args_t const& args ) const
  {
    return get_gradient_impl ( args, std::make_index_sequence<quick_descent::funct_args_count>() );
  }

  funct_args_t find_minimum ( noptim::find_minimum_t* statistics = nullptr ) const
  {
    return find_minimum_impl ( statistics, std::make_index_sequence<quick_descent::funct_args_count>() );
  }

private:
  template<size_t Index>
  funct_args_t find_partial_minimum ( noptim::find_minimum_t* statistics, funct_args_t const& args ) const
  {
    funct_args_t work_point{args};

    auto partial_function = [&work_point, this] ( funct_arg_t x )->funct_ret_t
    {
      std::get<Index> ( work_point ) = x;
      return funct ( work_point );
    };

    std::get<Index> ( work_point ) = noptim::find_minimum<find_minimum_method> (
                                       std::get<Index> ( min_point ),
                                       std::get<Index> ( max_point ), eps,
                                       partial_function,
                                       statistics );

    return work_point;
  }

  template<size_t ... Indexes>
  funct_args_t find_minimum_impl ( noptim::find_minimum_t* statistics, std::integer_sequence<size_t, Indexes...> ) const
  {
    funct_args_t work_point{min_point};

    ( ( work_point = find_partial_minimum<Indexes> ( statistics, work_point ) ), ... );

    return work_point;
  }

  template<size_t ... Indexes>
  funct_gradient_t get_gradient_impl ( funct_args_t const& args,
                                       std::integer_sequence<size_t, Indexes...> ) const
  {
    RET_TYPE const f0 = funct ( args );

    funct_gradient_t result{};

    ( ( std::get<Indexes> ( result ) = funct ( apply_step<Indexes> ( step, args ) ) - f0 ), ... );

    return result;
  }

  template<size_t Index>
  funct_args_t apply_step ( funct_arg_t step,
                            funct_args_t const& args ) const
  {
    funct_args_t result{args};

    std::get<Index> ( result ) += step;

    return result;
  }

  template<size_t ... Indexes>
  funct_args_t apply_step ( funct_arg_t step,
                            funct_args_t const& args,
                            std::integer_sequence<size_t, Indexes...> ) const
  {
    funct_args_t result{};

    ( ( std::get<Indexes> ( result ) = std::get<Indexes> ( args ) + step ), ... );

    return result;
  }

private:
  funct_arg_t const step;
  funct_arg_t const eps;
  funct_args_t const min_point;
  funct_args_t const max_point;
  target_function_t funct;
};


template<typename RET_TYPE, typename ... ARGS>
RET_TYPE get_normus ( std::tuple<ARGS...> const& a, std::tuple<ARGS...> const& b )
{
  auto const delta = quick_descent_details::operator_minus_impl ( a, b,
                     std::make_index_sequence<sizeof... ( ARGS ) >() );

  return quick_descent_details::get_normus_impl<RET_TYPE> ( delta,
         std::make_index_sequence<sizeof... ( ARGS ) >() );
}


} // namespace noptim
