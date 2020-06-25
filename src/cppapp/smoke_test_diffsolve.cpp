#include <cppapp/smoke_test_diffsolve.h>

#include <diffsolve/diffsolve.h>

#include <cassert>
#include <cmath>

void smoke_test_runge_kutta()
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

  auto my_f = [] ( [[maybe_unused]]my_funct_arg_t t, [[maybe_unused]]my_funct_args_t const & x )->my_funct_ret_t
  {
    return 1.0;
  };

  my_target_function_array_t const funct =
  {
    my_f
  };

  my_funct_arg_t const h = 0.001;
  my_funct_arg_t const eps = 0.001;
  my_funct_args_t const min_point = { 0.0 };

  my_diffsolve_t ds ( h,
                      eps,
                      min_point,
                      funct );

  my_funct_arg_t const t = 0.0;
  my_funct_args_t const y{0.0};
  my_funct_args_t const delta_value =  ds.evaluate_delta ( t, y );

  my_funct_args_t const expected_delta_value = { h* funct[0] ( t, y ) };

  assert ( fabs ( diffsolve::get_normus<my_funct_ret_t> ( delta_value, expected_delta_value ) ) < eps );
}

void smoke_test_runge_kutta_felberga()
{


}
