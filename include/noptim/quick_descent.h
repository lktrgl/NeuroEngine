#pragma once

#include <utility>
#include <tuple>
#include <functional>
#include <type_traits>

namespace noptim
{

namespace quick_descent_details
{


template<typename ... ARGS>
using funct_args_t = std::tuple<ARGS...>;

template<typename RET_TYPE, typename ... ARGS>
using funct_t = std::function<RET_TYPE ( funct_args_t<ARGS...> const& ) >;

}  // namespace quick_descent_details

template<typename RET_TYPE,
         typename ... ARGS>
struct quick_descent
{
  using funct_ret_t = RET_TYPE;
  using funct_arg_t = RET_TYPE;
  using funct_args_t = quick_descent_details::funct_args_t<ARGS... >;
  using funct_t = quick_descent_details::funct_t<funct_ret_t, ARGS... >;

  using funct_gradient_t = funct_args_t;

  static constexpr size_t const funct_args_count = std::tuple_size<funct_args_t>::value;

  quick_descent ( funct_arg_t const& step,
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
    static_assert ( std::is_arithmetic<funct_ret_t>::value, "RET_TYPE should be an arithmetic type" );
    static_assert ( std::is_arithmetic<funct_arg_t>::value, "RET_TYPE should be an arithmetic type" );
    static_assert ( ( std::is_arithmetic<ARGS>::value && ... ), "ARGS should be arithmetic types" );
  }

  funct_gradient_t get_gradient ( funct_args_t const& args )
  {
    return get_gradient_impl ( args, std::make_index_sequence<quick_descent::funct_args_count>() );
  }

private:
  template<size_t ... Indexes>
  funct_gradient_t get_gradient_impl ( funct_args_t const& args,
                                       std::integer_sequence<size_t, Indexes...> )
  {
    RET_TYPE const f0 = funct ( args );

    constexpr auto my_dummy = [] ( auto ) {};

    funct_gradient_t result = { ( ( my_dummy ( Indexes ), f0 ), ... ) };

    ( ( std::get<Indexes> ( result ) = funct ( apply_step<Indexes> ( step, args ) ) - std::get<Indexes> ( result ) ), ... );

    return result;
  }

  template<size_t Index>
  funct_args_t apply_step ( funct_arg_t step,
                            funct_args_t const& args )
  {
    funct_args_t result{args};

    std::get<Index> ( result ) += step;

    return result;
  }

  template<size_t ... Indexes>
  funct_args_t apply_step ( funct_arg_t step,
                            funct_args_t const& args,
                            std::integer_sequence<size_t, Indexes...> )
  {
    funct_args_t result{};

    ( ( std::get<Indexes> ( result ) = std::get<Indexes> ( args ) + step ), ... );

    return result;
  }

private:
  funct_arg_t const step;
  funct_args_t const min_point;
  funct_args_t const max_point;
  funct_args_t const start_point;
  funct_t funct;
};

} // namespace noptim
