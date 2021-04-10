#include "math/numberformat.hpp"

#include "math/fractiontest.hpp"

using namespace FractionTest;

std::string getRoundingFormatChar(mpfr_rnd_t mode) {
    switch (mode) {
        default:
        case MPFR_RNDN:
            return "N";
        case MPFR_RNDZ:
            return "Z";
        case MPFR_RNDU:
            return "U";
        case MPFR_RNDD:
            return "D";
        case MPFR_RNDA:
            return "Y";
    }
}

std::string NumberFormat::toDecimal(const ArithmeticType &v, int decimalSpaces) {
    std::string ret = v.toString("%."
                                 + std::to_string(decimalSpaces)
                                 + "R" + getRoundingFormatChar(mpfr::mpreal::get_default_rnd())
                                 + "f");
    if (ret.empty() || ret.find('.') == std::string::npos) {
        return ret;
    } else {
        std::string zeroStrip = ret;
        for (auto it = ret.rbegin(); it != ret.rend(); it++) {
            if (*it == '.') {
                zeroStrip.pop_back();
                break;
            } else if (*it != '0') {
                break;
            } else {
                zeroStrip.pop_back();
            }
        }
        return zeroStrip;
    }
}

std::string NumberFormat::toHex(const ArithmeticType &v, int decimalSpaces) {
    return v.toString("%."
                      + std::to_string(decimalSpaces)
                      + "R" + getRoundingFormatChar(mpfr::mpreal::get_default_rnd())
                      + "a");
}

std::string NumberFormat::toOctal(const ArithmeticType &v, int decimalSpaces) {
    if (v < 0) {
        throw std::runtime_error("Cannot convert negative number to octal");
    } else if (hasFraction(v)) {
        throw std::runtime_error("Cannot convert number with fraction to octal");
    } else if (v > mpfr::mpreal(std::numeric_limits<long long>::max())) {
        throw std::runtime_error("Cannot convert number to octal (Out of range)");
    } else {
        long long val = v.toLLong();
        std::stringstream stream;
        stream << std::oct << val;
        return stream.str();
    }
}

std::string NumberFormat::toBinary(const ArithmeticType &v, int decimalSpaces) {
    if (v < 0) {
        throw std::runtime_error("Cannot convert negative number to binary");
    } else if (hasFraction(v)) {
        throw std::runtime_error("Cannot convert number with fraction to binary");
    } else if (v > mpfr::mpreal(std::numeric_limits<long long>::max())) {
        throw std::runtime_error("Cannot convert number to binary (Out of range)");
    } else {
        std::string ret;
        std::string bitstring = std::bitset<sizeof(long long) * CHAR_BIT>(v.toLLong()).to_string();
        for (size_t i = 0; i < bitstring.size(); i++) {
            if (bitstring.at(i) != '0') {
                return bitstring.substr(i);
            }
        }
        return "0";
    }
}

ArithmeticType NumberFormat::fromDecimal(const std::string &s) {
    return mpfr::mpreal(s);
}

ArithmeticType NumberFormat::fromHex(const std::string &s) {
    return mpfr::mpreal(s, mpfr::mpreal::get_default_prec(), 16);
}

ArithmeticType NumberFormat::fromOctal(const std::string &s) {
    return mpfr::mpreal(s, mpfr::mpreal::get_default_prec(), 8);
}

ArithmeticType NumberFormat::fromBinary(const std::string &s) {
    if (s.size() > 63)
        throw std::runtime_error("Maximum 64 bits can be converted from string");
    return mpfr::mpreal(std::bitset<64>(s).to_ullong());
}