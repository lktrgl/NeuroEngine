#include <cppapp/smoke_test_find_minimum.h>

#include <noptim/extreme.h>
#include <noptim/quick_descent.h>

#include <utils/tuple_utils.h>

#include <cassert>
#include <cmath>

namespace
{

// smoke test for the noptim::find_minimum_dichotomie<>(.)
template<noptim::find_minimum_method METHOD_ENUM>
void smoke_test_find_minimum_X ( size_t const iter_count1, size_t const iter_count2 )
{
  {
    auto my_f = [] ( double x )->double
    {
      return ( x - 1.0 ) * ( x - 1.0 );
    };

    auto const xa = -1.0;
    auto const xb = 2.0;
    auto const eps = 0.01;

    auto const expected_x_min = 1.0;

    noptim::find_minimum_t stat;
    auto const x_min =
      noptim::find_minimum<METHOD_ENUM> ( xa, xb, eps, my_f, &stat );

    assert ( fabs ( x_min - expected_x_min ) <= eps );
    assert ( iter_count1 == stat.funct_invocation_count );
  }

  {
    constexpr auto const d = 10.0;

    auto my_f = [] ( double x )->double
    {
      return - ( d* d * x - x* x * x );
    };

    auto const xa = 0.;
    auto const xb = d;
    auto const eps = 0.01;

    constexpr auto const expected_x_min = d / sqrt ( 3.0 );

    noptim::find_minimum_t stat;
    auto const x_min =
      noptim::find_minimum<METHOD_ENUM> ( xa, xb, eps, my_f, &stat );

    assert ( fabs ( x_min - expected_x_min ) <= eps );
    assert ( iter_count2 == stat.funct_invocation_count );
  }
}

} // namespace anonymous

// smoke test for the noptim::find_minimum_dichotomie<>(.)
void smoke_test_find_minimum_dichotomie()
{
  smoke_test_find_minimum_X<noptim::find_minimum_method::dichotomie> ( 21, 23 );
}

// smoke test for the noptim::find_minimum_gold_ratio<>(.)
void smoke_test_find_minimum_gold_ratio()
{
  smoke_test_find_minimum_X<noptim::find_minimum_method::gold_ratio> ( 14, 17 );
}

// smoke test for the noptim::quick_descent<> class
void smoke_test_quick_descent()
{
  // test for the single argument function
  {
    constexpr auto const d = 10.0;

    using my_quick_descent_t = noptim::quick_descent<noptim::find_minimum_method::gold_ratio, double, double>;

    using my_funct_ret_t = my_quick_descent_t::funct_ret_t;
    using my_funct_args_t = my_quick_descent_t::funct_args_t;
    using my_funct_gradient_t = my_quick_descent_t::funct_gradient_t;

    auto my_f = [] ( my_funct_args_t const & x )->my_funct_ret_t
    {
      return - ( d* d * std::get<0> ( x ) - std::get<0> ( x ) * std::get<0> ( x ) * std::get<0> ( x ) );
    };

    constexpr auto const h = 0.01;

    auto const xa = 0.0;
    auto const xb = d;
    auto const eps = 0.01;

    my_funct_args_t const min_point = {xa};
    my_funct_args_t const max_point = {xb};

    my_funct_args_t const step_point = {xa + h};


    my_funct_args_t const expected_x_min = { d / sqrt ( 3.0 ) };

    my_quick_descent_t qd ( h, eps,
                            min_point, max_point,
                            my_f );

    my_funct_gradient_t const gr0 = qd.get_gradient ( min_point );
    my_funct_gradient_t const expected_gr0 = my_f ( step_point ) - my_f ( min_point );

    assert ( fabs ( tuple_utils::get_normus<my_funct_ret_t> ( gr0, expected_gr0 ) ) <= eps );

    noptim::find_minimum_t stat;
    my_funct_args_t const x_min = qd.find_minimum ( &stat );

    assert ( fabs ( tuple_utils::get_normus<my_funct_ret_t> ( x_min, expected_x_min ) ) <= eps );
    assert ( 17 == stat.funct_invocation_count );
  }

  // test for the two argument function
  {
    using my_quick_descent_t = noptim::quick_descent<noptim::find_minimum_method::gold_ratio, double, double, double>;

    using my_funct_ret_t = my_quick_descent_t::funct_ret_t;
    using my_funct_args_t = my_quick_descent_t::funct_args_t;
    using my_funct_gradient_t = my_quick_descent_t::funct_gradient_t;

    constexpr auto const x0 = 2.0;
    constexpr auto const y0 = 5.0;

    auto my_f = [] ( my_funct_args_t const & x )->my_funct_ret_t
    {
      return ( ( std::get<0> ( x ) - x0 ) * ( std::get<0> ( x ) - x0 )
               + ( std::get<1> ( x ) - y0 ) * ( std::get<1> ( x ) - y0 )
               + 10.0 );
    };

    constexpr auto const h = 0.01;

    auto const xa = 0.0;
    auto const xb = 6.0;
    auto const ya = 0.0;
    auto const yb = 6.0;
    auto const eps = 0.01;

    my_funct_args_t const min_point = {xa, ya};
    my_funct_args_t const max_point = {xb, yb};

    my_funct_args_t const step_point_x = {xa + h, ya};
    my_funct_args_t const step_point_y = {xa, ya + h};


    my_funct_args_t const expected_x_min = { x0, y0};

    my_quick_descent_t qd ( h, eps,
                            min_point, max_point,
                            my_f );

    my_funct_gradient_t const gr0 = qd.get_gradient ( min_point );
    my_funct_gradient_t const expected_gr0 =
    {
      my_f ( step_point_x ) - my_f ( min_point ),
      my_f ( step_point_y ) - my_f ( min_point )
    };

    assert ( fabs ( tuple_utils::get_normus<my_funct_ret_t> ( gr0, expected_gr0 ) ) <= eps );

    noptim::find_minimum_t stat;
    my_funct_args_t const x_min = qd.find_minimum ( &stat );

    assert ( fabs ( tuple_utils::get_normus<my_funct_ret_t> ( x_min, expected_x_min ) ) <= eps );
    assert ( 32 == stat.funct_invocation_count );
  }
}
