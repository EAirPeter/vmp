#ifndef VMP_INTRIN_VU64_IDXLOWBIT_HPP_
#define VMP_INTRIN_VU64_IDXLOWBIT_HPP_

#include "../Common.hpp"

#define VMP_VU64_ILB_ASM_QWD(ops_)                      \
U64 uRes;                                               \
asm volatile (                                          \
    ops_                                                \
"L_End_%=:"                                             \
:   [res]"+&r"(uRes = 0)                                \
:   [len]"i"(0),                                        \
    [dat]"r"(pData)                                     \
:   "cc"                                                \
);                                                      \
return uRes

#define VMP_VU64_ILB_ASM_VEC(ops_)                      \
__m256i yTmp;                                           \
U64 uIter, uRes;                                        \
asm volatile ("\n"                                      \
"L_Loop_%=:\n\t"                                        \
    "vmovdqu %[tmp], ymmword ptr[%[dat] + %[itr]]\n\t"  \
    "vptest %[tmp], %[tmp]\n\t"                         \
    "jnz L_InYmm_%=\n\t"                                \
    "add %[itr], 32\n\t"                                \
    "jnz L_Loop_%=\n\t"                                 \
    ops_                                                \
    "jmp L_End_%=\n"                                    \
"L_InYmm_%=:\n\t"                                       \
    "test qword ptr[%[dat] + %[itr]], -1\n\t"           \
    "jnz L_Done_%=\n\t"                                 \
    "mov %[res], %[len] + 1\n\t"                        \
    "test qword ptr[%[dat] + %[itr] + 8], -1\n\t"       \
    "jnz L_Done_%=\n\t"                                 \
    "mov %[res], %[len] + 2\n\t"                        \
    "test qword ptr[%[dat] + %[itr] + 16], -1\n\t"      \
    "jnz L_Done_%=\n\t"                                 \
    "mov %[res], %[len] + 3\n\t"                        \
"L_Done_%=:\n\t"                                        \
    "sar %[itr], 3\n\t"                                 \
    "add %[res], %[itr]\n\t"                            \
"L_End_%=:"                                             \
:   [tmp]"=&x"(yTmp),                                   \
    [itr]"+&r"(uIter = -(kLength >> 2 << 5)),           \
    [res]"+&r"(uRes = kLength >> 2 << 2)                \
:   [len]"i"(kLength >> 2 << 2),                        \
    [dat]"r"(pData + (kLength >> 2 << 2))               \
:   "cc"                                                \
);                                                      \
return uRes;

#define VMP_VU64_ILB_ASM_OPR(off_)                      \
    "test qword ptr[%[dat] + " off_ " * 8], -1\n\t"     \
    "jnz L_End_%=\n\t"                                  \
    "mov %[res], %[len] + " off_ " + 1\n\t"             \

namespace vmp::intrin {
    template<U64 kLength> inline
    U64 Vu64IdxLowBit(const U64 *pData) noexcept {
        constexpr U64 uLenMod = kLength & 3;
        if constexpr (uLenMod == 3) {
            VMP_VU64_ILB_ASM_VEC(
                VMP_VU64_ILB_ASM_OPR("0")
                VMP_VU64_ILB_ASM_OPR("1")
                VMP_VU64_ILB_ASM_OPR("2")
            );
        }
        else if constexpr (uLenMod == 2) {
            VMP_VU64_ILB_ASM_VEC(
                VMP_VU64_ILB_ASM_OPR("0")
                VMP_VU64_ILB_ASM_OPR("1")
            );
        }
        else if constexpr (uLenMod == 1) {
            VMP_VU64_ILB_ASM_VEC(
                VMP_VU64_ILB_ASM_OPR("0")
            );
        }
        else {
            VMP_VU64_ILB_ASM_VEC(
                ""
            );
        }
    }
    template<> inline
    U64 Vu64IdxLowBit<4>(const U64 *pData) noexcept {
        VMP_VU64_ILB_ASM_QWD(
            VMP_VU64_ILB_ASM_OPR("0")
            VMP_VU64_ILB_ASM_OPR("1")
            VMP_VU64_ILB_ASM_OPR("2")
            VMP_VU64_ILB_ASM_OPR("3")
        );
    }
    template<> inline
    U64 Vu64IdxLowBit<3>(const U64 *pData) noexcept {
        VMP_VU64_ILB_ASM_QWD(
            VMP_VU64_ILB_ASM_OPR("0")
            VMP_VU64_ILB_ASM_OPR("1")
            VMP_VU64_ILB_ASM_OPR("2")
        );
    }
    
    template<> inline
    U64 Vu64IdxLowBit<2>(const U64 *pData) noexcept {
        VMP_VU64_ILB_ASM_QWD(
            VMP_VU64_ILB_ASM_OPR("0")
            VMP_VU64_ILB_ASM_OPR("1")
        );
    }
    
    template<> inline
    U64 Vu64IdxLowBit<1>(const U64 *pData) noexcept {
        VMP_VU64_ILB_ASM_QWD(
            VMP_VU64_ILB_ASM_OPR("0")
        );
    }
    
    
    template<> inline
    U64 Vu64IdxLowBit<0>(const U64 *) noexcept {
        return 0;
    }
}

#endif // ifndef VMP_INTRIN_VU64_IDXIDXLOWBIT_HPP_

