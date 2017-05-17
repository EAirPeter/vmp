#ifndef VMP_INTRIN_VU64_COMPARE_HPP_
#define VMP_INTRIN_VU64_COMPARE_HPP_

#include "../Common.hpp"

#define VMP_VU64_CMP_ASM_QWD(len_, ops_)                        \
U64 uTmp;                                                       \
asm volatile (                                                  \
    ops_                                                        \
    "xor %k[qtm], %k[qtm]\n\t"                                  \
    "jmp jEnd_%=\n"                                             \
"jDone_%=:\n\t"                                                 \
    "sbb %k[qtm], %k[qtm]\n\t"                                  \
    "or %k[qtm], 1\n"                                           \
"jEnd_%=:\n\t"                                                  \
:   [qtm]"=&r"(uTmp)                                            \
:   [len]"i"(len_),                                             \
    [lhs]"r"(pLhs),                                             \
    [rhs]"r"(pRhs)                                              \
:   "cc"                                                        \
);                                                              \
return (int) uTmp

#define VMP_VU64_CMP_ASM_VEC(ops_)                              \
__m256i yTmp, yZero {};                                         \
U64 uIter, uTmp;                                                \
asm volatile (                                                  \
    ops_ "\n"                                                   \
"jLoop_%=:\n\t"                                                 \
    "vmovdqu %[ytm], ymmword ptr[%[lhs] + %[itr]]\n\t"          \
    "vxorps %[ytm], %[ytm], ymmword ptr[%[rhs] + %[itr]]\n\t"   \
    "vptest %[zero], %[ytm]\n\t"                                \
    "jnc jIn256_%=\n\t"                                         \
    "sub %[itr], 32\n\t"                                        \
    "jnz jLoop_%=\n\t"                                          \
    "xor %k[qtm], %k[qtm]\n\t"                                  \
    "jmp jEnd_%=\n"                                             \
"jIn256_%=:\n\t"                                                \
    "mov %[qtm], qword ptr[%[lhs] + %[itr] + 24]\n\t"           \
    "cmp %[qtm], qword ptr[%[rhs] + %[itr] + 24]\n\t"           \
    "jne jDone_%=\n\t"                                          \
    "mov %[qtm], qword ptr[%[lhs] + %[itr] + 16]\n\t"           \
    "cmp %[qtm], qword ptr[%[rhs] + %[itr] + 16]\n\t"           \
    "jne jDone_%=\n\t"                                          \
    "mov %[qtm], qword ptr[%[lhs] + %[itr] + 8]\n\t"            \
    "cmp %[qtm], qword ptr[%[rhs] + %[itr] + 8]\n\t"            \
    "jne jDone_%=\n\t"                                          \
    "mov %[qtm], qword ptr[%[lhs] + %[itr]]\n\t"                \
    "cmp %[qtm], qword ptr[%[rhs] + %[itr]]\n"                  \
"jDone_%=:\n\t"                                                 \
    "sbb %k[qtm], %k[qtm]\n\t"                                  \
    "or %k[qtm], 1\n"                                           \
"jEnd_%=:"                                                      \
:   [ytm]"=&x"(yTmp),                                           \
    [itr]"+&r"(uIter = kLength >> 2 << 5),                      \
    [qtm]"=&r"(uTmp)                                            \
:   [zero]"x"(yZero),                                           \
    [len]"i"(kLength),                                          \
    [lhs]"r"(pLhs - 4),                                         \
    [rhs]"r"(pRhs - 4)                                          \
:   "cc"                                                        \
);                                                              \
return (int) uTmp;

#define VMP_VU64_CMP_ASM_OPR(off_) \
    "mov %[qtm], qword ptr[%[lhs] + (%[len] + " off_ ") * 8]\n\t"   \
    "cmp %[qtm], qword ptr[%[rhs] + (%[len] + " off_ ") * 8]\n\t"   \
    "jne jDone_%=\n\t"

namespace vmp::intrin {
    template<U64 kLength> inline
    int Vu64Compare(const U64 *pLhs, const U64 *pRhs) noexcept {
        constexpr U64 uLenMod = kLength & 3;
        if constexpr (uLenMod == 3) {
            VMP_VU64_CMP_ASM_VEC(
                VMP_VU64_CMP_ASM_OPR("3")
                VMP_VU64_CMP_ASM_OPR("2")
                VMP_VU64_CMP_ASM_OPR("1")
            );
        }
        else if constexpr (uLenMod == 2) {
            VMP_VU64_CMP_ASM_VEC(
                VMP_VU64_CMP_ASM_OPR("3")
                VMP_VU64_CMP_ASM_OPR("2")
            );
        }
        else if constexpr (uLenMod == 1) {
            VMP_VU64_CMP_ASM_VEC(
                VMP_VU64_CMP_ASM_OPR("3")
            );
        }
        else {
            VMP_VU64_CMP_ASM_VEC(
                ""
            );
        }
    }
    template<> inline
    int Vu64Compare<4>(const U64 *pLhs, const U64 *pRhs) noexcept {
        VMP_VU64_CMP_ASM_QWD(
            4,
            VMP_VU64_CMP_ASM_OPR("-1")
            VMP_VU64_CMP_ASM_OPR("-2")
            VMP_VU64_CMP_ASM_OPR("-3")
            VMP_VU64_CMP_ASM_OPR("-4")
        );
    }
    template<> inline
    int Vu64Compare<3>(const U64 *pLhs, const U64 *pRhs) noexcept {
        VMP_VU64_CMP_ASM_QWD(
            3,
            VMP_VU64_CMP_ASM_OPR("-1")
            VMP_VU64_CMP_ASM_OPR("-2")
            VMP_VU64_CMP_ASM_OPR("-3")
        );
    }
    template<> inline
    int Vu64Compare<2>(const U64 *pLhs, const U64 *pRhs) noexcept {
        VMP_VU64_CMP_ASM_QWD(
            2,
            VMP_VU64_CMP_ASM_OPR("-1")
            VMP_VU64_CMP_ASM_OPR("-2")
        );
    }
    template<> inline
    int Vu64Compare<1>(const U64 *pLhs, const U64 *pRhs) noexcept {
        VMP_VU64_CMP_ASM_QWD(
            1,
            VMP_VU64_CMP_ASM_OPR("-1")
        );
    }
    template<> constexpr
    int Vu64Compare<0>(const U64 *, const U64 *) noexcept {
        return 0;
    }
}

#endif // ifndef VMP_INTRIN_VU64_COMPARE_HPP_

