#include "../intrin/Vu64Copy.hpp"
#include "_test.h"

using namespace vmp;

template<U64 N>
constexpr void U64Cpy1(U64 *pLhs, const U64 *pRhs) noexcept {
    intrin::Vu64Copy<N>(pLhs, pRhs);
}

template<U64 N>
constexpr void U64Cpy2(U64 *pLhs, const U64 *pRhs) noexcept {
    for (U64 i = N & 3; i < N; i += 4)
        _mm256_storeu_si256(
            (__m256i *) &pLhs[i],
            _mm256_loadu_si256((const __m256i *) &pRhs[i])
        );
    for (U64 j = 0; j < (N & 3); ++j)
        pLhs[j] = pRhs[j];
}

template<U64 N>
constexpr void U64Cpy3(U64 *pLhs, const U64 *pRhs) {
    __builtin_memcpy(
        pLhs,
        pRhs,
        N * sizeof(U64)
    );
}

constexpr U64 LEN = 65536;//67108864;

int main() {
    auto p1 = (U64 *) malloc(LEN * sizeof(U64));
    auto p2 = (U64 *) malloc(LEN * sizeof(U64));
    auto p3 = (U64 *) malloc(LEN * sizeof(U64));
    auto pa = (U64 *) malloc(LEN * sizeof(U64));
    auto d1 = tzero;
    auto d2 = tzero;
    auto d3 = tzero;
    for (U64 i = 0; i < 4096; ++i) {
        for (U64 j = 0; j < LEN; ++j)
            pa[j] = rng();
        auto t1 = tnow();
        U64Cpy1<LEN>(p1, pa);
        auto t2 = tnow();
        U64Cpy2<LEN>(p2, pa);
        auto t3 = tnow();
        U64Cpy3<LEN>(p3, pa);
        auto t4 = tnow();
        tdiff(d1, t1, t2);
        tdiff(d2, t2, t3);
        tdiff(d3, t3, t4);
        if (memcmp(p1, p3, LEN * sizeof(U64)) || memcmp(p2, p3, LEN * sizeof(U64))) {
            cout << "FAIL" << endl;
            return 0;
        }
    }
    cout << "U64Cpy1: " << d1 << endl;
    cout << "U64Cpy2: " << d2 << endl;
    cout << "U64Cpy3: " << d3 << endl;
    return 0;
}

