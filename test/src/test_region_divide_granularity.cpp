// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstdint>

#include <gtest/gtest.h>

#include <fifi/binary.hpp>
#include <fifi/binary4.hpp>
#include <fifi/binary8.hpp>
#include <fifi/binary16.hpp>
#include <fifi/final.hpp>
#include <fifi/region_divide_granularity.hpp>

#include "random_constant.hpp"
#include "helper_test_buffer.hpp"
#include "capture_calls.hpp"
#include "helper_region_info.hpp"
#include "helper_fall_through.hpp"

#include <iostream>

namespace fifi
{
    namespace
    {
        template<class Super>
        class dummy_typedef_basic_super : public Super
        {
        public:
            typedef Super BasicSuper;
        };

        template<class Super>
        class dummy_typedef_optimized_super : public Super
        {
        public:
            typedef Super OptimizedSuper;
        };

        template<class Field>
        class dummy_stack : public
            region_divide_granularity<
            dummy_typedef_optimized_super<
            helper_region_info<0,0,0,0,
            helper_fall_through<Field,
            dummy_typedef_basic_super<
            helper_region_info<0,0,0,0,
            helper_fall_through<Field,
            final<Field> > > > > > > >
        { };

        template<class Field>
        struct test_operation
        {
            typedef Field field_type;

            typedef typename field_type::value_type value_type;

            typedef dummy_stack<field_type> stack_type;
            typedef capture_calls<value_type> calls_type;

            typedef typename stack_type::BasicSuper basic_super;
            typedef typename stack_type::OptimizedSuper optimized_super;

            test_operation()
            {
                m_value_size = sizeof(value_type);
                // The alignment is measured in bytes
                m_alignment = 16;
                // The granularity is measured in value_types
                m_granularity = 16;
                m_length = 100;
            }

            void run_test()
            {
                fifi::helper_test_buffer<value_type> dest_buffer(
                    m_length, m_alignment);
                fifi::helper_test_buffer<value_type> src_buffer(
                    m_length, m_alignment);

                random_constant<field_type> constants;
                auto constant = constants.pack();

                value_type* dest = &dest_buffer.data()[0];
                value_type* src = &src_buffer.data()[0];

                ASSERT_EQ((uintptr_t)dest % m_alignment,
                          (uintptr_t)src  % m_alignment);

                // Test the division of the granulated part and the tail
                for (uint32_t length = 1; length <= m_length; length++)
                {

                    run_operation(
                        std::mem_fn(&stack_type::region_add),
                        std::mem_fn(&calls_type::call_region_add),
                        length, dest, src);

                    run_operation(
                        std::mem_fn(&stack_type::region_subtract),
                        std::mem_fn(&calls_type::call_region_subtract),
                        length, dest, src);

                    run_operation(
                        std::mem_fn(&stack_type::region_multiply),
                        std::mem_fn(&calls_type::call_region_multiply),
                        length, dest, src);

                    run_operation(
                        std::mem_fn(&stack_type::region_divide),
                        std::mem_fn(&calls_type::call_region_divide),
                        length, dest, src);

                    run_operation(
                        std::mem_fn(&stack_type::region_multiply_add),
                        std::mem_fn(&calls_type::call_region_multiply_add),
                        length, dest, src, constant);

                    run_operation(
                        std::mem_fn(&stack_type::region_multiply_subtract),
                        std::mem_fn(&calls_type::call_region_multiply_subtract),
                        length, dest, src, constant);
                }
            }


            template<class Function, class CallFunction, class... Args>
            void run_operation(Function function,
                CallFunction call_function,  uint32_t length, value_type* dest,
                Args&&... args)
            {
                basic_super& basic = m_stack;
                optimized_super& optimized = m_stack;

                basic.set_granularity(1U);
                optimized.set_granularity(m_granularity);

                optimized.clear();
                basic.clear();
                m_basic_calls.clear();
                m_optimized_calls.clear();
                function(m_stack, dest, args..., length);

                // Calculate the tail that is not granulated correctly
                uint32_t tail = length % m_granularity;

                // Calculate the number of granulated values
                uint32_t optimizable = length - tail;

                if (optimizable > 0)
                {
                    // The optimized implementation processes the
                    // granulated part
                    call_function(
                        m_optimized_calls, dest, args..., optimizable);
                }

                if (tail > 0)
                {
                    //The basic implementation runs on the tail
                    second_part_helper(
                        call_function, optimizable, tail, dest, args...);
                }

                EXPECT_EQ(m_optimized_calls, optimized.m_calls);
                EXPECT_EQ(m_basic_calls, basic.m_calls);
            }

            template<class CallFunction, class... Args>
            void second_part_helper(CallFunction call_function,
                uint32_t optimizable, uint32_t length, value_type* dest,
                const value_type* src, Args&&... args)
            {
                call_function(m_basic_calls, dest + optimizable,
                    src + optimizable, args..., length);
            }

            template<class CallFunction>
            void second_part_helper(CallFunction call_function,
                uint32_t optimizable, uint32_t length, value_type* dest, value_type constant)
            {
                call_function(m_basic_calls, dest + optimizable, constant,
                    length);
            }

        protected:

            stack_type m_stack;
            calls_type m_basic_calls;
            calls_type m_optimized_calls;

            uint32_t m_value_size;
            uint32_t m_alignment;
            uint32_t m_granularity;
            uint32_t m_length;

            random_constant<field_type> m_constants;
            value_type m_last_constant;
        };
    }
}

TEST(TestRegionDivideGranularity, granularity)
{
    fifi::dummy_stack<fifi::binary8> stack;
    fifi::dummy_stack<fifi::binary8>::BasicSuper& basic = stack;
    fifi::dummy_stack<fifi::binary8>::OptimizedSuper& optimized = stack;

    basic.set_granularity(1U);
    optimized.set_granularity(16U);

    EXPECT_EQ(1U, basic.granularity());
    EXPECT_EQ(16U, optimized.granularity());
    EXPECT_EQ(16U, stack.granularity());
}

TEST(TestRegionDivideGranularity, max_granularity)
{
    fifi::dummy_stack<fifi::binary8> stack;
    fifi::dummy_stack<fifi::binary8>::BasicSuper& basic = stack;
    fifi::dummy_stack<fifi::binary8>::OptimizedSuper& optimized = stack;

    basic.set_max_granularity(1U);
    optimized.set_max_granularity(16U);

    EXPECT_EQ(1U, basic.max_granularity());
    EXPECT_EQ(16U, optimized.max_granularity());
    EXPECT_EQ(16U, stack.max_granularity());
}

TEST(TestRegionDivideGranularity, all)
{
    {
        SCOPED_TRACE("binary");
        fifi::test_operation<fifi::binary>().run_test();
    }
    {
        SCOPED_TRACE("binary4");
        fifi::test_operation<fifi::binary4>().run_test();
    }
    {
        SCOPED_TRACE("binary8");
        fifi::test_operation<fifi::binary8>().run_test();
    }
    {
        SCOPED_TRACE("binary16");
        fifi::test_operation<fifi::binary16>().run_test();
    }
}
