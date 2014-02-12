// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

namespace fifi
{
    template<class Field, class Super>
    struct helper_region_fall_through : public Super
    {

        typedef Field field_type;

        typedef typename Field::value_type value_type;

        void region_multiply(value_type* dest, const value_type* src) const
        {
            (void) dest;
            (void) src;
            m_fall_through = true;
        }

        void region_divide(value_type* dest, const value_type* src) const
        {
            (void) dest;
            (void) src;
            m_fall_through = true;
        }

        void region_add(value_type* dest, const value_type* src) const
        {
            (void) dest;
            (void) src;
            m_fall_through = true;
        }

        void region_subtract(value_type* dest, const value_type* src) const
        {
            (void) dest;
            (void) src;
            m_fall_through = true;
        }

        void region_multiply_constant(
            value_type* dest, const value_type constant) const
        {
            (void) dest;
            (void) constant;
            m_fall_through = true;
        }

        void region_multiply_add(value_type* dest, const value_type* src,
                          value_type constant) const
        {
            (void) dest;
            (void) src;
            (void) constant;
            m_fall_through = true;
        }

        void region_multiply_subtract(value_type* dest, const value_type* src,
                                value_type constant) const
        {
            (void) dest;
            (void) src;
            (void) constant;
            m_fall_through = true;
        }

        mutable bool m_fall_through;
    };

    struct region_fall_through_result
    {
        region_fall_through_result() :
            add(true),
            subtract(true),
            multiply(true),
            divide(true),
            multiply_constant(true),
            multiply_add(true),
            multiply_subtract(true)
        { }

        bool add;
        bool subtract;
        bool multiply;
        bool divide;
        bool multiply_constant;
        bool multiply_add;
        bool multiply_subtract;
    };

    template<class Field, class Stack>
    void helper_region_fall_through_test(
        const region_fall_through_result& expected)
    {
        Stack stack;

        typedef typename Field::value_type value_type;

        uint32_t length = 128;
        std::vector<value_type> dest(length, 1);
        std::vector<value_type> src(length, ~0);
        value_type constant = 0;
        stack.set_length(length);

        stack.m_fall_through = false;
        stack.region_add(dest.data(), src.data());
        EXPECT_EQ(expected.add, stack.m_fall_through);

        stack.m_fall_through = false;
        stack.region_subtract(dest.data(), src.data());
        EXPECT_EQ(expected.subtract, stack.m_fall_through);

        stack.m_fall_through = false;
        stack.region_multiply(dest.data(), src.data());
        EXPECT_EQ(expected.multiply, stack.m_fall_through);

        stack.m_fall_through = false;
        stack.region_divide(dest.data(), src.data());
        EXPECT_EQ(expected.divide, stack.m_fall_through);

        stack.m_fall_through = false;
        stack.region_multiply_constant(dest.data(), constant);
        EXPECT_EQ(expected.multiply_constant, stack.m_fall_through);

        stack.m_fall_through = false;
        stack.region_multiply_add(dest.data(), src.data(), constant);
        EXPECT_EQ(expected.multiply_add, stack.m_fall_through);

        stack.m_fall_through = false;
        stack.region_multiply_subtract(dest.data(), src.data(), constant);
        EXPECT_EQ(expected.multiply_subtract, stack.m_fall_through);

    }
}