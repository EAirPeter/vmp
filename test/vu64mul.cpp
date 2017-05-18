#include "../intrin/Vu64Mul.hpp"
#include "_test.h"

using namespace vmp;

template<U64 N>
void U64Mul1(U64 *C, const U64 *A, const U64 *B) {
    intrin::Vu64Mul<N>(C, A, B);
}

template<U64 N>
void U64Mul2(U64 *C, const U64 *A, const U64 *B) {
    memset(C, 0, N * sizeof(U64));
    for (U64 i = 0; i < N; ++i) {
        unsigned __int128 x = 0;
        for (U64 j = 0; j + i < N; ++j) {
            x += (unsigned __int128) A[i] * B[j];
            x += C[i + j];
            C[i + j] = (U64) x;
            x >>= 64ULL;
        }
    }
}

constexpr U64 LEN = 2133;

int main() {
    auto p1 = (U64 *) malloc(LEN * sizeof(U64));
    auto p2 = (U64 *) malloc(LEN * sizeof(U64));
    auto pa = (U64 *) malloc(LEN * sizeof(U64));
    auto pb = (U64 *) malloc(LEN * sizeof(U64));
    auto d1 = tzero;
    auto d2 = tzero;
    for (U64 i = 0; i < 1024; ++i) {
        for (U64 j = 0; j < LEN; ++j) {
            pa[j] = rng();
            pb[j] = rng();
        }
        auto t1 = tnow();
        U64Mul1<LEN>(p1, pa, pb);
        auto t2 = tnow();
        U64Mul2<LEN>(p2, pa, pb);
        auto t3 = tnow();
        tdiff(d1, t1, t2);
        tdiff(d2, t2, t3);
        if (memcmp(p1, p2, LEN * sizeof(U64))) {
            cout << "FAIL" << endl;
            cout << p1[0] << endl;
            cout << p2[0] << endl;
            return 0;
        }
    }
    cout << "U64Mul1: " << d1 << endl;
    cout << "U64Mul2: " << d2 << endl;
    return 0;
}

