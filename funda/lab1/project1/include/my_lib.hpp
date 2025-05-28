#ifndef BORDER
#define BORDER
#include <iterator>
#include <iostream>
#include <initializer_list>
#include <compare>
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

    template <class T, size_t N>
    class Array : public Container<T> {
    private:
        T data_[N];

    public:
        template <class IterType>
        class Iterator final {

        protected:
            friend class Array;
            IterType* ptr = nullptr;
            Iterator(IterType* ptr) : ptr(ptr) {}

        public:
            Iterator(const Iterator& other) = default;
            //Iterator(Iterator&& other) = default;
            Iterator& operator++() {
                this->ptr++;
                return *this;
            }
            Iterator operator++(int) {
                Iterator tmp = *this;
                ++this->ptr;
                return tmp;
            }
            Iterator& operator--() {
                this->ptr--;
                return *this;
            }
            Iterator operator--(int) {
                Iterator tmp = *this;
                --this->ptr;
                return tmp;
            }
            Iterator operator+(int n) { return Iterator(this->ptr + n); }
            Iterator operator-(int n) { return Iterator(this->ptr - n); }
            bool operator==(const Iterator& other) const { return this->ptr == other.ptr; }
            bool operator!=(const Iterator& other) const { return this->ptr != other.ptr; }
            bool operator>(const Iterator& other) const { return this->ptr > other.ptr; }
            bool operator>=(const Iterator& other) const { return this->ptr >= other.ptr; }
            bool operator<(const Iterator& other) const { return this->ptr < other.ptr; }
            bool operator<=(const Iterator& other) const { return this->ptr <= other.ptr; }
            IterType& operator*() { return *this->ptr; }
        };

        template <class IterType>
        class RIterator final {

        protected:
            friend class Array;
            IterType* ptr = nullptr;
            RIterator(IterType* ptr) : ptr(ptr) {}

        public:
            RIterator(const RIterator& other) = default;
            //RIterator(RIterator&& other) = default;
            RIterator& operator++() {
                this->ptr--;
                return *this;
            }
            RIterator operator++(int) {
                RIterator tmp = *this;
                --this->ptr;
                return tmp;
            }
            RIterator& operator--() {
                this->ptr++;
                return *this;
            }
            RIterator operator--(int) {
                RIterator tmp = *this;
                ++this->ptr;
                return tmp;
            }
            RIterator operator+(int n) { return RIterator(this->ptr - n); }
            RIterator operator-(int n) { return RIterator(this->ptr + n); }
            bool operator==(const RIterator& other) const { return this->ptr == other.ptr; }
            bool operator!=(const RIterator& other) const { return this->ptr != other.ptr; }
            bool operator>(const RIterator& other) const { return this->ptr > other.ptr; }
            bool operator>=(const RIterator& other) const { return this->ptr >= other.ptr; }
            bool operator<(const RIterator& other) const { return this->ptr < other.ptr; }
            bool operator<=(const RIterator& other) const { return this->ptr <= other.ptr; }
            IterType& operator*() { return *this->ptr; }
        };

        using iterator = Iterator<T>;
        using const_iterator = Iterator<const T>;
        using reverse_iterator = RIterator<T>;
        using const_reverse_iterator = RIterator<const T>;
        using typename Container<T>::value_type;
        using typename Container<T>::size_type;
        using typename Container<T>::reference;
        using typename Container<T>::const_reference;
        using typename Container<T>::pointer;
        using typename Container<T>::const_pointer;

        ~Array() = default;
        Array() = default;
        size_type size() const noexcept override final { return N; }
        size_type max_size() const noexcept override final { return N; }
        bool empty() const noexcept override final { return N == 0; }
        reference operator[](size_type i) {
            if (i >= N) throw out_of_range("Index out of range");
            return data_[i];
        }
        const_reference operator[](size_type i) const {
            if (i >= N) throw out_of_range("Index out of range");
            return data_[i];
        }
        reference front() { return data_[0]; }
        const_reference front() const { return data_[0]; }
        reference back() { return data_[N - 1]; }
        const_reference back() const { return data_[N - 1]; }
        reference at(size_type i) {
            return (*this)[i];
        }
        /*void print() const {
            for (size_type i = 0; i < N; i++)
                cout << data_[i] << ' ';
            cout << '\n';
        }*/
        Array(initializer_list<T> init) {
            if (init.size() > N)
                throw out_of_range("Too many elements for Array");
            size_type i = 0;
            for (auto elem : init)
                data_[i++] = elem;
        }
        Array(const Array& a) {
            for (size_type i = 0; i < N; i++)
                data_[i] = a[i];
        }
        Array(Array&& a) noexcept { move(a.begin(), a.end(), data_); }
        Array& operator=(const Container<T>& other) {
            const Array& a = dynamic_cast<const Array&>(other);
            if (this != &other) {
                for (size_type i = 0; i < N; i++)
                    data_[i] = a[i];
            }
            return *this;
        }
        pointer data() noexcept { return data_; }
        void fill(const_reference val) {
            for (size_type i = 0; i < N; i++)
                data_[i] = val;
        }
        void swap(Array& a) noexcept {
            for (size_type i = 0; i < N; i++) {
                value_type tmp = data_[i];
                data_[i] = a[i];
                a[i] = tmp;
            }
        }

        auto operator<=>(const Array& other) const {
            for (size_type i = 0; i < N; ++i) {
                if (auto cmp = data_[i] <=> other.data_[i]; cmp != 0)
                    return cmp;
            }
            return strong_ordering::equal;
        }
        bool operator==(const Container<T>& other) const override final {
            const Array& a = dynamic_cast<const Array&>(other);
            if (N != a.size())
                return false;
            return (*this <=> a) == 0;
        }
        bool operator!=(const Container<T>& other) const override final {
            const Array& a = dynamic_cast<const Array&>(other);
            if (N != a.size())
                return false;
            return (*this <=> a) != 0;
        }
        bool operator<(const Array& other) const { return (*this <=> other) < 0; }
        bool operator<=(const Array& other) const { return (*this <=> other) <= 0; }
        bool operator>(const Array& other) const { return (*this <=> other) > 0; }
        bool operator>=(const Array& other) const { return (*this <=> other) >= 0; }

        iterator begin() noexcept { return iterator(data_); }
        iterator end() noexcept { return iterator(data_ + N); }
        const_iterator cbegin() const noexcept { return const_iterator(data_); }
        const_iterator cend() const noexcept { return const_iterator(data_ + N); }
        reverse_iterator rbegin() noexcept { return reverse_iterator(data_ + N - 1); }
        reverse_iterator rend() noexcept { return reverse_iterator(data_ - 1); }
        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(data_ + N - 1); }
        const_reverse_iterator crend() const noexcept { return const_reverse_iterator(data_ - 1); }

    };
}
#endif