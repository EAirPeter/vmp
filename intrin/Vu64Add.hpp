#ifndef VMP_INTRIN_VU64_ADD_HPP_
#define VMP_INTRIN_VU64_ADD_HPP_

#include "../Common.hpp"

namespace vmp::intrin {
    template<U64 kLength> inline
    void Vu64Add(U64 *pRes, const U64 *pLhs, const U64 *pRhs) noexcept {
        U64 uTmp, uIter;
        asm volatile (
            "mov %[tmp], qword ptr[%[lhs] + %[len] * 8]\n\t"
            "add %[tmp], qword ptr[%[rhs] + %[len] * 8]\n\t"
            "mov qword ptr[%[res] + %[len] * 8], %[tmp]\n"
        "jLoop_%=:\n\t"
            "mov %[tmp], qword ptr[%[lhs] + %[itr] * 8]\n\t"
            "adc %[tmp], qword ptr[%[rhs] + %[itr] * 8]\n\t"
            "mov qword ptr[%[res] + %[itr] * 8], %[tmp]\n\t"
            "inc %[itr]\n\t"
            "jnz jLoop_%="
        :   [tmp]"=&r"(uTmp),
            [itr]"+&r"(uIter = -kLength + 1)
        :   [len]"i"(-kLength),
            [res]"r"(pRes + kLength),
            [lhs]"r"(pLhs + kLength),
            [rhs]"r"(pRhs + kLength)
        :   "cc", "memory"
        );
    }
    template<> inline
    void Vu64Add<2>(U64 *pRes, const U64 *pLhs, const U64 *pRhs) noexcept {
        *(U128 *) pRes = *(const U128 *) pLhs + *(const U128 *) pRhs;
    }
    template<> constexpr
    void Vu64Add<1>(U64 *pRes, const U64 *pLhs, const U64 *pRhs) noexcept {
        pRes[0] = pLhs[0] + pRhs[0];
    }
    template<> constexpr
    void Vu64Add<0>(U64 *, const U64 *, const U64 *) noexcept {
    }
}

#endif // ifndef VMP_INTRIN_VU64_ADD_HPP_

