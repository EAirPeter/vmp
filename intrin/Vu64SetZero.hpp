#ifndef VMP_INTRIN_VU64_SETZERO_HPP_
#define VMP_INTRIN_VU64_SETZERO_HPP_

#include "../Common.hpp"

namespace vmp::intrin {
    template<U64 kLength> inline
    void Vu64SetZero(U64 *pData) noexcept {
        U64 uCounter;
        asm volatile (
            "rep stosq"
        :   "+D"(pData),
            "+c"(uCounter = kLength)
        :   "a"((U64) 0)
        :   "memory"
        );
    }
    template<> constexpr
    void Vu64SetZero<2>(U64 *pData) noexcept {
        pData[0] = 0;
        pData[1] = 0;
    }
    template<> constexpr
    void Vu64SetZero<1>(U64 *pData) noexcept {
        pData[0] = 0;
    }
    template<> constexpr
    void Vu64SetZero<0>(U64 *) noexcept {
    }
}

#endif // ifndef VMP_INTRIN_VU64_SETZERO_HPP_

