#include <cppapp/smoke_test_diffsolve.h>

#include <diffsolve/diffsolve.h>

#include <cassert>
#include <cmath>

void smoke_test_euler()
{
  // test the rank = 1
  {
    constexpr size_t const system_rank = 1;
    using my_diffsolve_t = diffsolve::diffsolve <
                           diffsolve::diffsolve_method::euler,
                           system_rank,
                           double,
                           double >;

    using my_funct_ret_t = my_diffsolve_t::ret_type_t;
    using my_funct_arg_t = my_diffsolve_t::funct_arg_t;
    using my_funct_args_t = my_diffsolve_t::funct_args_t;
    using my_target_function_array_t = my_diffsolve_t::target_function_array_t;

    //
    // x(t) = A * exp(a*t) => A := x(0) := y0
    //
    // x'(t) = a * x(t)
    //
    // t E [t0,t1]
    //
    my_funct_arg_t const t0 = 0.0;
    my_funct_arg_t const t1 = 2.0;
    constexpr auto const a = -3.0;
    constexpr auto const A = 100.0;
    my_funct_args_t const y0{A};

    auto my_f = [] ( [[maybe_unused]]my_funct_arg_t t, [[maybe_unused]]my_funct_args_t const & x )->my_funct_ret_t
    {
      return {a* std::get<0> ( x ) };
    };

    my_target_function_array_t const funct =
    {
      my_f
    };

    my_funct_arg_t const h = 0.00001;
    my_funct_arg_t const eps = 0.0001;
    my_funct_args_t const min_point = { 0.0 };

    my_diffsolve_t ds ( h,
                        min_point,
                        funct );

    my_funct_args_t const delta_value =  ds.evaluate_delta ( t0, y0 );

    my_funct_args_t const expected_delta_value = { h* funct[0] ( t0, y0 ) };

    assert ( fabs ( diffsolve::get_normus<my_funct_ret_t> ( delta_value, expected_delta_value ) ) < eps );

    my_funct_args_t const end_value = ds.integrate_from_too ( t0, t1, y0 );

    my_funct_args_t const expected_end_value = { A * exp ( a * t1 ) };

    assert ( fabs ( diffsolve::get_normus<my_funct_ret_t> ( end_value, expected_end_value ) ) < eps );
}

void smoke_test_runge_kutta_felberga()
{


}
