#include "../intrin/Vu64IdxLowBit.hpp"
#include "_test.h"

using namespace vmp;

template<U64 N> constexpr
U64 U64Ilb1(const U64 *pData) noexcept {
    return vmp::intrin::Vu64IdxLowBit<N>(pData);
}

template<U64 N> constexpr
U64 U64Ilb2(const U64 *pData) noexcept {
    for (U64 j = 0; j < (N & 3); ++j)
        if (pData[j])
            return j;
    for (U64 i = N & 3; i < N; i += 4) {
        auto yVal = _mm256_loadu_si256((const __m256i *) &pData[i]);
        if (!_mm256_testz_si256(yVal, yVal)) {
            for (U64 j = 0; j < 4; ++j)
                if (pData[i + j])
                    return i + j;
        }
    }
    return N;
}

template<U64 N> constexpr
U64 U64Ilb3(const U64 *pData) noexcept {
    for (U64 i = 0; i < N; ++i)
        if (pData[i])
            return i;
    return N;
}

constexpr U64 LEN = 128355;//67108864;

int main() {
    auto p1 = (U64 *) malloc(LEN * sizeof(U64));
    auto d1 = tzero;
    auto d2 = tzero;
    auto d3 = tzero;
    for (U64 i = 0; i < 4096; ++i) {
        for (U64 j = 0; j < LEN; ++j)
            p1[j] = 0;
        p1[LEN - 4] = rng();
        auto t1 = tnow();
        U64 v1 = U64Ilb1<LEN>(p1);
        auto t2 = tnow();
        U64 v2 = U64Ilb2<LEN>(p1);
        auto t3 = tnow();
        U64 v3 = U64Ilb3<LEN>(p1);
        auto t4 = tnow();
        tdiff(d1, t1, t2);
        tdiff(d2, t2, t3);
        tdiff(d3, t3, t4);
        if (v1 != v3 || v2 != v3) {
            cout << "FAIL: " << v1 << ' ';
            cout << v2 << ' ' << v3 << endl;
            return 0;
        }
    }
    cout << "U64Ilb1: " << d1 << endl;
    cout << "U64Ilb2: " << d2 << endl;
    cout << "U64Ilb3: " << d3 << endl;
    return 0;
}

