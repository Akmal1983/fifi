// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <gtest/gtest.h>

#include <fifi/is_valid_element.hpp>
#include <fifi/binary.hpp>
#include <fifi/binary4.hpp>

TEST(test_is_valid_element, api)
{
    EXPECT_TRUE(fifi::is_valid_element<fifi::binary>(0U));
    EXPECT_TRUE(fifi::is_valid_element<fifi::binary>(1U));
    EXPECT_FALSE(fifi::is_valid_element<fifi::binary>(2U));

    EXPECT_TRUE(fifi::is_valid_element<fifi::binary4>(0U));
    EXPECT_TRUE(fifi::is_valid_element<fifi::binary4>(1U));
    EXPECT_TRUE(fifi::is_valid_element<fifi::binary4>(2U));
    EXPECT_FALSE(fifi::is_valid_element<fifi::binary4>(16U));
}
