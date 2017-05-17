#ifndef VMP_COMMON_HPP_
#define VMP_COMMON_HPP_

#include <cstdint>

#include <intrin.h>
#include <immintrin.h>

#define CONCAT_(a_, b_) a_ ## b_
#define CONCAT(a_, b_) CONCAT_(a_, b_)
#define TOSTR_(a_) #a_
#define TOSTR(a_) TOSTR_(a_)

namespace vmp {
    using U8  = std::uint8_t;
    using U16 = std::uint16_t;
    using U32 = std::uint32_t;
    using U64 = std::uint64_t;
    using U128 = unsigned __int128;
    using I8  = std::int8_t;
    using I16 = std::int16_t;
    using I32 = std::int32_t;
    using I64 = std::int64_t;
    using I128 = __int128;
}

#endif // ifndef VMP_COMMON_HPP_

