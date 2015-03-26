// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>
#include <cstdlib>
#include <utility>

#include "fifi_utils.hpp"
#include "find_polynomial_degree.hpp"
#include "is_valid_element.hpp"

namespace fifi
{
    /// Simple online finite field algorithms - computes the results
    /// on the fly without relying on pre-computed look-up tables etc.
    template<class Super>
    class simple_online_arithmetic : public Super
    {
    public:

        /// @copydoc layer::field_type
        typedef typename Super::field_type field_type;

        /// @copydoc layer::value_type
        typedef typename field_type::value_type value_type;

        // Static check for the prime2325 field, the simple online finite field
        // algorithms only works with binary extension fields
        static_assert(!std::is_same<prime2325, field_type>::value,
            "This layer does not support the 2^32 - 5 prime field");

    public:

        /// @copydoc layer::add(value_type, value_type) const
        value_type add(value_type a, value_type b) const
        {
            assert(is_valid_element<field_type>(a));
            assert(is_valid_element<field_type>(b));
            return a ^ b;
        }

        /// @copydoc layer::subtract(value_type, value_type) const
        value_type subtract(value_type a, value_type b) const
        {
            assert(is_valid_element<field_type>(a));
            assert(is_valid_element<field_type>(b));
            // In the binary extension fields add and subtract are the same
            return add(a, b);
        }

        /// @copydoc layer::multiply(value_type, value_type) const
        value_type multiply(value_type a, value_type b) const
        {
            static_assert(!std::is_same<binary, field_type>::value,
                          "This member function does not support the "
                          "binary  field");

            assert(is_valid_element<field_type>(a));
            assert(is_valid_element<field_type>(b));

            if ((a == 0) || (b == 0))
                return 0;

            value_type high_bit_flag = 0;
            value_type low_bit_flag = 0;

            value_type high_bit_mask = 1U << (field_type::degree - 1);

            value_type result = 0;

            /// Typedef of the data type used for each the degree of the field
            typedef typename field_type::degree_type degree_type;

            for (degree_type i = 0; i < field_type::degree; ++i)
            {
                low_bit_flag = b & 0x1;

                if (low_bit_flag != 0)
                {
                    result ^= a;
                }

                high_bit_flag = a & high_bit_mask;

                a <<= 1;
                b >>= 1;

                if (high_bit_flag != 0)
                {
                    a ^= field_type::prime;
                }
            }

            return result & field_type::max_value;

        }

        /// @copydoc layer::divide(value_type, value_type) const
        value_type divide(value_type numerator, value_type denominator) const
        {
            static_assert(!std::is_same<binary, field_type>::value,
                "This member function does not support the binary field");

            assert(is_valid_element<field_type>(numerator));
            assert(is_valid_element<field_type>(denominator));

            return multiply(invert(denominator), numerator);
        }

        /// @copydoc layer::invert(value_type) const
        value_type invert(value_type a) const
        {
            static_assert(!std::is_same<binary, field_type>::value,
                "This member function does not support the binary field");

            assert(a != 0); // Zero has no inverse
            assert(is_valid_element<field_type>(a));

            // If element is 1 the inverse is 1, since we had to
            // 'unwrap' the first iteration (see below), we make an
            // explicit check here.
            if (a == 1)
                return 1;

            value_type r_large = field_type::prime;
            value_type r_small = a;

            value_type y_large = 0;
            value_type y_small = 1;

            // Since we cannot use the element data type to represent
            // the field polynomial we would not get a correct degree
            // calculation in the first iteration. Thus we perform the
            // first iteration of the algorithm here with the correct
            // prime degree. In the following iterations of the
            // algorithm the degree of r_large, which initially holds
            // our polynomial will be representable in value_type
            int32_t j = field_type::degree -
                find_polynomial_degree<field_type>(r_small);

            assert(j > 0);

            r_large ^= (r_small << j);
            y_large ^= (y_small << j);

            r_large &= field_type::max_value;
            y_large &= field_type::max_value;

            while (r_large != 1)
            {
                assert(r_large > 1);

                j = find_polynomial_degree<field_type>(r_large) -
                    find_polynomial_degree<field_type>(r_small);

                if (j < 0)
                {
                    std::swap(r_large, r_small);
                    std::swap(y_large, y_small);

                    j = std::abs(j);
                }

                r_large ^= (r_small << j);
                y_large ^= (y_small << j);

                r_large &= field_type::max_value;
                y_large &= field_type::max_value;

            }

            return y_large;
        }
    };
}
