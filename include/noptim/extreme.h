#pragma once

#include <type_traits>
#include <functional>

namespace noptim
{

enum class find_minimum_method
{
  dichotomie,
  gold_ratio
};


struct find_minimum_t
{
  size_t funct_invocation_count{};
};

namespace find_minimum_details
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


template <typename T>
using target_function_t = std::function<T ( T ) >;




template<typename T, find_minimum_method METHOD_ENUM>
struct find_minimum_traits;

template<typename T>
struct find_minimum_traits<T, find_minimum_method::dichotomie>
{
  static T method ( T const xa, T const xb,
                    T const eps, target_function_t<T> funct,
                    find_minimum_t* statistics )
  {

    auto x0 = xa;
    auto x1 = xb;

    auto x0i = ( x1 + x0 ) / find_minimum_details::middle_div<T>();;
    auto x1i = x1;

    auto f0i = funct ( x0i );
    auto f1i = f0i;

    if ( statistics )
    {
      statistics->funct_invocation_count++;
    }

    while ( x1 - x0 > eps )
    {
      if ( statistics )
      {
        statistics->funct_invocation_count++;
      }

      x1i = ( x1 + x0i ) / find_minimum_details::middle_div<T>();
      f1i = funct ( x1i );

      if ( f0i >= f1i )
      {
        x0 = x0i;

        x0i = x1i;
        f0i = f1i;
      }
      else
      {
        if ( statistics )
        {
          statistics->funct_invocation_count++;
        }

        auto const x2i = ( x0 + x0i ) / find_minimum_details::middle_div<T>();
        auto const f2i = funct ( x2i );

        if ( f2i <= f0i )
        {
          x1 = x1i;

          x0i = x2i;
          f0i = f2i;
        }
        else // if ( f2i >= f0i )
        {
          x1 = x1i;

          x0 = x2i;

          // x01 = x01;
          // f0i = f0i;
        }
      }
    }

    return ( x0 + x1 ) / find_minimum_details::middle_div<T>();
  }
};

template<typename T>
struct find_minimum_traits<T, find_minimum_method::gold_ratio>
{
  // taken from:
  // https://math.semestr.ru/optim/golden.php
  static T method ( T const xa, T const xb,
                    T const eps, target_function_t<T> funct,
                    find_minimum_t* statistics )
  {

    auto x0 = xa;
    auto x1 = xb;

    auto x0i = x0 + find_minimum_details::tau_compliment * ( x1 - x0 );
    auto x1i = x0 + find_minimum_details::tau * ( x1 - x0 );

    auto f0i = funct ( x0i );
    auto f1i = funct ( x1i );

    if ( statistics )
    {
      statistics->funct_invocation_count += 2;
    }

    while ( x1 - x0 > eps )
    {
      if ( f0i <= f1i )
      {
        // x0 = x0;
        x1 = x1i;

        x1i = x0i;
        x0i = x0 + find_minimum_details::tau_compliment * ( x1 - x0 );

        f1i = f0i;
        f0i = funct ( x0i );

        if ( statistics )
        {
          statistics->funct_invocation_count++;
        }
      }
      else if ( f0i > f1i )
      {
        x0 = x0i;
        // x1 = x1;

        x0i = x1i;
        x1i = x0 + find_minimum_details::tau * ( x1 - x0 );

        f0i = f1i;
        f1i = funct ( x1i );

        if ( statistics )
        {
          statistics->funct_invocation_count++;
        }
      }
    }

    return ( x0 + x1 ) / find_minimum_details::middle_div<T>();
  }

};

}  // namespace find_minimum_details

template <find_minimum_method METHOD_ENUM,
          typename T,
          typename LOSS_FUNCTION_T = find_minimum_details::target_function_t<T>>
T find_minimum ( T const xa, T const xb,
                 T const eps,
                 LOSS_FUNCTION_T& funct,
                 find_minimum_t* statistics )
{
  return find_minimum_details::find_minimum_traits<T, METHOD_ENUM>::method ( xa, xb, eps, funct, statistics );
}

}  // namespace noptim
