#include <cppapp/smoke_test_diffsolve.h>

#include <diffsolve/diffsolve.h>

#include <utils/tuple_utils.h>
#include <utils/target_functions.h>

#include <cassert>
#include <cmath>

namespace
{

constexpr auto const g_eps = 0.001;

template<diffsolve::diffsolve_method METHOD_ENUM, typename ARG_TYPE>
void smoke_test_X_rank_1 ( ARG_TYPE h_in, ARG_TYPE eps_in )
{
  constexpr size_t const system_rank = 1;
  using my_diffsolve_t = diffsolve::diffsolve <
                         METHOD_ENUM,
                         system_rank,
                         ARG_TYPE,
                         ARG_TYPE >;

  using my_funct_ret_t = typename my_diffsolve_t::ret_type_t;
  using my_funct_arg_t = typename my_diffsolve_t::funct_arg_t;
  using my_funct_args_t = typename my_diffsolve_t::funct_args_t;
  using my_target_function_array_t = typename my_diffsolve_t::target_function_array_t;

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
    return a * std::get<0> ( x );
  };

  my_target_function_array_t const funct =
  {
    my_f
  };

  my_funct_arg_t const h = h_in;
  my_funct_arg_t const eps = eps_in;

  my_diffsolve_t ds ( h,
                      y0,
                      funct );

  my_funct_args_t const gradient_value =  ds.evaluate_gradient ( t0, y0 );

  my_funct_args_t const expected_gradient_value = { h* funct[0] ( t0, y0 ) };

  assert ( fabs ( tuple_utils::get_normus<my_funct_ret_t> ( gradient_value, expected_gradient_value ) ) < eps );

  my_funct_args_t const end_value = ds.from_too ( t0, t1, y0 );

  my_funct_args_t const expected_end_value = { A * exp ( a * t1 ) };

  assert ( fabs ( tuple_utils::get_normus<my_funct_ret_t> ( end_value, expected_end_value ) ) < eps );
}

template<diffsolve::diffsolve_method METHOD_ENUM, typename ARG_TYPE>
void smoke_test_conservative_X_2 ( ARG_TYPE h_in, ARG_TYPE eps_in )
{
  constexpr size_t const system_rank = 2;
  using my_diffsolve_t = diffsolve::diffsolve <
                         METHOD_ENUM,
                         system_rank,
                         ARG_TYPE,
                         ARG_TYPE, ARG_TYPE >;

  using my_funct_ret_t = typename my_diffsolve_t::ret_type_t;
  using my_funct_arg_t = typename my_diffsolve_t::funct_arg_t;
  using my_funct_args_t = typename my_diffsolve_t::funct_args_t;
  using my_target_function_array_t = typename my_diffsolve_t::target_function_array_t;

  //
  // x(t) = A * sin(w*t+fi) => { fi := arctg( w*x(0) / x'(0)); A := x(0)/ sin(fi); }
  //
  // x1'(t) = x2(t)
  // x2'(t) = - w * x1(t)
  //
  // t E [t0,t1]
  //
  constexpr auto const t0 = 0.0;
  constexpr auto const t1 = 2.0;
  constexpr auto const w = 3.0;
  constexpr auto const A = 100.0;
  constexpr auto const fi = M_PI / 3.0;
  my_funct_args_t const y0{ A * sin ( fi ), w* A * cos ( fi ) };

  auto my_f1 = [] ( [[maybe_unused]]my_funct_arg_t t, [[maybe_unused]]my_funct_args_t const & x )->my_funct_ret_t
  {
    return std::get<1> ( x );
  };

  auto my_f2 = [] ( [[maybe_unused]]my_funct_arg_t t, [[maybe_unused]]my_funct_args_t const & x )->my_funct_ret_t
  {
    return -w * w * std::get<0> ( x );
  };

  my_target_function_array_t const funct =
  {
    my_f1,
    my_f2
  };

  my_funct_arg_t const h = h_in;
  my_funct_arg_t const eps = eps_in;

  my_diffsolve_t ds ( h,
                      y0,
                      funct );

  my_funct_args_t const gradient_value =  ds.evaluate_gradient ( t0, y0 );

  my_funct_args_t const expected_gradient_value =
  {
    h* funct[0] ( t0, y0 ),
    h* funct[1] ( t0, y0 )
  };

  assert ( fabs ( tuple_utils::get_normus<my_funct_ret_t> ( gradient_value, expected_gradient_value ) ) < eps );

  my_funct_args_t const end_value = ds.from_too ( t0, t1, y0 );

  my_funct_args_t const expected_end_value =
  {
    A * sin ( w * t1 + fi ),
    w* A * cos ( w * t1 + fi )
  };

  assert ( fabs ( tuple_utils::get_normus<my_funct_ret_t> ( end_value, expected_end_value ) ) < eps );
}


template<diffsolve::diffsolve_method METHOD_ENUM, typename ARG_TYPE>
void smoke_test_2nd_degree_system_X_2 ( ARG_TYPE h_in, ARG_TYPE eps_in )
{
  using my_diffsolve_t = diffsolve::diffsolve <
                         METHOD_ENUM,
                         target_function_utils::test_function_2nd_degree_system::system_rank,
                         ARG_TYPE,
                         ARG_TYPE, ARG_TYPE >;

  using my_funct_ret_t = typename my_diffsolve_t::ret_type_t;
  using my_funct_arg_t = typename my_diffsolve_t::funct_arg_t;
  using my_funct_args_t = typename my_diffsolve_t::funct_args_t;

  my_funct_arg_t const step = h_in;
  my_funct_arg_t const eps = eps_in;

  my_diffsolve_t ds ( step,
                      target_function_utils::test_function_2nd_degree_system::initial_state,
                      target_function_utils::test_function_2nd_degree_system::system_definition );

  my_funct_args_t const end_value = ds.from_too (
                                      target_function_utils::test_function_2nd_degree_system::t0,
                                      target_function_utils::test_function_2nd_degree_system::t1,
                                      target_function_utils::test_function_2nd_degree_system::initial_state );

  assert ( fabs ( tuple_utils::get_normus<my_funct_ret_t> (
                    end_value,
                    target_function_utils::test_function_2nd_degree_system::expected_end_value
                  )
                ) < eps
         );
}

void smoke_test_euler_rank_1()
{
  smoke_test_X_rank_1<diffsolve::diffsolve_method::euler> ( /*h_in*/0.0001, /*eps_in*/g_eps );
}

void smoke_test_euler_runk_2()
{
  smoke_test_conservative_X_2<diffsolve::diffsolve_method::euler> ( /*h_in*/0.0000001, /*eps_in*/g_eps );

  smoke_test_2nd_degree_system_X_2<diffsolve::diffsolve_method::euler> ( /*h_in*/0.0000001, /*eps_in*/g_eps );
}

void smoke_test_rk_4_rank_1()
{
  smoke_test_X_rank_1<diffsolve::diffsolve_method::runge_kutta_4th> ( /*h_in*/0.001, /*eps_in*/g_eps );
}

void smoke_test_rk_4_runk_2()
{
  smoke_test_conservative_X_2<diffsolve::diffsolve_method::runge_kutta_4th> ( /*h_in*/0.0000001, /*eps_in*/g_eps );

  smoke_test_2nd_degree_system_X_2<diffsolve::diffsolve_method::runge_kutta_4th> ( /*h_in*/0.0000001, /*eps_in*/g_eps );
}

void smoke_test_rkf_7_rank_1()
{
  smoke_test_X_rank_1<diffsolve::diffsolve_method::runge_kutta_felberga_7th> ( /*h_in*/0.001, /*eps_in*/g_eps );
}

void smoke_test_rkf_7_runk_2()
{
  smoke_test_conservative_X_2<diffsolve::diffsolve_method::runge_kutta_felberga_7th> ( /*h_in*/0.0000001, /*eps_in*/g_eps );

  smoke_test_2nd_degree_system_X_2<diffsolve::diffsolve_method::runge_kutta_felberga_7th> ( /*h_in*/0.0000001, /*eps_in*/g_eps );
}

void smoke_test_euler()
{
  // test the rank = 1
  smoke_test_euler_rank_1();

  // test the rank = 2
  smoke_test_euler_runk_2();
}

void smoke_test_runge_kutta_4th()
{
  // test the rank = 1
  smoke_test_rk_4_rank_1();

  // test the rank = 2
  smoke_test_rk_4_runk_2();
}

void smoke_test_runge_kutta_felberga_7th()
{
  // test the rank = 1
  smoke_test_rkf_7_rank_1();

  // test the rank = 2
  smoke_test_rkf_7_runk_2();
}

} // namespace anonymous

void test_diffsolve()
{
  smoke_test_euler();

  smoke_test_runge_kutta_4th();

  smoke_test_runge_kutta_felberga_7th();
}
