#include <cppapp/smoke_test_quick_descent.h>

#include <noptim/extreme.h>
#include <noptim/quick_descent.h>

#include <utils/target_functions.h>

#include <cassert>

namespace
{

template<noptim::find_minimum_method METHOD_ENUM>
void smoke_test_quick_descent_single_argument ( size_t const iter_count )
{
  using my_quick_descent_t = noptim::quick_descent<METHOD_ENUM, double, double>;

  using my_funct_ret_t = typename my_quick_descent_t::funct_ret_t;
  using my_funct_args_t = typename my_quick_descent_t::funct_args_t;
  using my_funct_gradient_t = typename my_quick_descent_t::funct_gradient_t;

  target_function_utils::test_function_qubic_t my_f;

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

  target_function_utils::test_function_parabola_t my_f;

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

} // namespace anonymous

void test_quick_descent()
{
  smoke_test_quick_descent_dichotomie();

  smoke_test_quick_descent_gold_ratio();
}
