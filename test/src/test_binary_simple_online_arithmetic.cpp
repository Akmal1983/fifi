// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <fifi/binary.hpp>
#include <fifi/binary16.hpp>
#include <fifi/binary4.hpp>
#include <fifi/binary8.hpp>
#include <fifi/binary_simple_online_arithmetic.hpp>

#include <gtest/gtest.h>

#include "fifi_unit_test/expected_results.hpp"
#include "fifi_unit_test/helper_fall_through.hpp"

namespace fifi
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        template<class Field>
        struct dummy_stack : public
            binary_simple_online_arithmetic<Field,
            helper_fall_through<Field> >
        { };
    }
}

TEST(test_binary_simple_online_arithmetic, fall_through_binary)
{
    typedef fifi::dummy_stack<fifi::binary> stack;
    // The binary field specialization for this layer implements multiply,
    // divide, and invert. Therefore these should not fall through.
    fifi::capture_calls<fifi::binary::value_type> expected_calls;
    stack s;

    s.m_calls.clear();
    expected_calls.clear();
    s.multiply(1U, 1U);
    s.divide(1U, 1U);
    s.invert(1U);

    EXPECT_EQ(expected_calls, s.m_calls);

    // Test that other calls does fall through.
    fifi::test_fall_through_add<stack>();
    fifi::test_fall_through_subtract<stack>();
}

TEST(test_binary_simple_online_arithmetic, fall_through_binary4)
{
    fifi::test_fall_through<fifi::dummy_stack<fifi::binary4>>();
}

TEST(test_binary_simple_online_arithmetic, fall_through_binary8)
{
    fifi::test_fall_through<fifi::dummy_stack<fifi::binary8>>();
}

TEST(test_binary_simple_online_arithmetic, fall_through_binary16)
{
    fifi::test_fall_through<fifi::dummy_stack<fifi::binary16>>();
}

TEST(test_binary_simple_online_arithmetic, multiply)
{
    check_results_multiply<fifi::dummy_stack<fifi::binary> >();
}

TEST(test_binary_simple_online_arithmetic, divide)
{
    check_results_divide<fifi::dummy_stack<fifi::binary>>();
}

TEST(test_binary_simple_online_arithmetic, invert)
{
    check_results_invert<fifi::dummy_stack<fifi::binary>>();
}

TEST(test_binary_simple_online_arithmetic, random)
{
    check_random_default<fifi::dummy_stack<fifi::binary>>();
}
