#include <cppapp/smoke_test_integral.h>

#include <integ/integral.h>

#include <cassert>
#include <cmath>

namespace
{

template<integral::integral_method METHOD_ENUM,
         typename RET_TYPE,
         typename ARG_TYPE>
void smoke_test_X ( ARG_TYPE const h_in, ARG_TYPE const eps_in )
{
  using my_integral = integral::integral<METHOD_ENUM, RET_TYPE, ARG_TYPE>;
  using my_ret_type_t = typename my_integral::ret_type_t;
  using my_funct_arg_t = typename my_integral::funct_arg_t;

  constexpr my_funct_arg_t const A = 3.0;

  auto my_funct = [] ( double const & t )
  {
    return A * cos ( t );
  };

  my_funct_arg_t const h = h_in;
  constexpr my_funct_arg_t const a = 0.0;
  constexpr my_funct_arg_t const b = M_PI / 2.0;
  my_funct_arg_t const eps = eps_in;

  constexpr my_ret_type_t const expected_integral_value = A * sin ( b );

  my_integral integr ( h, my_funct );

  my_ret_type_t const integral_value = integr.from_to ( a, b );

  assert ( fabs ( integral_value - expected_integral_value ) < eps );
}

void smoke_test_rectangle()
{
  smoke_test_X<integral::integral_method::rectangle, double, double> ( 0.0006, 0.001 );
}

void smoke_test_trapezoid()
{
  smoke_test_X<integral::integral_method::trapezoid, double, double> ( 0.1, 0.001 );
}

} // namespace anonymous

void test_integral()
{
  smoke_test_rectangle();

  smoke_test_trapezoid();
}
