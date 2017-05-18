#include "../intrin/Vu64Add.hpp"
#include "_test.h"

using namespace vmp;

template<U64 N>
void U64Add1(U64 *C, const U64 *A, const U64 *B) {
    intrin::Vu64Add<N>(C, A, B);
}

template<U64 N>
void U64Add2(U64 *C, const U64 *A, const U64 *B) {
    U128 x = 0;
    for (U64 i = 0; i < N; ++i) {
        x += (U128) A[i] + B[i];
        C[i] = (U64) x;
        x >>= 64ULL;
    }
}

constexpr U64 LEN = 2377;

int main() {
    auto p1 = (U64 *) malloc(LEN * sizeof(U64));
    auto p2 = (U64 *) malloc(LEN * sizeof(U64));
    auto pa = (U64 *) malloc(LEN * sizeof(U64));
    auto pb = (U64 *) malloc(LEN * sizeof(U64));
    auto d1 = tzero;
    auto d2 = tzero;
    for (U64 i = 0; i < 65536; ++i) {
        for (U64 j = 0; j < LEN; ++j) {
            pa[j] = rng();
            pb[j] = rng();
        }
        auto t1 = tnow();
        U64Add1<LEN>(p1, pa, pb);
        auto t2 = tnow();
        U64Add2<LEN>(p2, pa, pb);
        auto t3 = tnow();
        tdiff(d1, t1, t2);
        tdiff(d2, t2, t3);
        if (memcmp(p1, p2, LEN * sizeof(U64))) {
            cout << "FAIL" << endl;
            cout << p1[1] << endl;
            cout << p2[1] << endl;
            return 0;
        }
    }
    cout << "U64Add1: " << d1 << endl;
    cout << "U64Add2: " << d2 << endl;
    return 0;
}

