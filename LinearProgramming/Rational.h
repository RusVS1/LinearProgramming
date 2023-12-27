#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include <numeric>

std::string Cutting(std::string str, int begin, int end) {
    std::string answer = "";
    for (int i = begin; i < end; i++) {
        answer = answer + str[i];
    }
    return answer;
}

class RationalDivisionByZero : public std::runtime_error {
public:
    RationalDivisionByZero() : std::runtime_error("RationalDivisionByZero") {
    }
};

class Rational {
    int64_t numerator_;
    int64_t denominator_;

    void Reduce();

public:
    Rational();
    Rational(int64_t val);
    Rational(int64_t numerator, int64_t denominator);

    void SetNumerator(int64_t val);
    void SetDenominator(int64_t val);

    int64_t GetNumerator() const;
    int64_t GetDenominator() const;

    Rational operator+() const;
    Rational operator-() const;
    Rational& operator+=(const Rational& other);
    Rational& operator-=(const Rational& other);
    Rational& operator/=(const Rational& other);
    Rational& operator*=(const Rational& other);
    Rational operator+(const Rational& other) const;
    Rational operator-(const Rational& other) const;
    Rational operator/(const Rational& other) const;
    Rational operator*(const Rational& other) const;
    Rational& operator++();
    Rational operator++(int);
    Rational& operator--();
    Rational operator--(int);

    bool operator==(const Rational& other) const;
    bool operator!=(const Rational& other) const;
    bool operator<(const Rational& other) const;
    bool operator>(const Rational& other) const;
    bool operator<=(const Rational& other) const;
    bool operator>=(const Rational& other) const;

    friend bool operator>(int64_t val, Rational rational_other);

    friend std::istream& operator>>(std::istream& is, Rational& val);
    friend std::ostream& operator<<(std::ostream& os, const Rational& val);
};
void Rational::Reduce() {
    if (denominator_ == 0) {
        throw RationalDivisionByZero{};
    }
    int64_t a = abs(numerator_);
    int64_t b = abs(denominator_);
    while (a != 0 && b != 0) {
        if (a > b) {
            a = a % b;
        }
        else {
            b = b % a;
        }
    }
    int64_t gcd = a + b;
    numerator_ /= gcd;
    denominator_ /= gcd;
    if (denominator_ < 0) {
        numerator_ = -numerator_;
        denominator_ = -denominator_;
    }
}

Rational::Rational() : numerator_(0), denominator_(1) {
}

Rational::Rational(int64_t val) : numerator_(val), denominator_(1) {
}

Rational::Rational(int64_t numerator, int64_t denominator) : numerator_(numerator), denominator_(denominator) {
    if (denominator_ == 0) {
        throw RationalDivisionByZero{};
    }
    if (denominator_ < 0) {
        numerator_ = -numerator_;
        denominator_ = -denominator_;
    }
    Reduce();
}

void Rational::SetNumerator(int64_t val) {
    this->numerator_ = val;
    Reduce();
}

void Rational::SetDenominator(int64_t val) {
    if (val == 0) {
        throw RationalDivisionByZero{};
    }
    this->denominator_ = val;
    Reduce();
}

int64_t Rational::GetNumerator() const {
    return this->numerator_;
}

int64_t Rational::GetDenominator() const {
    return this->denominator_;
}

Rational Rational::operator+() const {
    return *this;
}

Rational Rational::operator-() const {
    Rational res;
    res.numerator_ = -GetNumerator();
    res.denominator_ = GetDenominator();
    return res;
}

Rational& Rational::operator+=(const Rational& other) {
    numerator_ = other.denominator_ * numerator_ + denominator_ * other.numerator_;
    denominator_ *= other.denominator_;
    Reduce();
    return *this;
}

Rational& Rational::operator-=(const Rational& other) {
    numerator_ = other.denominator_ * numerator_ - denominator_ * other.numerator_;
    denominator_ *= other.denominator_;
    Reduce();
    return *this;
}

Rational& Rational::operator*=(const Rational& other) {
    numerator_ *= other.numerator_;
    denominator_ *= other.denominator_;
    Reduce();
    return *this;
}

Rational& Rational::operator/=(const Rational& other) {
    numerator_ *= other.denominator_;
    denominator_ *= other.numerator_;
    Reduce();
    return *this;
}

Rational Rational::operator+(const Rational& other) const {
    Rational res = *this;
    res += other;
    return res;
}

Rational Rational::operator-(const Rational& other) const {
    Rational res = *this;
    res -= other;
    return res;
}

Rational Rational::operator*(const Rational& other) const {
    Rational res = *this;
    res *= other;
    return res;
}

Rational Rational::operator/(const Rational& other) const {
    Rational res = *this;
    res /= other;
    return res;
}

Rational& Rational::operator++() {
    *this += 1;
    return *this;
}

Rational Rational::operator++(int) {
    Rational res = *this;
    *this += 1;
    return res;
}

Rational& Rational::operator--() {
    *this -= 1;
    return *this;
}

Rational Rational::operator--(int) {
    Rational res = *this;
    *this -= 1;
    return res;
}

bool Rational::operator==(const Rational& other) const {
    return numerator_ == other.numerator_ && denominator_ == other.denominator_;
}

bool Rational::operator!=(const Rational& other) const {
    return !(numerator_ == other.numerator_ && denominator_ == other.denominator_);
}

bool Rational::operator<(const Rational& other) const {
    int first_numerator = numerator_ * other.denominator_;
    int second_numertor = other.numerator_ * denominator_;
    return first_numerator < second_numertor;
}

bool Rational::operator>(const Rational& other) const {
    int first_numerator = numerator_ * other.denominator_;
    int second_numertor = other.numerator_ * denominator_;
    return first_numerator > second_numertor;
}

bool Rational::operator<=(const Rational& other) const {
    int first_numerator = numerator_ * other.denominator_;
    int second_numertor = other.numerator_ * denominator_;
    return first_numerator <= second_numertor;
}

bool Rational::operator>=(const Rational& other) const {
    int first_numerator = numerator_ * other.denominator_;
    int second_numertor = other.numerator_ * denominator_;
    return first_numerator >= second_numertor;
}

bool operator>(int64_t val, Rational rational_other) {
    bool ans = false;
    if (val > rational_other.numerator_ / rational_other.denominator_) {
        ans = true;
    }
    return ans;
}

std::istream& operator>>(std::istream& is, Rational& val) {
    is >> val.numerator_;
    if (is.peek() == '/') {
        is.ignore(1);
        is >> val.denominator_;
    }
    if (val.denominator_ == 0) {
        throw RationalDivisionByZero{};
    }
    val.Reduce();
    return is;
}

std::ostream& operator<<(std::ostream& os, const Rational& val) {
    os << val.numerator_;
    if (val.denominator_ != 1) {
        os << '/' << val.denominator_;
    }
    return os;
}

Rational ToRational(std::string str) {
    int index = str.find('/');
    if (index != std::string::npos) {
        int64_t numerator = std::stoll(Cutting(str, 0, index));
        int64_t denominator = std::stoll(Cutting(str, index + 1, str.size()));
        return Rational(numerator, denominator);
    }
    return std::stoi(str);
}

namespace std {
    std::string to_string(Rational rational) {
        if (rational.GetDenominator() == 1) {
            return std::to_string(rational.GetNumerator());
        }
        return std::to_string(rational.GetNumerator()) + "/" + std::to_string(rational.GetDenominator());
    }
}