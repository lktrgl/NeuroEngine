#pragma once

#include <type_traits>
#include <functional>

namespace noptim
{

template <typename T>
using loss_function_t = auto ( T )->T;

template <typename T, typename LOSS_FUNCTION_T = loss_function_t<T>>
T find_minimum_dichotomie ( T const xa, T const xb, T const eps, LOSS_FUNCTION_T& funct )
{
  constexpr auto const middle_div = []()
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
  ();

  auto x0 = xa;
  auto f0 = funct ( x0 );
  auto x1 = xb;

  while ( x1 - x0 > eps )
  {
    auto const x2 = ( x1 + x0 ) / middle_div;
    auto const f2 = funct ( x2 );

    if ( f0 > f2 )
    {
      x0 = x2;
      f0 = f2;
    }
    else if ( f0 < f2 )
    {
      x0 = ( xa + x2 ) / middle_div;
      f0 = funct ( x0 );
      x1 = x2;
    }
    else
    {
      return x2;
    }
  }

  return ( x0 + x1 ) / middle_div;
}

}  // namespace noptim
