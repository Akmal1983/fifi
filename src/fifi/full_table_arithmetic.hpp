// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cassert>
#include <cstdint>
#include <type_traits>
#include <vector>

#include "binary.hpp"
#include "binary16.hpp"
#include "is_valid_element.hpp"
#include "prime2325.hpp"

namespace fifi
{
    /// Produces a full lookup table of the multiplication and
    /// division operations.  The full look-up table works fine for
    /// the 2^8 binary extension field.  However, for higher field
    /// sizes it will most likely not work due to the very high memory
    /// requirements.
    template<class Super>
    class full_table_arithmetic : public Super
    {

    public:

        /// @copydoc layer::field_type
        typedef typename Super::field_type field_type;

        /// @copydoc layer::value_type
        typedef typename field_type::value_type value_type;

        // The full lookup table does not support the binary field
        static_assert(!std::is_same<binary, field_type>::value,
            "This layer does not support the binary field");

        // Check for the binary16 field the full lookup table cannot
        // be used with binary16 due to the excessive amounts of
        // memory it would require to create the look-up table.
        static_assert(!std::is_same<binary16, field_type>::value,
            "This layer does not support the binary16 field");

        // Static check for the prime2325 field, the full lookup table
        // only works with binary extension fields
        static_assert(!std::is_same<prime2325, field_type>::value,
            "This layer does not support the 2^32 - 5 prime field");

    public:

        /// Constructor
        full_table_arithmetic()
        {
            m_multiplication_table.resize(
                field_type::order * field_type::order, '\0');
            m_division_table.resize(
                field_type::order * field_type::order, '\0');

            for (uint32_t i = 0; i < field_type::order; ++i)
            {
                int offset = i * field_type::order;

                for (uint32_t j = 0; j < field_type::order; ++j)
                {
                    m_multiplication_table[offset + j] = Super::multiply(i,j);

                    if (j == 0) // Cannot divide by zero
                        continue;

                    m_division_table[offset + j] = Super::divide(i,j);
                }
            }
        }

        /// @copydoc layer::multiply(value_type, value_type) const
        value_type multiply(value_type a, value_type b) const
        {
            assert(is_valid_element<field_type>(a));
            assert(is_valid_element<field_type>(b));

            return m_multiplication_table[(a << field_type::degree) + b];
        }

        /// @copydoc layer::divide(value_type, value_type) const
        value_type divide(value_type numerator, value_type denominator) const
        {
            assert(is_valid_element<field_type>(numerator));
            assert(is_valid_element<field_type>(denominator));

            return m_division_table[
                (numerator << field_type::degree) + denominator];
        }

        /// @copydoc layer::invert(value_type) const
        value_type invert(value_type a) const
        {
            assert(is_valid_element<field_type>(a));

            return divide(1, a);
        }

        /// Get a pointer to a row in the multiplication table
        /// @param row Index of the row
        /// @return Row from the table
        const value_type* multiplication_row(uint32_t row) const
        {
            assert(row <= field_type::order);
            return &m_multiplication_table[row << field_type::degree];
        }

        /// Get a pointer to a row in the multiplication table
        /// @param row Index of the row
        /// @return Row from the table
        const value_type* division_row(uint32_t row) const
        {
            assert(row <= field_type::order);
            return &m_division_table[row << field_type::degree];
        }

    private:
        /// The multiplication table.
        std::vector<value_type> m_multiplication_table;

        /// The division table
        std::vector<value_type> m_division_table;
    };
}
