#ifndef BORDER
#define BORDER
#include <iterator>
#include <iostream>
#include <initializer_list>
#include <compare>
#include <algorithm>
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
    class Vector : public Container<T> {
    private:

        T* data_ = NULL;
        size_t size_ = 0;
        size_t capacity_ = 0;
        void reallocate(size_t new_capacity) {
            if (new_capacity > this->max_size())
                throw length_error("Vector capacity exceeds max size");
            T* new_data_;
            try { new_data_ = new T[new_capacity]; }
            catch (const bad_alloc&) { throw bad_alloc(); }
            capacity_ = new_capacity;
            for (size_t i = 0; i < size_; i++)
                new_data_[i] = data_[i];
            if (data_)
                delete[] data_;
            data_ = new_data_;
        }

    public:

        template <class IterType>
        class Iterator {

        protected:
            friend class Vector;
            IterType* ptr = nullptr;
            Iterator(IterType* ptr) : ptr(ptr) {}

        public:
            Iterator& operator=(const Iterator& other) = default;
            Iterator(const Iterator& other) = default;
            Iterator(Iterator&& other) = default;
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
            Iterator operator+(int n) const { return Iterator(this->ptr + n); }
            Iterator operator-(int n) const { return Iterator(this->ptr - n); }
            size_t operator-(const Iterator& i) const { return this->ptr - i.ptr; }
            bool operator==(const Iterator& other) const { return this->ptr == other.ptr; }
            bool operator<(const Iterator& other) const { return this->ptr < other.ptr; }
            bool operator!=(const Iterator& other) const { return !(*this == other); }
            IterType& operator*() { return *this->ptr; }
        };

        using iterator = Iterator<T>;
        using const_iterator = Iterator<const T>;
        using typename Container<T>::value_type;
        using typename Container<T>::size_type;
        using typename Container<T>::reference;
        using typename Container<T>::const_reference;
        using typename Container<T>::pointer;
        using typename Container<T>::const_pointer;

        Vector() = default;
        Vector(initializer_list<T> init) {
            reserve(init.size());
            for (const auto& item : init)
                data_[size_++] = item;
        }
        Vector(size_type cnt, const_reference val) {
            reserve(cnt);
            size_ = cnt;
            for (size_type i = 0; i < size_; i++)
                data_[i] = val;
        }
        Vector(const Vector& v) {
            reserve(v.size_);
            for (size_type i = 0; i < v.size_; i++)
                data_[i] = v.data_[i];
            size_ = v.size_;
        }
        Vector(Vector&& v) noexcept : data_(v.data_), size_(v.size_), capacity_(v.capacity_) {
            v.data_ = nullptr;
            v.size_ = 0;
            v.capacity_ = 0;
        }
        void print() const noexcept {
            cout << "Size = " << size_ << ", capacity = " << capacity_ << '\n';
            for (size_type i = 0; i < size_; i++)
                cout << data_[i] << ' ';
            cout << "|\n";
        }

        Vector& operator=(const Vector& v) {
            if (*this != v) {
                Vector tmp(v);
                this->swap(tmp);
            }
            return *this;
        }
        ~Vector() {
            size_ = 0;
            capacity_ = 0;
            if (data_)
                delete[] data_;
            data_ = NULL;
        }

        reference operator[](size_type pos) { return data_[pos]; }
        const_reference operator[](size_type pos) const { return data_[pos]; }
        reference at(size_type pos) {
            if (pos >= size_)
                throw out_of_range("Index out of range");
            return data_[pos];
        }
        const_reference at(size_type pos) const {
            if (pos >= size_)
                throw out_of_range("Index out of range");
            return data_[pos];
        }
        reference front() { return data_[0]; }
        const_reference front() const { return data_[0]; }
        reference back() { return data_[size_ - 1]; }
        const_reference back() const { return data_[size_ - 1]; }
        pointer data() { return data_; }
        const_pointer data() const { return data_; }

        bool empty() const noexcept override final { return !size_; }
        size_type size() const noexcept override final { return size_; }
        size_type max_size() const noexcept override final { return (size_type)(-1) / sizeof(T); }
        size_type capacity() const { return capacity_; }
        void reserve(size_type new_capacity) {
            if (new_capacity > this->max_size())
                throw length_error("Vector capacity exceeds max size");
            if (new_capacity > capacity_)
                reallocate(new_capacity);
        }
        void shrink_to_fit() {
            if (size_ < capacity_)
                this->reallocate(size_);
        }

        void clear() {
            for (size_type i = 0; i < size_; i++)
                data_[i].~T();
            size_ = 0;
        }
        void push_back(const_reference val) {
            if (size_ == capacity_) {
                if (!capacity_)
                    this->reallocate(2);
                else
                    this->reallocate(2 * capacity_);
            }
            data_[size_++] = val;
        }
        void push_back(T&& val) {
            if (size_ == capacity_) {
                if (!capacity_)
                    this->reallocate(2);
                else
                    this->reallocate(2 * capacity_);
            }
            data_[size_++] = move(val);
        }
        void pop_back() {
            if (size_) {
                data_[size_ - 1].~T();
                size_--;
            }
        }
        void resize(size_type cnt) {
            if (cnt > size_)
                reserve(cnt);
            else {
                for (size_type i = cnt; i < size_; i++)
                    data_[i].~T();
            }
            size_ = cnt;
        }
        void resize(size_type cnt, const_reference val) {
            if (cnt > size_) {
                reserve(cnt);
                for (size_type i = size_; i < cnt; i++)
                    data_[i] = val;
            }
            else {
                for (size_type i = cnt; i < size_; i++)
                    data_[i].~T();
            }
            size_ = cnt;
        }
        void swap(Vector& v) noexcept {
            std::swap(data_, v.data_);
            std::swap(size_, v.size_);
            std::swap(capacity_, v.capacity_);
        }
        iterator insert(const_iterator pos, const_reference val) {
            size_type index = pos - this->cbegin();
            if (size_ == capacity_) {
                if (!capacity_)
                    this->reallocate(2);
                else
                    this->reallocate(2 * capacity_);
            }
            for (size_type i = size_; i > index; i--)
                data_[i] = move(data_[i - 1]);
            data_[index] = val;
            size_++;
            return iterator(data_ + index);
        }
        iterator insert(const_iterator pos, size_type cnt, const_reference val) {
            size_type index = pos - this->cbegin();
            for (size_type i = 0; i < cnt; i++) {
                this->insert(pos, val);
                pos = this->cbegin() + index;
            }
            return iterator(data_ + index);
        }
        iterator erase(iterator pos) {
            size_type index = pos - this->begin();
            data_[index].~T();
            for (size_type i = index; i < size_ - 1; i++)
                data_[i] = move(data_[i + 1]);
            size_--;
            return iterator(data_ + index);
        }
        iterator erase(iterator first, iterator last) {
            size_type diff = last - first;
            for (size_type i = 0; i < diff; i++)
                this->erase(first);
            return last;
        }
        iterator erase(const_iterator pos) {
            size_type index = pos - this->cbegin();
            data_[index].~T();
            for (size_type i = index; i < size_ - 1; i++)
                data_[i] = move(data_[i + 1]);
            size_--;
            return iterator(data_ + index);
        }
        iterator erase(const_iterator first, const_iterator last) {
            size_type diff = last - first;
            for (size_type i = 0; i < diff; i++)
                this->erase(first);
            return iterator((T*)last.ptr);
        }

        auto operator<=>(const Vector& v) const {
            if (this->size_ != v.size_)
                return this->size_ <=> v.size_;
            for (size_type i = 0; i < this->size_; i++) {
                if (auto cmp = data_[i] <=> v.data_[i]; cmp != 0)
                    return cmp;
            }
            return strong_ordering::equal;
        }
        bool operator==(const Container<T>& other) const override final {
            const Vector& v = dynamic_cast<const Vector&>(other);
            if (v.size() != this->size_)
                return false;
            return (*this <=> v) == 0;
        }
        bool operator!=(const Container<T>& other) const override final {
            return !(*this == other);
        }
        bool operator<(const Vector& other) const { return (*this <=> other) < 0; }
        bool operator<=(const Vector& other) const { return (*this <=> other) <= 0; }
        bool operator>(const Vector& other) const { return (*this <=> other) > 0; }
        bool operator>=(const Vector& other) const { return (*this <=> other) >= 0; }

        iterator begin() noexcept { return iterator(data_); }
        iterator end() noexcept { return iterator(data_ + size_); }
        const_iterator cbegin() const noexcept { return const_iterator(data_); }
        const_iterator cend() const noexcept { return const_iterator(data_ + size_); }
    };
}
#endif