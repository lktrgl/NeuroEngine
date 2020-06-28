#pragma once

#include <utils/tuple_utils.h>

#include <type_traits>
#include <functional>

namespace integral
{

enum class integral_method
{
  rectangle,
  trapezoid,
  simpson
};

namespace integral_details
{

template<typename T>
constexpr auto middle_div()
{
  if constexpr ( std::is_floating_point<T>::value )
  {
    return 2.0;
  }
  else
  {
    return 2;
  }
}

template<typename RET_TYPE, typename ARG_TYPE>
using target_funct_t = std::function<RET_TYPE ( ARG_TYPE ) >;

template<typename T>
constexpr bool always_false()
{
  return false;
}

template<integral_method METOD_ENUM,
         typename RET_TYPE,
         typename ARG_TYPE,
         typename TARGET_FUNCT>
struct integral_traits
{
  static RET_TYPE method ( ARG_TYPE, ARG_TYPE, ARG_TYPE, TARGET_FUNCT )
  {
    static_assert ( always_false<RET_TYPE>(), "Implement specialization for METOD_ENUM instead" );
    return {};
  }
};

template<typename RET_TYPE,
         typename ARG_TYPE>
struct integral_traits<integral_method::rectangle,
         RET_TYPE,
         ARG_TYPE,
         target_funct_t<RET_TYPE, ARG_TYPE>>
{
  static RET_TYPE method ( ARG_TYPE const from, ARG_TYPE const to, ARG_TYPE const step,
                           target_funct_t<RET_TYPE, ARG_TYPE> funct )
  {
    auto const fa = funct ( from );
    auto const fb = funct ( to );

    RET_TYPE result{};

    for ( auto t = from + step; t < to; t += step )
    {
      result += funct ( t );
    }

    result = result + fa + fb;

    result *= step;

    return result;
  }
};

template<typename RET_TYPE,
         typename ARG_TYPE>
struct integral_traits<integral_method::trapezoid,
         RET_TYPE,
         ARG_TYPE,
         target_funct_t<RET_TYPE, ARG_TYPE>>
{
  static RET_TYPE method ( ARG_TYPE const from, ARG_TYPE const to, ARG_TYPE const step,
                           target_funct_t<RET_TYPE, ARG_TYPE> funct )
  {
    auto const fa = funct ( from );
    auto const fb = funct ( to );

    RET_TYPE result{};

    for ( auto t = from + step; t < to; t += step )
    {
      result += funct ( t );
    }

    result = result + ( fa + fb ) / integral_details::middle_div<RET_TYPE>();

    result *= step;

    return result;
  }
};

}  // namespace integral_details

template<integral_method METOD_ENUM,
         typename RET_TYPE,
         typename ARG_TYPE>
struct integral
{
  using ret_type_t = RET_TYPE;
  using funct_arg_t = ARG_TYPE;
  using target_funct_t = integral_details::target_funct_t<ret_type_t, funct_arg_t>;

  integral ( funct_arg_t step, target_funct_t funct )
    : step ( step )
    , funct ( funct )
  {
    static_assert ( std::is_arithmetic<RET_TYPE>::value, "RET_TYPE should have an arithmetic type" );
    static_assert ( std::is_arithmetic<ARG_TYPE>::value, "ARG_TYPE should have an arithmetic type" );
  }

  RET_TYPE from_to ( funct_arg_t const& from, funct_arg_t const& to )
  {
    auto const method_ptr =
      integral_details::integral_traits<METOD_ENUM, RET_TYPE, ARG_TYPE, target_funct_t>::method;

    return method_ptr ( from, to, step, funct );
  }

private:
  funct_arg_t const step;
  target_funct_t const funct;
};

}  // namespace integral

