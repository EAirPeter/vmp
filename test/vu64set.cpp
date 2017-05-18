#include "../intrin/Vu64SetZero.hpp"
#include "_test.h"

using namespace vmp;

template<U64 N>
constexpr void U64Set1(U64 *pData) {
    intrin::Vu64SetZero<N>(pData);
}

template<U64 N>
constexpr void U64Set2(U64 *pData) noexcept {
    auto y0 = _mm256_setzero_si256();
    for (U64 i = N & 3; i < N; i += 4)
        _mm256_storeu_si256((__m256i *) &pData[i], y0);
    for (U64 j = 0; j < (N & 3); ++j)
        pData[j] = 0;
}

template<U64 N>
constexpr void U64Set3(U64 *pData) {
    __builtin_memset(
        pData,
        0,
        N * sizeof(U64)
    );
}

constexpr U64 LEN = 65536;//67108864;

int main() {
    auto p1 = (U64 *) malloc(LEN * sizeof(U64));
    auto p2 = (U64 *) malloc(LEN * sizeof(U64));
    auto p3 = (U64 *) malloc(LEN * sizeof(U64));
    auto d1 = tzero;
    auto d2 = tzero;
    auto d3 = tzero;
    for (U64 i = 0; i < 4096; ++i) {
        for (U64 j = 1; j < LEN; ++j)
            p1[j] = p2[j] = p3[j] = rng();
        p1[rng() % LEN] = rng();
        p2[rng() % LEN] = rng();
        p3[rng() % LEN] = rng();
        auto t1 = tnow();
        U64Set1<LEN>(p1);
        auto t2 = tnow();
        U64Set2<LEN>(p2);
        auto t3 = tnow();
        U64Set3<LEN>(p3);
        auto t4 = tnow();
        tdiff(d1, t1, t2);
        tdiff(d2, t2, t3);
        tdiff(d3, t3, t4);
        if (memcmp(p1, p3, LEN * sizeof(U64)) || memcmp(p1, p3, LEN * sizeof(U64))) {
            cout << "FAIL" << endl;
            return 0;
        }
    }
    cout << "U64Set1: " << d1 << endl;
    cout << "U64Set2: " << d2 << endl;
    cout << "U64Set3: " << d3 << endl;
    return 0;
}

