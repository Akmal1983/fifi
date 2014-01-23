// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

namespace fifi
{

    /// Packed arithmetic is used to perform standard arithmetic
    /// operations but where field elementes are expected to be
    /// "packed" in the value_type. This ensures that we correctly
    /// deals with fields that do not have a corresponding native data
    /// type to contain its value. E.g. for binary4 we have no data
    /// type in C++ to efficiently represent it, therefore we
    /// typically have to store the binary4 value in a larger data
    /// type e.g. uint8_t. This is wasteful since in this specific
    /// case we waste 4 bits of the uint8_t. To remedy this one may
    /// use the packed operations. When calling packed operations the
    /// functions will expect that the finite field values have been
    /// "packed" into the data type used e.g. in the case of binary4
    /// we expect that the uint8_t will contain two finite field
    /// values.
    ///
    /// In the following we implement the "base" cases where the data
    /// type used matches the finite field, so we may simply pass the
    /// computations to the standard arithmetic operations.
    template<class Super>
    class packed_arithmetic : public Super
    {
    public:

        /// Typedef of the data type used for each field element
        typedef typename Super::value_type value_type;

    public:

        /// @copydoc finite_field::packed_add()
        value_type packed_multiply(value_type a, value_type b) const
        {
            return Super::multiply(a, b);
        }

        /// @copydoc finite_field::divide()
        value_type packed_divide(value_type numerator,
                                 value_type denominator) const
        {
            return Super::divide(numerator, denominator);
        }

        /// @copydoc finite_field::invert()
        value_type packed_invert(value_type a) const
        {
            return Super::invert(a);
        }

        /// @copydoc finite_field::add()
        value_type packed_add(value_type a, value_type b) const
        {
            return Super::packed_add(a, b);
        }

        /// @copydoc finite_field::subtract()
        value_type packed_subtract(value_type a, value_type b) const
        {
            return Super::subract(a, b);
        }

    };

}
