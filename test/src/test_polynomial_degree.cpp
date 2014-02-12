// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <fifi/binary.hpp>
#include <fifi/binary16.hpp>
#include <fifi/binary4.hpp>
#include <fifi/binary8.hpp>
#include <fifi/final.hpp>
#include <fifi/polynomial_degree.hpp>
#include <fifi/prime2325.hpp>

#include <gtest/gtest.h>

#include "expected_results.hpp"

namespace fifi
{
    namespace
    {
        template<class Field>
        struct dummy_stack : public
        polynomial_degree<
        final<Field> >
        { };
    }
}

TEST(TestPolynomialDegree, polynomial_degree)
{
    {
        SCOPED_TRACE("binary");
        check_results_find_degree<fifi::dummy_stack<fifi::binary> >();
    }
    {
        SCOPED_TRACE("binary4");
        check_results_find_degree<fifi::dummy_stack<fifi::binary4> >();
    }
    {
        SCOPED_TRACE("binary8");
        check_results_find_degree<fifi::dummy_stack<fifi::binary8> >();
    }
    {
        SCOPED_TRACE("binary16");
        check_results_find_degree<fifi::dummy_stack<fifi::binary16> >();
    }
    {
        SCOPED_TRACE("prime2325");
        check_results_find_degree<fifi::dummy_stack<fifi::prime2325> >();
    }
}