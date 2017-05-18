#ifndef TEST_H_
#define TEST_H_

#include <windows.h>
#include <random>
#include <iostream>

using namespace std;

__inline__ __attribute__((always_inline))
LARGE_INTEGER tfreq(void) {
    LARGE_INTEGER li;
    QueryPerformanceFrequency(&li);
    return li;
}

__inline__ __attribute__((always_inline))
LARGE_INTEGER tnow(void) {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return li;
}

static constexpr LARGE_INTEGER tzero {};
static const LARGE_INTEGER tfreq_ = tfreq();

static mt19937_64 rng((unsigned long long) tnow().QuadPart);

__inline__ __attribute__((always_inline))
void tdiff(LARGE_INTEGER &li, const LARGE_INTEGER &st, const LARGE_INTEGER &ed) {
    li.QuadPart += ed.QuadPart - st.QuadPart;
}

__inline__ __attribute__((always_inline))
ostream &operator <<(ostream &os, const LARGE_INTEGER &li) {
    return os << (1000000LL * li.QuadPart / tfreq_.QuadPart) << " us";
}

#endif // ifndef TEST_H_
