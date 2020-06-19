#pragma once

#include <type_traits>
#include <functional>

namespace noptim
{

namespace details
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

constexpr double const tau = 0.618;
constexpr double const tau_compliment = 1.0 - tau;


}  // namespace details

template <typename T>
using loss_function_t = auto ( T )->T;

struct find_minimum_t
{
  size_t loop_count{};
};

template <typename T, typename LOSS_FUNCTION_T = loss_function_t<T>>
T find_minimum_dichotomie ( T const xa, T const xb, T const eps, LOSS_FUNCTION_T& funct,
                            find_minimum_t* statistics )
{

  auto x0 = xa;
  auto f0 = funct ( x0 );
  auto x1 = xb;

  while ( x1 - x0 > eps )
  {
    if ( statistics )
    {
      statistics->loop_count++;
    }

    auto const x2 = ( x1 + x0 ) / details::middle_div<T>();
    auto const f2 = funct ( x2 );

    if ( f0 > f2 )
    {
      x0 = x2;
      f0 = f2;
    }
    else if ( f0 < f2 )
    {
      x0 = ( xa + x2 ) / details::middle_div<T>();
      f0 = funct ( x0 );
      x1 = x2;
    }
    else
    {
      return x2;
    }
  }

  return ( x0 + x1 ) / details::middle_div<T>();
}

template <typename T, typename LOSS_FUNCTION_T = loss_function_t<T>>
T find_minimum_gold_ratio ( T const xa, T const xb, T const eps, LOSS_FUNCTION_T& funct,
                            find_minimum_t* statistics )
{

  auto x0 = xa;
  auto x1 = xb;

  auto x0i = x0 + details::tau_compliment * ( x1 - x0 );
  auto x1i = x0 + details::tau * ( x1 - x0 );

  auto f0i = funct ( x0i );
  auto f1i = funct ( x1i );

  while ( x1 - x0 > eps )
  {
    if ( statistics )
    {
      statistics->loop_count++;
    }

    if ( f0i <= f1i )
    {
      // x0 = x0;
      x1 = x1i;

      x1i = x0i;
      x0i = x0 + details::tau_compliment * ( x1 - x0 );

      f1i = f0i;
      f0i = funct ( x0i );
    }
    else if ( f0i > f1i )
    {
      x0 = x0i;
      // x1 = x1;

      x0i = x1i;
      x1i = x0 + details::tau * ( x1 - x0 );

      f0i=f1i;
      f1i = funct ( x1i );
    }
  }

  return ( x0 + x1 ) / details::middle_div<T>();
}

}  // namespace noptim
