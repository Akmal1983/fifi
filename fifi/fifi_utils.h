// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#ifndef FIFI_FIFI_UTILS_H
#define FIFI_FIFI_UTILS_H

#include "field_types.h"

namespace fifi
{

    /// Returns the size in bytes needed to store a certain
    /// number of elements
    /// @param elements, the number of elements to store
    /// @return the size in bytes that need to be stored
    template<class Field>
    inline uint32_t bytes_needed(uint32_t elements)
    {
        assert(elements > 0);

        return sizeof(typename Field::value_type) * elements;
    }

    /// size_needed specilization for the binary field
    /// @see size_needed()
    template<>
    inline uint32_t bytes_needed<binary>(uint32_t elements)
    {
        assert(elements > 0);

        // Note: std::numeric_limits<value_type>::digits
        // returns the number of bits in the template parameter

        // ceil(x/y) = ((x - 1) / y) + 1
        return ((elements - 1) /
                std::numeric_limits<binary::value_type>::digits) + 1;
    }

    /// Usefull abstraction functions for accessing field elements if
    /// a mixed binary & other fields implementation is created.
    /// Returns the value of an element at the specific position in the
    /// symbol.
    template<class Field>
    inline typename Field::value_type get_value(typename Field::value_ptr symbol,
                                                uint32_t index)
    {
        assert(symbol != 0);
        // typedef typename Field::value_ptr value_ptr;
        // value_ptr elements = reinterpret_cast<value_ptr>(symbol);
        return symbol[index];
    }

    template<>
    inline binary::value_type get_value<binary>(uint8_t *symbol, uint32_t index)
    {
        assert(symbol != 0);

        typedef binary::value_ptr value_ptr;
        value_ptr elements = reinterpret_cast<value_ptr>(symbol);

        uint32_t array_index = index / std::numeric_limits<binary::value_type>::digits;
        uint32_t offset = index % std::numeric_limits<binary::value_type>::digits;

        return (elements[array_index] >> offset) & 0x1;
    }

    template<class Field>
    inline void set_value(typename Field::value_ptr elements, uint32_t index,
                          typename Field::value_type value)
    {
        assert(elements != 0);
        elements[index] = value;
    }

    template<>
    inline void set_value<binary>(binary::value_ptr elements, uint32_t index,
                                  binary::value_type value)
    {
        /// @todo: find a better/faster way to do this

        assert(elements != 0);
        assert(value < 2); // only {0,1} allowed

        uint32_t array_index = index / std::numeric_limits<binary::value_type>::digits;
        uint32_t offset = index % std::numeric_limits<binary::value_type>::digits;

        binary::value_type mask = 1 << offset;

        if(value)
        {
            elements[array_index] |= mask;
        }
        else
        {
            elements[array_index] &= ~mask;
        }
    }
}

#endif

