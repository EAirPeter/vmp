#ifndef VMP_INTRIN_VU64_ISZERO_HPP_
#define VMP_INTRIN_VU64_ISZERO_HPP_



#define VMP_VU64_ISZ_ASM_QWD(len_, ops_)                        \
U64 uTmp;                                                       \
asm volatile (                                                  \
    ops_                                                        \
    "mov %b[qtm], 1\n"                                          \
"jEnd_%=:"                                                      \
:   [qtm]"+&r"(uTmp = 0)                                        \
:   [len]"i"(len_),                                             \
    [dat]"r"(pData)                                             \
:   "cc"                                                        \
);                                                              \
return (bool) uTmp

#define VMP_VU64_ISZ_ASM_VEC(ops_)                              \
__m256i yZero {};                                               \
U64 uIter, uTmp;                                                \
asm volatile ("\n"                                              \
"jLoop_%=:\n\t"                                                 \
    "vptest %[zero], ymmword ptr[%[dat] + %[itr]]\n\t"          \
    "jnc jEnd_%=\n\t"                                           \
    "add %[itr], 32\n\t"                                        \
    "jnz jLoop_%=\n\t"                                          \
    ops_                                                        \
    "mov %b[qtm], 1\n\t"                                        \
"jEnd_%=:"                                                      \
:   [itr]"+&r"(uIter = -(kLength >> 2 << 5)),                   \
    [qtm]"+&r"(uTmp = 0)                                        \
:   [zero]"x"(yZero),                                           \
    [len]"i"(0),                                                \
    [dat]"r"(pData + (kLength >> 2 << 2))                       \
:   "cc"                                                        \
);                                                              \
return (bool) uTmp;

#define VMP_VU64_ISZ_ASM_OPR(off_)                              \
    "test qword ptr[%[dat] + (%[len] + " off_ ") * 8], -1\n\t"  \
    "jnz jEnd_%=\n\t"

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
            3,
            VMP_VU64_ISZ_ASM_OPR("-3")
            VMP_VU64_ISZ_ASM_OPR("-2")
            VMP_VU64_ISZ_ASM_OPR("-1")
        );
    }
    template<> inline
    bool Vu64IsZero<2>(const U64 *pData) noexcept {
        VMP_VU64_ISZ_ASM_QWD(
            2,
            VMP_VU64_ISZ_ASM_OPR("-2")
            VMP_VU64_ISZ_ASM_OPR("-1")
        );
    }
    template<> inline
    bool Vu64IsZero<1>(const U64 *pData) noexcept {
        VMP_VU64_ISZ_ASM_QWD(
            1,
            VMP_VU64_ISZ_ASM_OPR("-1")
        );
    }
    template<> constexpr
    bool Vu64IsZero<0>(const U64 *) noexcept {
        return true;
    }
}

#endif // ifndef VMP_INTRIN_VU64_ISZERO_HPP_

