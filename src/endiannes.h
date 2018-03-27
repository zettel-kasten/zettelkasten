/** @file endian.h
    @brief Big/Little endian

 This file contains declaration of integers that are stored as little or big endian
 and are automatically converted to/from native byteorder.
 This is usefull when reading from files with specific format.
*/
#pragma once

#include <boost/detail/endian.hpp>
#include <stdint.h>

/**
 Convert between big <-> little endian. Don't use this function, use typedefs at the end of this file.
*/
template<typename TInt>
inline TInt invertByteOrder(TInt Value)
{
    switch (sizeof(TInt))
    {
    case 1:
        return Value;
    case 2:
        {
            uint16_t v(Value);
            return (v << 8) | ((v >> 8) & 0xFF);
        }
    case 4:
        {
            uint32_t v(Value);
            uint32_t high = invertByteOrder( uint16_t(v) ) & 0xFFFF;
            uint32_t low  = invertByteOrder( uint16_t(v >> 16) ) & 0xFFFF;
            return (high << 16) | low;
        }
    case 8:
        {
            uint64_t v(Value);
            uint64_t high = invertByteOrder( uint32_t(v) ) & 0xFFFFFFFF;
            uint64_t low  = invertByteOrder( uint32_t(v >> 32) ) & 0xFFFFFFFF;
            return (high << 32) | low;
        }
    default:
        return 0;
    }
}

/**
 Objects of this class store numbers in reversed byte-order.
 This class offers automatic conversion to/from native byte-order, using typecast operators.
 No operators (+, -, *, /, ...) are provided, just use typecast, then operator.
*/
template<typename T>
class COtherEndian
{
    T m_Value; // reversed byte-order

public:
    /// Construct from Value. Value has native byte-order.
    COtherEndian(T Value = 0)
        : m_Value(invertByteOrder(Value))
    {}

    /// Get value in native byte-order.
    operator T() const
    { return invertByteOrder(m_Value); }
};

// Big/Little-endian integer types.
#ifdef BOOST_BIG_ENDIAN
    typedef COtherEndian<uint16_t> uint16LE;
    typedef COtherEndian<uint32_t> uint32LE;
    typedef COtherEndian<uint64_t> uint64LE;
    typedef COtherEndian< int16_t>  int16LE;
    typedef COtherEndian< int32_t>  int32LE;
    typedef COtherEndian< int64_t>  int64LE;
    typedef uint16_t uint16BE;
    typedef uint32_t uint32BE;
    typedef uint64_t uint64BE;
    typedef  int16_t  int16BE;
    typedef  int32_t  int32BE;
    typedef  int64_t  int64BE;
#else
    typedef COtherEndian<uint16_t> uint16BE;
    typedef COtherEndian<uint32_t> uint32BE;
    typedef COtherEndian<uint64_t> uint64BE;
    typedef COtherEndian< int16_t>  int16BE;
    typedef COtherEndian< int32_t>  int32BE;
    typedef COtherEndian< int64_t>  int64BE;
    typedef uint16_t uint16LE;
    typedef uint32_t uint32LE;
    typedef uint64_t uint64LE;
    typedef  int16_t  int16LE;
    typedef  int32_t  int32LE;
    typedef  int64_t  int64LE;
#endif
