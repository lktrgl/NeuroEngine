#include <cppapp/smoke_test_find_minimum.h>

#include <noptim/extreme.h>
#include <noptim/quick_descent.h>

#include <utils/tuple_utils.h>

#include <cassert>
#include <cmath>

namespace
{

struct test_function_parabola_t
{
  double operator() ( double x )
  {
    return A * ( x - root_x ) * ( x - root_x ) + d;
  }

  template<typename ... ARGS>
  double operator() ( std::tuple<ARGS...> const& x )
  {
    return A * ( std::get<0> ( x ) - root_x ) * ( std::get<0> ( x ) - root_x )
           + B * ( std::get<1> ( x ) - root_y ) * ( std::get<1> ( x ) - root_y )
           + d;
  };

  static constexpr auto  A = 3.0;
  static constexpr auto  B = 4.0;
  static constexpr auto  d = 10.0;
  static constexpr auto root_x = 1.0;
  static constexpr auto root_y = 1.0;

  static constexpr auto const xa = -1.0;
  static constexpr auto const xb = 2.0;
  static constexpr auto const ya = 0.0;
  static constexpr auto const yb = 6.0;

  static constexpr auto const h = 0.01;
  static constexpr auto const eps = 0.01;

  static constexpr auto const expected_x_min = test_function_parabola_t::root_x;
  static constexpr auto const expected_y_min = test_function_parabola_t::root_y;
};

struct test_function_qubic_t
{
  double operator() ( double x )
  {
    return - ( d * d * x - x * x * x );
  }

  template<typename ... ARGS>
  double operator() ( std::tuple<ARGS...> const& x )
  {
    return - ( d * d * std::get<0> ( x ) - std::get<0> ( x ) * std::get<0> ( x ) * std::get<0> ( x ) );
  };

  static constexpr auto const d = 10.0;

  static constexpr auto const xa = 0.;
  static constexpr auto const xb = test_function_qubic_t::d;
  static constexpr auto const h = 0.01;
  static constexpr auto const eps = 0.01;

  static constexpr auto const expected_x_min = test_function_qubic_t::d / sqrt ( 3.0 );
};


template<noptim::find_minimum_method METHOD_ENUM>
void smoke_test_find_minimum_X ( size_t const iter_count1, size_t const iter_count2 )
{
  {
    test_function_parabola_t my_f;

    noptim::find_minimum_t stat;
    auto const x_min =
      noptim::find_minimum<METHOD_ENUM> ( my_f.xa, my_f.xb, my_f.eps, my_f, &stat );

    assert ( fabs ( x_min - my_f.expected_x_min ) <= my_f.eps );
    assert ( iter_count1 == stat.funct_invocation_count );
  }

  {
    test_function_qubic_t my_f;

    noptim::find_minimum_t stat;
    auto const x_min =
      noptim::find_minimum<METHOD_ENUM> ( my_f.xa, my_f.xb, my_f.eps, my_f, &stat );

    assert ( fabs ( x_min - my_f.expected_x_min ) <= my_f.eps );
    assert ( iter_count2 == stat.funct_invocation_count );
  }
}

template<noptim::find_minimum_method METHOD_ENUM>
void smoke_test_quick_descent_single_argument ( size_t const iter_count )
{
  using my_quick_descent_t = noptim::quick_descent<METHOD_ENUM, double, double>;

  using my_funct_ret_t = typename my_quick_descent_t::funct_ret_t;
  using my_funct_args_t = typename my_quick_descent_t::funct_args_t;
  using my_funct_gradient_t = typename my_quick_descent_t::funct_gradient_t;

  test_function_qubic_t my_f;

  my_funct_args_t const min_point = {my_f.xa};
  my_funct_args_t const max_point = {my_f.xb};

  my_funct_args_t const step_point = {my_f.xa + my_f.h};


  my_funct_args_t const expected_x_min = { my_f.expected_x_min };

  my_quick_descent_t qd ( my_f.h, my_f.eps,
                          min_point, max_point,
                          my_f );

  my_funct_gradient_t const gr0 = qd.get_gradient ( min_point );
  my_funct_gradient_t const expected_gr0 = my_f ( step_point ) - my_f ( min_point );

  assert ( fabs ( tuple_utils::get_normus<my_funct_ret_t> ( gr0, expected_gr0 ) ) <= my_f.eps );

  noptim::find_minimum_t stat;
  my_funct_args_t const x_min = qd.find_minimum ( &stat );

  assert ( fabs ( tuple_utils::get_normus<my_funct_ret_t> ( x_min, expected_x_min ) ) <= my_f.eps );
  assert ( iter_count == stat.funct_invocation_count );
}

template<noptim::find_minimum_method METHOD_ENUM>
void smoke_test_quick_descent_two_argument ( size_t const iter_count )
{
  using my_quick_descent_t = noptim::quick_descent<METHOD_ENUM, double, double, double>;

  using my_funct_ret_t = typename my_quick_descent_t::funct_ret_t;
  using my_funct_args_t = typename my_quick_descent_t::funct_args_t;
  using my_funct_gradient_t = typename my_quick_descent_t::funct_gradient_t;

  test_function_parabola_t my_f;

  my_funct_args_t const min_point = {my_f.xa, my_f.ya};
  my_funct_args_t const max_point = {my_f.xb, my_f.yb};

  my_funct_args_t const step_point_x = {my_f.xa + my_f.h, my_f.ya};
  my_funct_args_t const step_point_y = {my_f.xa, my_f.ya + my_f.h};


  my_funct_args_t const expected_x_min = {my_f.expected_x_min, my_f.expected_y_min};

  my_quick_descent_t qd ( my_f.h, my_f.eps,
                          min_point, max_point,
                          my_f );

  my_funct_gradient_t const gr0 = qd.get_gradient ( min_point );
  my_funct_gradient_t const expected_gr0 =
  {
    my_f ( step_point_x ) - my_f ( min_point ),
    my_f ( step_point_y ) - my_f ( min_point )
  };

  assert ( fabs ( tuple_utils::get_normus<my_funct_ret_t> ( gr0, expected_gr0 ) ) <= my_f.eps );

  noptim::find_minimum_t stat;
  my_funct_args_t const x_min = qd.find_minimum ( &stat );

  assert ( fabs ( tuple_utils::get_normus<my_funct_ret_t> ( x_min, expected_x_min ) ) <= my_f.eps );
  assert ( iter_count == stat.funct_invocation_count );
}

} // namespace anonymous

void smoke_test_find_minimum_dichotomie()
{
  smoke_test_find_minimum_X<noptim::find_minimum_method::dichotomie> ( 21, 23 );
}

void smoke_test_find_minimum_gold_ratio()
{
  smoke_test_find_minimum_X<noptim::find_minimum_method::gold_ratio> ( 14, 17 );
}

void smoke_test_quick_descent_dichotomie()
{
  // test for the single argument function
  smoke_test_quick_descent_single_argument<noptim::find_minimum_method::dichotomie> ( 23 );

  // test for the two argument function
  smoke_test_quick_descent_two_argument<noptim::find_minimum_method::dichotomie> ( 44 );
}

void smoke_test_quick_descent_gold_ratio()
{
  // test for the single argument function
  smoke_test_quick_descent_single_argument<noptim::find_minimum_method::gold_ratio> ( 17 );

  // test for the two argument function
  smoke_test_quick_descent_two_argument<noptim::find_minimum_method::gold_ratio> ( 30 );
}
