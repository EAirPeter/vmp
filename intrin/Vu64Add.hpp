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
        "L_Loop_%=:\n\t"
            "mov %[tmp], qword ptr[%[lhs] + %[itr] * 8]\n\t"
            "adc %[tmp], qword ptr[%[rhs] + %[itr] * 8]\n\t"
            "mov qword ptr[%[res] + %[itr] * 8], %[tmp]\n\t"
            "inc %[itr]\n\t"
            "jnz L_Loop_%="
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
        U64 uTmp;
        asm volatile (
            "mov %[qtmp], %[lhs0]\n\t"
            "add %[qtmp], %[rhs0]\n\t"
            "mov %[res0], %[qtmp]\n\t"
            "mov %[qtmp], %[lhs1]\n\t"
            "adc %[qtmp], %[rhs1]\n\t"
            "mov %[res1], %[qtmp]"
        :   [res0]"=&m"(pRes[0]),
            [res1]"=&m"(pRes[1]),
            [qtmp]"=&r"(uTmp)
        :   [lhs0]"m"(pLhs[0]),
            [lhs1]"m"(pLhs[1]),
            [rhs0]"m"(pRhs[0]),
            [rhs1]"m"(pRhs[1])
        :   "cc"
        );
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

