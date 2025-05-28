#ifndef BORDER
#define BORDER
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class BigInt{
    
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
        bool less_abs(const BigInt& x) const{
            if (data.size() != x.data.size())
                return (data.size() < x.data.size());
            for (int i = data.size() - 1; i >= 0; i--){
                if (data[i] != x.data[i]) 
                    return (data[i] < x.data[i]);
            }
            return false;
        }
        
    public:
        BigInt() = default;
        BigInt(const BigInt& x) : sign(x.sign), data(x.data) {}
        BigInt(BigInt&& x) noexcept{
            data = move(x.data);
            sign = move(x.sign);
        }
        BigInt(long long x){
            if (x < 0)
                sign = -1;
            x *= sign;
            while (x || data.empty()){
                data.push_back(x % 10);
                x /= 10;
            }
        }
        BigInt(string str){
            if (str.empty())
                return;
            int str_begin = str[0] == '-' ? 1 : 0;
            sign = str[0] == '-' ? -1 : 1;
            for (int i = str.size() - 1; i >= str_begin; i--){
                if (str[i] < '0' || str[i] > '9')
                    throw invalid_argument("Not a number");
                data.push_back(str[i] - '0');
            }
            if (data.empty()){
                data.push_back(0);
                sign = 1;
            }
            remove_lead_zeros();
        }
        vector<unsigned short>& digits() {return data;}
        short& sign_() {return sign;}
        
        BigInt& operator=(const BigInt& x) {
            if (this != &x){
                data = x.data;
                sign = x.sign;
            }
            return *this;
        }
        bool operator==(const BigInt& x) const{
            if (sign != x.sign || data.size() != x.data.size())
                return false;
            for (size_t i = 0; i < data.size(); i++){
                if (data[i] != x.data[i])
                    return false;
            }
            return true;
        }
        bool operator!=(const BigInt& x) const {return !(*this == x);}
        bool operator<(const BigInt& x) const{
            if (sign != x.sign)
                return sign < x.sign;
            if (data.size() != x.data.size())
                return (data.size() < x.data.size()) ^ (sign == -1);
            for (int i = data.size() - 1; i >= 0; i--){
                if (data[i] != x.data[i]) 
                    return (data[i] < x.data[i]) ^ (sign == -1);
            }
            return false;
        }
        bool operator>=(const BigInt& x) const {return !(*this < x);}
        bool operator<=(const BigInt& x) const {return (*this < x) || (*this == x);}
        bool operator>(const BigInt& x) const {return !(*this <= x);}
        bool is_null() const {return data.size() == 1 && data[0] == 0;}
        
        BigInt operator-() const{
            BigInt res = *this;
            if (res != BigInt(0))
                res.sign *= -1;
            return res;
        }
        BigInt operator+(const BigInt& x) const{
            BigInt res;
            if (sign == x.sign){
                res = this->add_abs(x);
                res.sign = sign;
                return res;
            }
            if (this->less_abs(x)){
                res = x.subtr_abs(*this);
                res.sign = (res.is_null()) ? 1: x.sign;
                return res;
            }
            res = this->subtr_abs(x);
            res.sign = (res.is_null()) ? 1: sign;
            return res;
        }
        BigInt& operator+=(const BigInt& x){
            *this = *this + x;
            return *this;
        }
        BigInt operator-(const BigInt& x) const{return *this + (-x);}
        BigInt& operator-=(const BigInt& x){
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
        BigInt& operator*=(const BigInt& x){
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
        BigInt& operator/=(const BigInt& x){
            *this = *this / x;
            return *this;
        }
        
        friend ostream& operator<<(ostream& os, const BigInt& x){
            if (x.sign == -1)
                os << '-';
            for (auto it = x.data.crbegin(); it < x.data.crend(); it++)
                os << *it;
            return os;
        }
        friend istream& operator>>(istream& is, BigInt& x){
            string str;
            is >> str;
            x = BigInt(str);
            return is;
        }
        
        void half(BigInt& jr, BigInt& old) const {
            for (size_t i = 0; i < data.size() / 2; i++)
                jr.data.push_back(data[i]);
            for (size_t i = data.size() / 2; i < data.size(); i++)
                old.data.push_back(data[i]);
            jr.sign = 1;
            old.sign = 1;
        }
};

BigInt karatsuba(const BigInt& a, const BigInt& b){
    BigInt x(a), y(b);
    if (x.digits().size() <= 1 || y.digits().size() <= 1)
        return x * y;
    while(x.digits().size() % 2 == 1 || x.digits().size() < y.digits().size())
        x.digits().push_back(0);
    while(y.digits().size() % 2 == 1 || y.digits().size() < x.digits().size())
        y.digits().push_back(0);
    size_t len = x.digits().size();
    BigInt x_jr, x_old, y_jr, y_old;
    x.half(x_jr, x_old);
    y.half(y_jr, y_old);
    BigInt old_prod = karatsuba(x_old, y_old);
    BigInt jr_prod = karatsuba(x_jr, y_jr);
    BigInt mid_part = karatsuba(x_jr + x_old, y_jr + y_old) - old_prod - jr_prod;
    mid_part.digits().insert(mid_part.digits().begin(), len / 2, 0);
    old_prod.digits().insert(old_prod.digits().begin(), len, 0);
    BigInt res = old_prod + mid_part + jr_prod;
    res.sign_() = x.sign_() * y.sign_();
    return res;
}

#endif