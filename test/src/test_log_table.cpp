// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <gtest/gtest.h>

#include <fifi/log_table.hpp>
#include <fifi/field_types.hpp>

#include "expected_results.hpp"

TEST(test_log_table,binary8multiply)
{
    check_results_multiply<fifi::log_table<fifi::binary8> >();
}

TEST(test_log_table,binary8invert)
{
    check_results_invert<fifi::log_table<fifi::binary8> >();
}

TEST(test_log_table,binary8divide)
{
    check_results_divide<fifi::log_table<fifi::binary8> >();
}

TEST(test_log_table,binary8add)
{
    check_results_add<fifi::log_table<fifi::binary8> >();
}

TEST(test_log_table,binary8subtract)
{
    check_results_subtract<fifi::log_table<fifi::binary8> >();
}

TEST(test_log_table,binary16multiply)
{
    check_results_multiply<fifi::log_table<fifi::binary16> >();
}

TEST(test_log_table,binary16invert)
{
    check_results_invert<fifi::log_table<fifi::binary16> >();
}

TEST(test_log_table,binary16divide)
{
    check_results_divide<fifi::log_table<fifi::binary16> >();
}

TEST(test_log_table,binary16add)
{
    check_results_add<fifi::log_table<fifi::binary16> >();
}

TEST(test_log_table,binary16subtract)
{
    check_results_subtract<fifi::log_table<fifi::binary16> >();
}

TEST(test_log_table,binary8randCheck)
{
    check_random<fifi::log_table<fifi::binary8> >();
}

TEST(test_log_table,binary16randCheck)
{
    check_random<fifi::log_table<fifi::binary16> >();
}

