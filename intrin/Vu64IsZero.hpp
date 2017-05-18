#ifndef VMP_INTRIN_VU64_ISZERO_HPP_
#define VMP_INTRIN_VU64_ISZERO_HPP_

#include "../Common.hpp"

#define VMP_VU64_ISZ_ASM_QWD(ops_)                      \
bool bRes;                                              \
asm volatile (                                          \
    ops_                                                \
    "mov %[res], 1\n"                                   \
"L_End_%=:"                                             \
:   [res]"+&r"(bRes = false)                            \
:   [dat]"r"(pData)                                     \
:   "cc"                                                \
);                                                      \
return bRes;

#define VMP_VU64_ISZ_ASM_VEC(ops_)                      \
__m256i yTmp;                                           \
U64 uIter;                                              \
bool bRes;                                              \
asm volatile ("\n"                                      \
"L_Loop_%=:\n\t"                                        \
    "vmovdqu %[tmp], ymmword ptr[%[dat] + %[itr]]\n\t"  \
    "vptest %[tmp], %[tmp]\n\t"                         \
    "jnz L_End_%=\n\t"                                  \
    "add %[itr], 32\n\t"                                \
    "jnz L_Loop_%=\n\t"                                 \
    ops_                                                \
    "mov %[res], 1\n\t"                                 \
"L_End_%=:"                                             \
:   [tmp]"=&x"(yTmp),                                   \
    [itr]"+&r"(uIter = -(kLength >> 2 << 5)),           \
    [res]"+&r"(bRes = false)                            \
:   [dat]"r"(pData + (kLength >> 2 << 2))               \
:   "cc"                                                \
);                                                      \
return bRes;

#define VMP_VU64_ISZ_ASM_OPR(off_)                      \
    "test qword ptr[%[dat] + " off_ " * 8], -1\n\t"     \
    "jnz L_End_%=\n\t"

namespace vmp::intrin {
    template<U64 kLength> inline
    bool Vu64IsZero(const U64 *pData) noexcept {
        constexpr U64 uLenMod = kLength & 3;
        if constexpr (uLenMod == 3) {
            VMP_VU64_ISZ_ASM_VEC(
                VMP_VU64_ISZ_ASM_OPR("0")
                VMP_VU64_ISZ_ASM_OPR("1")
                VMP_VU64_ISZ_ASM_OPR("2")
            );
        }
        else if constexpr (uLenMod == 2) {
            VMP_VU64_ISZ_ASM_VEC(
                VMP_VU64_ISZ_ASM_OPR("0")
                VMP_VU64_ISZ_ASM_OPR("1")
            );
        }
        else if constexpr (uLenMod == 1) {
            VMP_VU64_ISZ_ASM_VEC(
                VMP_VU64_ISZ_ASM_OPR("0")
            );
        }
        else {
            VMP_VU64_ISZ_ASM_VEC(
                ""
            );
        }
    }
    template<> inline
    bool Vu64IsZero<3>(const U64 *pData) noexcept {
        VMP_VU64_ISZ_ASM_QWD(
            VMP_VU64_ISZ_ASM_OPR("0")
            VMP_VU64_ISZ_ASM_OPR("1")
            VMP_VU64_ISZ_ASM_OPR("2")
        );
    }
    template<> inline
    bool Vu64IsZero<2>(const U64 *pData) noexcept {
        VMP_VU64_ISZ_ASM_QWD(
            VMP_VU64_ISZ_ASM_OPR("0")
            VMP_VU64_ISZ_ASM_OPR("1")
        );
    }
    template<> inline
    bool Vu64IsZero<1>(const U64 *pData) noexcept {
        VMP_VU64_ISZ_ASM_QWD(
            VMP_VU64_ISZ_ASM_OPR("0")
        );
    }
    template<> constexpr
    bool Vu64IsZero<0>(const U64 *) noexcept {
        return true;
    }
}

#endif // ifndef VMP_INTRIN_VU64_ISZERO_HPP_

