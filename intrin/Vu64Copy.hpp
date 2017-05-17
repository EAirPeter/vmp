#ifndef VMP_INTRIN_VU64_COPY_HPP_
#define VMP_INTRIN_VU64_COPY_HPP_

#include "../Common.hpp"

namespace vmp::intrin {
    template<U64 kLength> inline
    void Vu64Copy(U64 *pLhs, const U64 *pRhs) noexcept {
        U64 uCounter;
        asm volatile (
            "rep movsq"
        :   "+S"(pRhs),
            "+D"(pLhs),
            "+c"(uCounter = kLength)
        ::  "memory"
        );
    }
    template<> constexpr
    void Vu64Copy<2>(U64 *pLhs, const U64 *pRhs) noexcept {
        pLhs[0] = pRhs[0];
        pLhs[1] = pRhs[1];
    }
    template<> constexpr
    void Vu64Copy<1>(U64 *pLhs, const U64 *pRhs) noexcept {
        pLhs[0] = pRhs[0];
    }
    template<> constexpr
    void Vu64Copy<0>(U64 *, const U64 *) noexcept {
    }
}

#endif // ifndef VMP_INTRIN_VU64_COPY_HPP_

