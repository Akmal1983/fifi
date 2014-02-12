// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <fifi/binary4.hpp>
#include <fifi/binary8.hpp>
#include <fifi/binary8_full_table_region_arithmetic.hpp>
#include <fifi/final.hpp>
#include <fifi/full_table_arithmetic.hpp>
#include <fifi/packed_arithmetic.hpp>
#include <fifi/polynomial_degree.hpp>
#include <fifi/region_arithmetic.hpp>
#include <fifi/region_info.hpp>
#include <fifi/simple_online_arithmetic.hpp>

#include <gtest/gtest.h>

#include "expected_results.hpp"
#include "helper_catch_all.hpp"
#include "helper_region_fall_through.hpp"

namespace fifi
{
    namespace
    {
        template<class Field>
        struct dummy_stack_fall_through : public
        binary8_full_table_region_arithmetic<Field,
        helper_region_fall_through<Field,
        region_info<Field,
        full_table_arithmetic<Field,
        simple_online_arithmetic<
        polynomial_degree<
        helper_catch_all<Field> > > > > > >
        { };
    }

    namespace
    {
        template<class Field>
        struct dummy_stack : public
        binary8_full_table_region_arithmetic<Field,
        region_arithmetic<
        region_info<Field,
        packed_arithmetic<
        full_table_arithmetic<Field,
        simple_online_arithmetic<
        polynomial_degree<
        final<Field> > > > > > > >
        { };
    }
}

TEST(TestBinary8FullTableRegionArithmetic, fall_through)
{
    {
        fifi::region_fall_through_result expected;
        SCOPED_TRACE("binary4");
        fifi::helper_region_fall_through_test<fifi::binary4,
        fifi::dummy_stack_fall_through<fifi::binary4> >(expected);
    }
    {
        SCOPED_TRACE("binary8");
        fifi::region_fall_through_result expected;
        expected.multiply_add = false;
        expected.multiply_subtract = false;
        fifi::helper_region_fall_through_test<fifi::binary8,
        fifi::dummy_stack_fall_through<fifi::binary8> >(expected);
    }
}

TEST(TestBinary8FullTableRegionArithmetic, init)
{
    fifi::dummy_stack<fifi::binary8> stack;
    (void) stack;
}

TEST(TestBinary8FullTableRegionArithmetic, multiply_add)
{
    check_results_region_multiply_add<fifi::dummy_stack<fifi::binary8> >();
}

TEST(TestBinary8FullTableRegionArithmetic, multiply_subtract)
{
    check_results_region_multiply_subtract<fifi::dummy_stack<fifi::binary8> >();
}