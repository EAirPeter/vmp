#include "Unsigned.hpp"

#include <iostream>

int main() {
    using namespace std;
    using U1024 = vmp::Unsigned<16>;
    using U2048 = vmp::Unsigned<32>;
    using U4096 = vmp::Unsigned<64>;
    U1024 u1(20), u2(30);
    U2048 v1(30), v2(40);
    U4096 w1(40), w2(50);
    cout << !!u1 << endl;
    cout << (u1 < u2) << endl;
    cout << (v1 < (U2048) u2) << endl;
    cout << (w2 <= (U4096) u1) << endl;
    cout << ((U4096) v2 == w1) << endl;
    cout << ((U4096) (u1 + u2) == w2) << endl;
    cout << (v2 - (U2048) u1 != (U2048) u1) << endl;
    cout << ((U2048) w1 * (U2048) u1 == (U2048) ((U1024) w2 - u2) * v2) << endl;
    return 0;
}

