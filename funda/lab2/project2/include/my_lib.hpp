#ifndef H_GRANDFATHER
#define H_GRANDFATHER

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class BigInt {

private:
    short sign = 1;
    vector<unsigned short> data;
    void remove_lead_zeros() {
        while (data.size() > 1 && data.back() == 0)
            data.pop_back();
        if (data.size() == 1 && data[0] == 0)
            sign = 1;
    }
    BigInt add_abs(const BigInt& x) const {
        BigInt res;
        size_t max_len = max(data.size(), x.data.size());
        res.data.resize(max_len + 1, 0);
        unsigned short carry = 0;

        for (size_t i = 0; i < max_len; i++) {
            unsigned short a = i < data.size() ? data[i] : 0;
            unsigned short b = i < x.data.size() ? x.data[i] : 0;
            unsigned short sum = a + b + carry;
            res.data[i] = sum % 10;
            carry = sum / 10;
        }

        if (carry > 0)
            res.data[max_len] = carry;
        res.remove_lead_zeros();
        return res;
    }
    BigInt subtr_abs(const BigInt& x) const {
        BigInt res;
        res.data.resize(data.size(), 0);
        short borrow = 0;

        for (size_t i = 0; i < data.size(); ++i) {
            short a = data[i];
            short b = i < x.data.size() ? x.data[i] : 0;
            short diff = a - b - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            }
            else
                borrow = 0;
            res.data[i] = diff;
        }

        res.remove_lead_zeros();
        return res;
    }
    bool less_abs(const BigInt& x) const {
        if (data.size() != x.data.size())
            return (data.size() < x.data.size());
        for (int i = data.size() - 1; i >= 0; i--) {
            if (data[i] != x.data[i])
                return (data[i] < x.data[i]);
        }
        return false;
    }

public:
    BigInt() = default;
    BigInt(const BigInt& x) : sign(x.sign), data(x.data) {}
    BigInt(BigInt&& x) noexcept {
        data = move(x.data);
        sign = move(x.sign);
    }
    BigInt(long long x) {
        if (x < 0)
            sign = -1;
        x *= sign;
        while (x || data.empty()) {
            data.push_back(x % 10);
            x /= 10;
        }
    }
    BigInt(string str) {
        if (str.empty())
            return;
        int str_begin = str[0] == '-' ? 1 : 0;
        sign = str[0] == '-' ? -1 : 1;
        for (int i = str.size() - 1; i >= str_begin; i--) {
            if (str[i] < '0' || str[i] > '9')
                throw invalid_argument("Not a number");
            data.push_back(str[i] - '0');
        }
        if (data.empty()) {
            data.push_back(0);
            sign = 1;
        }
        remove_lead_zeros();
    }
    vector<unsigned short>& digits() { return data; }
    short sign_() { return sign; }

    BigInt& operator=(const BigInt& x) {
        if (this != &x) {
            data = x.data;
            sign = x.sign;
        }
        return *this;
    }
    bool operator==(const BigInt& x) const {
        if (sign != x.sign || data.size() != x.data.size())
            return false;
        for (size_t i = 0; i < data.size(); i++) {
            if (data[i] != x.data[i])
                return false;
        }
        return true;
    }
    bool operator!=(const BigInt& x) const { return !(*this == x); }
    bool operator<(const BigInt& x) const {
        if (sign != x.sign)
            return sign < x.sign;
        if (data.size() != x.data.size())
            return (data.size() < x.data.size()) ^ (sign == -1);
        for (int i = data.size() - 1; i >= 0; i--) {
            if (data[i] != x.data[i])
                return (data[i] < x.data[i]) ^ (sign == -1);
        }
        return false;
    }
    bool operator>=(const BigInt& x) const { return !(*this < x); }
    bool operator<=(const BigInt& x) const { return (*this < x) || (*this == x); }
    bool operator>(const BigInt& x) const { return !(*this <= x); }
    bool is_null() const { return data.size() == 1 && data[0] == 0; }

    BigInt operator-() const {
        BigInt res = *this;
        if (res != BigInt(0))
            res.sign *= -1;
        return res;
    }
    BigInt operator+(const BigInt& x) const {
        BigInt res;
        if (sign == x.sign) {
            res = this->add_abs(x);
            res.sign = sign;
            return res;
        }
        if (this->less_abs(x)) {
            res = x.subtr_abs(*this);
            res.sign = (res.is_null()) ? 1 : x.sign;
            return res;
        }
        res = this->subtr_abs(x);
        res.sign = (res.is_null()) ? 1 : sign;
        return res;
    }
    BigInt& operator+=(const BigInt& x) {
        *this = *this + x;
        return *this;
    }
    BigInt operator-(const BigInt& x) const { return *this + (-x); }
    BigInt& operator-=(const BigInt& x) {
        *this = *this - x;
        return *this;
    }
    BigInt operator*(const BigInt& x) const {
        if (this->is_null() || x.is_null())
            return BigInt(0);
        BigInt res;
        res.data.resize(data.size() + x.data.size(), 0);

        for (size_t i = 0; i < data.size(); i++) {
            unsigned short carry = 0;
            for (size_t j = 0; j < x.data.size(); j++) {
                unsigned short product = data[i] * x.data[j] + res.data[i + j] + carry;
                res.data[i + j] = product % 10;
                carry = product / 10;
            }
            if (carry > 0)
                res.data[i + x.data.size()] += carry;
        }

        res.sign = sign * x.sign;
        res.remove_lead_zeros();
        return res;
    }
    BigInt& operator*=(const BigInt& x) {
        *this = *this * x;
        return *this;
    }
    BigInt operator/(const BigInt& x) const {
        if (x.is_null())
            throw runtime_error("Division by zero");
        if (this->less_abs(x))
            return BigInt(0);

        BigInt divider = x;
        divider.sign = 1;
        BigInt res, cur;
        res.data.resize(data.size(), 0);

        for (int i = data.size() - 1; i >= 0; i--) {
            cur = cur * BigInt(10) + BigInt(data[i]);
            unsigned short cur_digit = 0;
            while (cur >= divider) {
                cur -= divider;
                cur_digit++;
            }
            res.data[i] = cur_digit;
        }

        res.sign = sign * x.sign;
        res.remove_lead_zeros();
        return res;
    }
    BigInt& operator/=(const BigInt& x) {
        *this = *this / x;
        return *this;
    }

    friend ostream& operator<<(ostream& os, const BigInt& x) {
        if (x.sign == -1)
            os << '-';
        for (auto it = x.data.crbegin(); it < x.data.crend(); it++)
            os << *it;
        return os;
    }
    friend istream& operator>>(istream& is, BigInt& x) {
        string str;
        is >> str;
        x = BigInt(str);
        return is;
    }
};

BigInt mod_n(const BigInt& x, const BigInt& y) { return x - y * (x / y); }
BigInt mod_exp(const BigInt& base, const BigInt& power, const BigInt& mod) {
    if (power.is_null())
        return BigInt(1);
    BigInt k0 = mod_exp(mod_n(base, mod), power / 2, mod);
    if (mod_n(power, BigInt(2)).is_null())
        return mod_n(k0 * k0, mod);
    return mod_n(mod_n(base, mod) * mod_n(k0 * k0, mod), mod);
}

#endif

