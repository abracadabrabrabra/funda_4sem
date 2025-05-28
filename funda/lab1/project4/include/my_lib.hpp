#ifndef BORDER
#define BORDER
#include <iterator>
#include <iostream>
#include <initializer_list>
#include <compare>
#include <deque>
using namespace std;

template <class T>
class Container {
public:

    using value_type = T;
    using size_type = size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = T*;
    using const_pointer = const T*;

    virtual ~Container() = 0;

    virtual size_type size() const noexcept = 0;
    virtual size_type max_size() const noexcept = 0;
    virtual bool empty() const noexcept = 0;
    virtual bool operator==(const Container& other) const = 0;
    virtual bool operator!=(const Container& other) const = 0;

protected:

    Container() = default;
    Container(const Container&) = default;
    Container& operator=(const Container&) = default;
};

template <class T>
Container<T>::~Container() = default;

namespace my_container {

    template <class T>
    class Stack {
    private:
        deque<T> data_;

    public:
        ~Stack() = default;
        Stack() = default;
        Stack(const Stack& s) : data_(s.data_) {}
        Stack(initializer_list<T> init) : data_(init) {}
        Stack(Stack&& s) noexcept { data_ = move(s.data_); }

        Stack& operator=(const Stack& s) {
            if (this != &s)
                data_ = s.data_;
            return *this;
        }
        Stack& operator=(Stack&& s) noexcept {
            if (this != &s)
                data_ = move(s.data_);
            return *this;
        }

        T& top() { return data_.back(); }
        const T& top() const { return data_.back(); }
        bool empty() const { return data_.empty(); }
        size_t size() const { return data_.size(); }
        size_t max_size() const { return data_.max_size(); }

        void push(const T& val) { data_.push_back(val); }
        void push(T&& val) { data_.push_back(move(val)); }
        void pop() { data_.pop_back(); }
        void swap(Stack& s) noexcept { data_.swap(s.data_); }

        auto operator<=>(const Stack& s) const {
            if (this->size() != s.size())
                return this->size() <=> s.size();
            return data_ <=> s.data_;
        }
        bool operator==(const Stack& s) const {
            return this->size() == s.size() && data_ == s.data_;
        }
        bool operator!=(const Stack& s) const {
            return !(*this == s);
        }
        bool operator>(const Stack& s) const { return *this <=> s > 0; }
        bool operator>=(const Stack& s) const { return *this <=> s >= 0; }
        bool operator<(const Stack& s) const { return *this <=> s < 0; }
        bool operator<=(const Stack& s) const { return *this <=> s <= 0; }
    };

}


#endif