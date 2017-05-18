#ifndef VMP_INTRIN_VU64_MUL_HPP_
#define VMP_INTRIN_VU64_MUL_HPP_

#include "../Common.hpp"

#include "Vu64SetZero.hpp"

namespace vmp::intrin {
    template<U64 kLength> inline
    void Vu64Mul(U64 *pRes, const U64 *pLhs, const U64 *pRhs) noexcept {
        Vu64SetZero<kLength>(pRes);
        U64 uCarry, uTmp1, uTmp2, uIter1, uIter2;
        asm volatile ( "\n"
        "L_Loop1_%=:\n\t"
            "mov %[it2], %[it1]\n\t"
            "mov %[tm1], qword ptr[%[rhs] + %[it1] * 8]\n\t"
            "mov rax, qword ptr[%[lhs] + %[it2] * 8]\n\t"
            "mul %[tm1]\n\t"
            "add rax, qword ptr[%[res] + %[it2] * 8]\n\t"
            "jmp L_First_%=\n\t"
            "L_Loop2_%=:\n\t\t"
                "mov %[tm2], rdx\n\t\t"
                "adc %[tm2], qword ptr[%[res] + %[it2] * 8]\n\t\t"
                "setc %b[car]\n\t\t"
                "mov rax, qword ptr[%[lhs] + %[it2] * 8]\n\t\t"
                "mul %[tm1]\n\t\t"
                "add rdx, %[car]\n\t\t"
                "add rax, %[tm2]\n\t"
            "L_First_%=:\n\t\t"
                "mov qword ptr[%[res] + %[it2] * 8], rax\n\t\t"
                "inc %[it2]\n\t\t"
                "jnz L_Loop2_%=\n\t"
            "sub %[lhs], 8\n\t"
            "inc %[it1]\n\t"
            "jnz L_Loop1_%=\n\t"
        :   [car]"+&r"(uCarry = 0),
            [tm1]"=&r"(uTmp1),
            [tm2]"=&r"(uTmp2),
            [it1]"+&r"(uIter1 = -kLength),
            [it2]"=&r"(uIter2),
            [lhs]"+&r"(pLhs += kLength)
        :   [res]"r"(pRes + kLength),
            [rhs]"r"(pRhs + kLength)
        :   "cc", "memory", "rax", "rdx"
        );
    }
    template<> inline
    void Vu64Mul<2>(U64 *pRes, const U64 *pLhs, const U64 *pRhs) noexcept {
        U64 uTmp1, uTmp2;
        asm volatile (
            "mov rax, %[tmpl]\n\t"
            "mul %[tmpr]\n\t"
            "imul %[tmpl], %[rhs1]\n\t"
            "imul %[tmpr], %[lhs1]\n\t"
            "add rdx, %[tmpl]\n\t"
            "add rdx, %[tmpr]\n\t"
            "mov %[res0], rax\n\t"
            "mov %[res1], rdx"
        :   [res0]"=&m"(pRes[0]),
            [res1]"=&m"(pRes[1]),
            [tmpl]"+&r"(uTmp1 = pLhs[0]),
            [tmpr]"+&r"(uTmp2 = pRhs[0])
        :   [lhs1]"m"(pLhs[1]),
            [rhs1]"m"(pRhs[1])
        :   "cc", "rax", "rdx"
        );
    }
    template<> constexpr
    void Vu64Mul<1>(U64 *pRes, const U64 *pLhs, const U64 *pRhs) noexcept {
        pRes[0] = pLhs[0] * pRhs[0];
    }
    template<> constexpr
    void Vu64Mul<0>(U64 *, const U64 *, const U64 *) noexcept {
    }
}

#endif // ifndef VMP_INTRIN_VU64_MUL_HPP_

