#include "../intrin/Vu64Compare.hpp"
#include "_test.h"

using namespace vmp;

template<U64 N>
constexpr int U64Cmp1(const U64 *pLhs, const U64 *pRhs) noexcept {
    return vmp::intrin::Vu64Compare<N>(pLhs, pRhs);
}

template<U64 N>
constexpr int U64Cmp2(const U64 *pLhs, const U64 *pRhs) noexcept {
    auto y0 = _mm256_setzero_si256();
    for (auto i = N - 4; i < N; i -= 4) {
        auto yVal = _mm256_castps_si256(_mm256_xor_ps(
            _mm256_castsi256_ps(_mm256_loadu_si256((const __m256i *) &pLhs[i])),
            _mm256_castsi256_ps(_mm256_loadu_si256((const __m256i *) &pRhs[i]))
        ));
        if (!_mm256_testc_si256(y0, yVal)) {
            for (U64 j = 3; j < 4; --j)
                if (yVal[j])
                    return pLhs[i + j] < pRhs[i + j] ? -1 : 1;
        }
    }
    for (auto j = N & 3; j--; )
        if (pLhs[j] != pRhs[j])
            return pLhs[j] < pRhs[j] ? -1 : 1;
    return 0;
}

template<U64 N>
constexpr int U64Cmp3(const U64 *pLhs, const U64 *pRhs) {
    for (auto i = N; i--; )
        if (pLhs[i] != pRhs[i])
            return pLhs[i] > pRhs[i] ? 1 : -1;
    return 0;
}

template<U64 N>
int U64Cmp4(const U64 *pLhs, const U64 *pRhs) {
    int iRes = __builtin_memcmp(
        pLhs,
        pRhs,
        N * sizeof(U64)
    );
    return iRes;
}

int sgn(int v) {
    return v > 0 ? 1 : v < 0 ? -1 : 0;
}

constexpr U64 LEN = 18115;//67108864;

int main() {
    auto p1 = (U64 *) malloc(LEN * sizeof(U64));
    auto p2 = (U64 *) malloc(LEN * sizeof(U64));
    auto d1 = tzero;
    auto d2 = tzero;
    auto d3 = tzero;
    auto d4 = tzero;
    for (U64 i = 0; i < 8192; ++i) {
        for (U64 j = 0; j < LEN; ++j)
            p1[j] = p2[j] = rng();
        p1[LEN / 2] = rng();
        auto t1 = tnow();
        int v1 = U64Cmp1<LEN>(p1, p2);
        auto t2 = tnow();
        int v2 = U64Cmp2<LEN>(p1, p2);
        auto t3 = tnow();
        int v3 = U64Cmp3<LEN>(p1, p2);
        auto t4 = tnow();
        int v4 = U64Cmp4<LEN>(p1, p2);
        auto t5 = tnow();
        tdiff(d1, t1, t2);
        tdiff(d2, t2, t3);
        tdiff(d3, t3, t4);
        tdiff(d4, t4, t5);
        if (sgn(v1) != sgn(v3) || sgn(v2) != sgn(v3)) {
            cout << "FAIL: " << v1 << ' ' << v2 << ' ';
            cout << v3 << ' ' << v4 << endl;
            return 0;
        }
    }
    cout << "U64Cmp1: " << d1 << endl;
    cout << "U64Cmp2: " << d2 << endl;
    cout << "U64Cmp3: " << d3 << endl;
    cout << "U64Cmp4: " << d4 << endl;
    return 0;
}

