#pragma once

#include "Rational.h"

class M {
    Rational valid_;
    Rational inf_;
    std::string visual_;
public:
    void ToVisual() {
        if (inf_ == 0) {
            visual_ = std::to_string(valid_);
        }
        else if (valid_ == 0) {
            if (inf_ == 1) {
                visual_ = "M";
            }
            else if (inf_ == -1) {
                visual_ = "-M";
            }
            else {
                visual_ = std::to_string(inf_) + "M";
            }
        }
        else if (inf_ < 0) {
            if (inf_ == -1) {
                visual_ = std::to_string(valid_) + "-M";
            }
            else {
                visual_ = std::to_string(valid_) + std::to_string(inf_) + "M";
            }
        }
        else {
            if (inf_ == 1) {
                visual_ = std::to_string(valid_) + "+M";
            }
            else {
                visual_ = std::to_string(valid_) + "+" + std::to_string(inf_) + "M";
            }
        }
    }

    std::string GetVisual() {
        ToVisual();
        return visual_;
    }

    Rational GetInf() {
        return inf_;
    }

    void ToM() {
        int index = visual_.find('+');
        if (visual_ == "-M") {
            inf_ = -1;
        }
        else if (index != std::string::npos) {
            valid_ = ToRational(Cutting(visual_, 0, index));
            if (Cutting(visual_, index + 1, visual_.size() - 1).size() == 0) {
                inf_ = 1;
            }
            else {
                inf_ = ToRational(Cutting(visual_, index + 1, visual_.size() - 1));
            }
        }
        else if (Count(visual_, '-') == 2) {
            int index_second_minus = Cutting(visual_, 1, visual_.size()).find('-') + 1;
            valid_ = ToRational(Cutting(visual_, 0, index_second_minus));
            if (visual_[visual_.size() - 2] == '-') {
                inf_ = -1;
            }
            else {
                inf_ = ToRational(Cutting(visual_, index_second_minus, visual_.size() - 1));
            }
        }
        else {
            int index_minus = visual_.find('-');
            int index_plus = visual_.find('+');
            int index_m = visual_.find('M');
            if (index_minus == std::string::npos && index_plus == std::string::npos && index_m != std::string::npos) {
                if (visual_ == "M") {
                    inf_ = 1;
                }
                else {
                    inf_ = ToRational(Cutting(visual_, 0, visual_.size() - 1));
                }
            }
            else if (index_minus != std::string::npos && index_plus == std::string::npos && index_m != std::string::npos) {
                if (visual_[0] != '-') {
                    valid_ = ToRational(Cutting(visual_, 0, index_minus));
                    inf_ = ToRational(Cutting(visual_, index_minus, visual_.size() - 1));
                }
                else {
                    inf_ = ToRational(Cutting(visual_, 0, visual_.size() - 1));
                }
            }
            else if (index_m == std::string::npos) {
                valid_ = ToRational(visual_);
            }
            else if (index_minus != std::string::npos && index_plus != std::string::npos) {
                valid_ = ToRational(Cutting(visual_, 0, index_plus));
                inf_ = ToRational(Cutting(visual_, index_plus + 1, visual_.size() - 1));
            }
        }
    }

    M() : valid_(0), inf_(0) {
    }

    M(int64_t val) : valid_(val), inf_(0) {
    }

    M(Rational val) : valid_(val), inf_(0) {
    }

    M(Rational valid, Rational inf) : valid_(valid), inf_(inf) {
    }

    friend std::istream& operator>>(std::istream& is, M& val) {
        is >> val.visual_;
        val.ToM();
        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, M& val) {
        val.ToVisual();
        os << val.visual_;
        return os;
    }

    M& operator+=(const M& other) {
        valid_ += other.valid_;
        inf_ += other.inf_;
        return *this;
    }

    M& operator-=(const M& other) {
        valid_ -= other.valid_;
        inf_ -= other.inf_;
        return *this;
    }

    M& operator*=(const M& other) {
        if (valid_ != 0 && inf_ == 0 && other.inf_ != 0 && other.valid_ == 0) {
            inf_ = valid_ * other.inf_;
            valid_ = 0;
        }
        else {
            valid_ *= other.valid_;
            inf_ *= other.inf_;
        }
        return *this;
    }

    M& operator/=(const M& other) {
        valid_ /= other.valid_;
        inf_ /= other.valid_;
        return *this;
    }

    M operator+(const M& other) const {
        M res = *this;
        res += other;
        return res;
    }

    M operator-(const M& other) const {
        M res = *this;
        res -= other;
        return res;
    }

    M operator*(const M& other) const {
        M res = *this;
        res *= other;
        return res;
    }

    M operator/(const M& other) const {
        M res = *this;
        res /= other;
        return res;
    }


    bool operator==(const M& other) const {
        return valid_ == other.valid_ && inf_ == other.inf_;
    }

    bool operator!=(const M& other) const {
        return !(valid_ == other.valid_ && inf_ == other.inf_);
    }

    bool operator<(const M& other) const {
        if (inf_ == other.inf_) {
            if (valid_ < other.valid_) {
                return true;
            }
            return false;
        }
        if (inf_ < other.inf_) {
            return true;
        }
        return false;
    }

    bool operator>(const M& other) const {
        if (inf_ == other.inf_) {
            if (valid_ > other.valid_) {
                return true;
            }
            return false;
        }
        if (inf_ > other.inf_) {
            return true;
        }
        return false;
    }

    bool operator>=(const M& other) const {
        if (inf_ == other.inf_) {
            if (valid_ >= other.valid_) {
                return true;
            }
            return false;
        }
        if (inf_ >= other.inf_) {
            return true;
        }
        return false;
    }

    bool operator<=(const M& other) const {
        if (inf_ == other.inf_) {
            if (valid_ <= other.valid_) {
                return true;
            }
            return false;
        }
        if (inf_ <= other.inf_) {
            return true;
        }
        return false;
    }

    M operator-() const {
        M res;
        res.valid_ = -valid_;
        res.inf_ = -inf_;
        return res;
    }
};

namespace std {
    std::string to_string(M m) {
        return m.GetVisual();
    }
}