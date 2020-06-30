#include <cppapp/smoke_test_find_minimum.h>

#include <noptim/extreme.h>

#include <utils/tuple_utils.h>
#include <utils/target_functions.h>

#include <cassert>
#include <cmath>

namespace
{

template<noptim::find_minimum_method METHOD_ENUM>
void smoke_test_find_minimum_X ( size_t const iter_count1, size_t const iter_count2 )
{
  {
    target_function_utils::test_function_parabola_t my_f;

    noptim::find_minimum_t stat;
    auto const x_min =
      noptim::find_minimum<METHOD_ENUM> ( my_f.xa, my_f.xb, my_f.eps, my_f, &stat );

    assert ( fabs ( x_min - my_f.expected_x_min ) <= my_f.eps );
    assert ( iter_count1 == stat.funct_invocation_count );
  }

  {
    target_function_utils::test_function_qubic_t my_f;

    noptim::find_minimum_t stat;
    auto const x_min =
      noptim::find_minimum<METHOD_ENUM> ( my_f.xa, my_f.xb, my_f.eps, my_f, &stat );

    assert ( fabs ( x_min - my_f.expected_x_min ) <= my_f.eps );
    assert ( iter_count2 == stat.funct_invocation_count );
  }
}

void smoke_test_find_minimum_dichotomie()
{
  smoke_test_find_minimum_X<noptim::find_minimum_method::dichotomie> ( 21, 23 );
}

void smoke_test_find_minimum_gold_ratio()
{
  smoke_test_find_minimum_X<noptim::find_minimum_method::gold_ratio> ( 14, 17 );
}

} // namespace anonymous

void test_find_minimum()
{
  smoke_test_find_minimum_dichotomie();

  smoke_test_find_minimum_gold_ratio();
}
