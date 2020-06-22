#pragma once

#include <utility>
#include <tuple>
#include <functional>

namespace noptim
{

namespace quick_descent_details
{


template<typename ... ARGS>
using funct_args_t = std::tuple<ARGS...>;

//template<typename T, typename ... ARGS>
//using funct_t = auto ( funct_args_t<ARGS...> const& )->T;

template<typename T, typename ... ARGS>
using funct_t = std::function<T ( funct_args_t<ARGS...> const& ) >;

}  // namespace quick_descent_details

template<typename T,
         typename ... ARGS>
struct quick_descent
{
  using funct_args_t = quick_descent_details::funct_args_t<ARGS... >;
  using funct_t = quick_descent_details::funct_t<T, ARGS... >;

  using funct_gradient_t = funct_args_t;

  static constexpr size_t const funct_args_count = std::tuple_size<funct_args_t>::value;

  quick_descent ( T const& step,
                  funct_args_t const& min_point,
                  funct_args_t const& max_point,
                  funct_args_t const& start_point,
                  funct_t funct )
    : step ( step )
    , min_point ( min_point )
    , max_point ( max_point )
    , start_point ( start_point )
    , funct ( funct )
  {
    /* DO NOTHING */
  }


  template<size_t ... Indexes>
  funct_args_t apply_step ( T step,
                            funct_args_t const& args,
                            std::integer_sequence<size_t, Indexes...>/* = std::make_index_sequence<funct_args_count>() */ )
  {
    funct_args_t result{};

    ( ( std::get<Indexes> ( result ) = std::get<Indexes> ( args ) + step ), ... );

    return result;
  }

  template<size_t ... Indexes>
  funct_gradient_t get_gradient ( funct_args_t const& args,
                                  std::integer_sequence<size_t, Indexes...>/* = std::make_index_sequence<funct_args_count>()*/ )
  {
    T f0 = funct ( args );

    constexpr auto my_dummy = [] ( auto ) {};

    funct_gradient_t result = { ( ( my_dummy ( Indexes ), f0 ), ... ) };

    //    T point1 = funct ( apply_step ( step, args ) );

    funct_args_t args1 {};
    ( ( args1 = args, std::get<Indexes> ( args1 ) += step, std::get<Indexes> ( result ) -= funct ( args1 ) ), ... );

    return result;
  }

  funct_args_t operator() ()
  {

  }

private:
  T const step;
  funct_args_t const min_point;
  funct_args_t const max_point;
  funct_args_t const start_point;
  funct_t funct;

};

} // namespace noptim
