#ifndef NUMBERFORMAT_HPP
#define NUMBERFORMAT_HPP

#include <string>
#include <sstream>
#include <climits>
#include <cmath>
#include <limits>
#include <bitset>
#include <iomanip>

#include "math/arithmetictype.hpp"

namespace NumberFormat {
    std::string toDecimal(const ArithmeticType &v, int decimalSpaces, mpfr_rnd_t rounding);

    std::string toHex(const ArithmeticType &v, int decimalSpaces, mpfr_rnd_t rounding);

    std::string toOctal(const ArithmeticType &v, int decimalSpaces, mpfr_rnd_t rounding);

    std::string toBinary(const ArithmeticType &v, int decimalSpaces, mpfr_rnd_t rounding);

    ArithmeticType fromDecimal(const std::string &s, mpfr_rnd_t rounding);

    ArithmeticType fromHex(const std::string &s, mpfr_rnd_t rounding);

    ArithmeticType fromOctal(const std::string &s, mpfr_rnd_t rounding);

    ArithmeticType fromBinary(const std::string &s, mpfr_rnd_t rounding);
}

#endif // NUMBERFORMAT_HPP
