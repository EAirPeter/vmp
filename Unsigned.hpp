#ifndef VMP_UNSIGNED_HPP_
#define VMP_UNSIGNED_HPP_

#include "Common.hpp"
#include "Vu64.hpp"

#include <algorithm>
#include <cstring>
#include <string>
#include <utility>

namespace vmp {
    
    template<U64 kLength>
    class Unsigned {
    public:
        static constexpr U64 LENGTH = kLength;
    public:
        constexpr Unsigned() noexcept = default;
        constexpr Unsigned(const Unsigned &) noexcept = default;
        constexpr Unsigned(Unsigned &&) noexcept = default;
        constexpr Unsigned(U64 uVal) noexcept {
            Assign(uVal);
        }
        template<U64 kValLen>
        constexpr Unsigned(const Unsigned<kValLen> &mpuVal) noexcept {
            constexpr auto uLenMin = LENGTH < kValLen ? LENGTH : kValLen;
            intrin::Vu64Copy<uLenMin>(
                x_vData,
                mpuVal.GetData()
            );
            intrin::Vu64SetZero<LENGTH - uLenMin>(x_vData + uLenMin);
        }
    public:
        // Cast - Operator
    #define VMP_MPU_CASTU(type_, mask_)                         \
        constexpr explicit operator type_() const noexcept {    \
            return x_vData[0] & mask_;                          \
        }
        VMP_MPU_CASTU(U8 , 0x00000000000000ff)
        VMP_MPU_CASTU(U16, 0x000000000000ffff)
        VMP_MPU_CASTU(U32, 0x00000000ffffffff)
        VMP_MPU_CASTU(U64, 0xffffffffffffffff)
        constexpr explicit operator bool() const noexcept {
            return !intrin::Vu64IsZero<LENGTH>(x_vData);
        }
    #undef VMP_MPU_CASTU
    public:
        // Assignment - Operator
        constexpr Unsigned &
        operator =(const Unsigned &) noexcept = default;
        constexpr Unsigned &
        operator =(Unsigned &&) noexcept = default;
        constexpr Unsigned &
        operator =(U64 uVal) noexcept {
            return Assign(uVal);
        }
        // Assignment - Function
        constexpr Unsigned &&
        Assign(const Unsigned &mpuRhs) noexcept {
            return *this = mpuRhs;
        }
        constexpr Unsigned &
        Assign(Unsigned &&mpuRhs) noexcept {
            return *this = mpuRhs;
        }
        constexpr Unsigned &
        Assign(U64 uVal) noexcept {
            x_vData[0] = uVal;
            intrin::Vu64SetZero<LENGTH - 1>(x_vData + 1);
            return *this;
        }
    public:
        // Comparation - Operator
    #define VMP_MPU_COMPARE(op)                                 \
        constexpr bool                                          \
        operator op(const Unsigned &mpuRhs) const noexcept {    \
            return CompareTo(mpuRhs) op 0;                      \
        }
        VMP_MPU_COMPARE(==)
        VMP_MPU_COMPARE(!=)
        VMP_MPU_COMPARE(< )
        VMP_MPU_COMPARE(> )
        VMP_MPU_COMPARE(<=)
        VMP_MPU_COMPARE(>=)
    #undef VMP_MPU_COMPARE
        // Comparation - Function
        constexpr int
        CompareTo(const Unsigned &mpuRhs) const noexcept {
            return intrin::Vu64Compare<LENGTH>(x_vData, mpuRhs.x_vData);
            for (U64 i = 0; i < LENGTH; ++i)
                if (x_vData[i] != mpuRhs.x_vData[i])
                    return x_vData[i] < mpuRhs.x_vData[i] ? -1 : 1;
            return 0;
        }
    public:
        // Arithmetic - Operator
    #define VMP_MPU_ARITH(op, fn)                               \
        constexpr Unsigned                                      \
        operator op(const Unsigned &mpuRhs) const noexcept {    \
            return fn(mpuRhs);                                  \
        }
        VMP_MPU_ARITH(+, Add)
        VMP_MPU_ARITH(-, Sub)
        VMP_MPU_ARITH(*, Mul)
        VMP_MPU_ARITH(/, Div)
        VMP_MPU_ARITH(%, Mod)
    #undef VMP_MPU_ARITH
        inline Unsigned
        Add(const Unsigned &mpuRhs) const noexcept {
            Unsigned mpuRes;
            intrin::Vu64Add<LENGTH>(mpuRes.x_vData, x_vData, mpuRhs.x_vData);
            return mpuRes;
        }
        inline Unsigned
        Sub(const Unsigned &mpuRhs) const noexcept {
            Unsigned mpuRes;
            intrin::Vu64Sub<LENGTH>(mpuRes.x_vData, x_vData, mpuRhs.x_vData);
            return mpuRes;
        }
        inline Unsigned
        Mul(const Unsigned &mpuRhs) const noexcept {
            Unsigned mpuRes;
            intrin::Vu64Mul<LENGTH>(mpuRes.x_vData, x_vData, mpuRhs.x_vData);
            return mpuRes;
        }
        constexpr Unsigned
        Div(const Unsigned &mpuRhs) const noexcept {
            return Unsigned(0);
        }
        constexpr Unsigned
        Mod(const Unsigned &mpuRhs) const noexcept {
            return Unsigned(0);
        }
        // Arithmetic - Function
    public:
        // Compound Assignment - Operator
        constexpr Unsigned &
        operator +=(const Unsigned &mpuRhs) noexcept {
            intrin::Vu64Add<LENGTH>(x_vData, x_vData, mpuRhs.x_vData);
            return *this;
        }
        constexpr Unsigned &
        operator -=(const Unsigned &mpuRhs) noexcept {
            intrin::Vu64Sub<LENGTH>(x_vData, x_vData, mpuRhs.x_vData);
            return *this;
        }
        constexpr Unsigned &
        operator *=(const Unsigned &mpuRhs) noexcept {
            return *this = Unsigned(*this).Mul(mpuRhs);
        }
    public:
        U64 GetIdxLowBit() const noexcept {
            return 0;
        }
        U64 GetIdxHighBit() const noexcept {
            return 0;
        }
    public:
        const U64 *GetData() const {
            return x_vData;
        }
    private:
        U64 x_vData[LENGTH];
    };
    
}

#endif // ifndef VMP_UNSIGNED_HPP_

