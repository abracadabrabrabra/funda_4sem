#ifndef BORDER
#define BORDER
#include <iostream>
#include <exception>
#include <algorithm>
#include <iterator>
#include <initializer_list>
#include <limits>

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

    template<class T>
    class List : public Container<T> {
    private:
        class Node {
        public:
            T data;
            Node* next;
            Node* prev;
            Node() : data(T{}), next(nullptr), prev(nullptr) {}
            Node(T& _data) : data(_data), next(nullptr), prev(nullptr) {}
            Node(const T& _data) : data(_data), next(nullptr), prev(nullptr) {}
            Node(const T& _data, const Node*& prev, const Node*& next) : data(_data), next(next), prev(prev) {}
            Node(const T& _data, Node*& prev, Node*& next) : data(_data), next(next), prev(prev) {}
            Node(const T& _data, Node** prev, Node** next) : data(_data) {
                this->prev = (prev == nullptr) ? nullptr : *prev;
                this->next = (next == nullptr) ? nullptr : *next;
            }
            ~Node() = default;
        };

    private:
        Node* _head;
        Node* _tail;
        size_t _size;

        Node* _begin() const noexcept { return _head; }
        const Node* _cbegin() const noexcept { return _head; }
        Node* _end() const noexcept { return _tail->next; }
        const Node* _cend() const noexcept { return _tail->next; }
        Node* _rend() const noexcept { return _begin()->prev; }
        const Node* _crend() const noexcept { return _cbegin()->prev; }
        Node* _rbegin() const noexcept { return _tail; }
        const Node* _crbegin() const noexcept { return _tail; }

    public:

        class Iterator {
        public:
            Node* ptr;
            Iterator() : ptr(nullptr) {}
            Iterator(Node* ptr) : ptr(ptr) {}
            Iterator(const Iterator& itr) : ptr(itr.ptr) {}
            T& operator*() const { return ptr->data; }
            Iterator& operator++() {
                ptr = ptr->next;
                return *this;
            }
            Iterator operator++(int) {
                Iterator tmp = *this;
                this->ptr = this->ptr->next;
                return tmp;
            }
            Iterator& operator--() {
                ptr = ptr->prev;
                return *this;
            }
            Iterator operator--(int) {
                Iterator tmp = *this;
                this->ptr = this->ptr->prev;
                return tmp;
            }
            Iterator operator+(int n) const {
                Node* pptr = ptr;
                for (size_t i = 0; i < n || pptr != nullptr; i++) {
                    pptr = pptr->next;
                }
                return Iterator(pptr);
            }
            Iterator operator-(int n) const {
                Node* pptr = ptr;
                for (size_t i = 0; i < n || pptr != nullptr; i++) {
                    pptr = pptr->prev;
                }
                return Iterator(pptr);
            }
            Iterator& operator+=(int n) {
                for (size_t i = 0; i < n || ptr != nullptr; i++) {
                    ptr = ptr->next;
                }
                return *this;
            }
            Iterator& operator-=(int n) {
                for (size_t i = 0; i < n || ptr != nullptr; i++) {
                    ptr = ptr->prev;
                }
                return *this;
            }

            bool operator==(const Iterator& other) const { return ptr == other.ptr; }
            bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
            std::strong_ordering operator<=> (const Iterator& other) const {
                if (ptr == other.ptr) {
                    return std::strong_ordering::equal;
                }
                else {
                    Node* tmp = ptr;
                    while (tmp != nullptr) {
                        tmp = tmp->next;
                        if (tmp == other) {
                            return std::strong_ordering::greater;
                        }
                    }
                    return std::strong_ordering::less;
                }
            }

        };


        class ConstIterator {
        public:
            const Node* ptr;
            ConstIterator() : ptr(nullptr) {}
            ConstIterator(const Node* ptr) : ptr(ptr) {}
            ConstIterator(Node* ptr) : ptr(ptr) {}
            ConstIterator(const ConstIterator& itr) : ptr(itr.ptr) {}
            const T& operator*() const { return ptr->data; }
            ConstIterator& operator++() {
                ptr = ptr->next;
                return *this;
            }
            ConstIterator operator++(int) {
                ConstIterator tmp = *this;
                this->ptr = this->ptr->next;
                return tmp;
            }
            ConstIterator& operator--() {
                ptr = ptr->prev;
                return *this;
            }
            ConstIterator operator--(int) {
                ConstIterator tmp = *this;
                this->ptr = this->ptr->prev;
                return tmp;
            }
            ConstIterator operator+(int n) const {
                Node* pptr = ptr;
                for (size_t i = 0; i < n || pptr != nullptr; i++) {
                    pptr = pptr->next;
                }
                return ConstIterator(pptr);
            }
            ConstIterator operator-(int n) const {
                Node* pptr = ptr;
                for (size_t i = 0; i < n || pptr != nullptr; i++) {
                    pptr = pptr->prev;
                }
                return ConstIterator(pptr);
            }
            ConstIterator& operator+=(int n) {
                for (size_t i = 0; i < n || ptr != nullptr; i++) {
                    ptr = ptr->next;
                }
                return *this;
            }
            ConstIterator& operator-=(int n) {
                for (size_t i = 0; i < n || ptr != nullptr; i++) {
                    ptr = ptr->prev;
                }
                return *this;
            }

            bool operator==(const ConstIterator& other) const { return ptr == other.ptr; }
            bool operator!=(const ConstIterator& other) const { return ptr != other.ptr; }
            std::strong_ordering operator<=> (const ConstIterator& other) const {
                if (ptr == other.ptr) {
                    return std::strong_ordering::equal;
                }
                else {
                    Node* tmp = ptr;
                    while (tmp != nullptr) {
                        tmp = tmp->next;
                        if (tmp == other) {
                            return std::strong_ordering::greater;
                        }
                    }
                    return std::strong_ordering::less;
                }
            }

            int operator-(const ConstIterator& other) const {
                return ptr - other.ptr;
            }
        };

        class ReverseIterator {
        public:
            Node* ptr;
            ReverseIterator() : ptr(nullptr) {}
            ReverseIterator(Node* ptr) : ptr(ptr) {}
            ReverseIterator(const ReverseIterator& itr) : ptr(itr.ptr) {}
            T& operator*() const { return ptr->data; }
            ReverseIterator& operator++() {
                ptr = ptr->prev;
                return *this;
            }
            ReverseIterator operator++(int) {
                Iterator tmp = *this;
                this->ptr = this->ptr->prev;
                return tmp;
            }
            ReverseIterator& operator--() {
                ptr = ptr->next;
                return *this;
            }
            ReverseIterator operator--(int) {
                Iterator tmp = *this;
                this->ptr = this->ptr->next;
                return tmp;
            }
            ReverseIterator operator+(int n) const {
                Node* pptr = ptr;
                for (size_t i = 0; i < n || pptr != nullptr; i++) {
                    pptr = pptr->prev;
                }
                return ReverseIterator(pptr);
            }
            ReverseIterator operator-(int n) const {
                Node* pptr = ptr;
                for (size_t i = 0; i < n || pptr != nullptr; i++) {
                    pptr = pptr->next;
                }
                return ReverseIterator(pptr);
            }
            ReverseIterator& operator+=(int n) {
                for (size_t i = 0; i < n || ptr != nullptr; i++) {
                    ptr = ptr->prev;
                }
                return *this;
            }
            ReverseIterator& operator-=(int n) {
                for (size_t i = 0; i < n || ptr != nullptr; i++) {
                    ptr = ptr->next;
                }
                return *this;
            }

            bool operator==(const ReverseIterator& other) const { return ptr == other.ptr; }
            bool operator!=(const ReverseIterator& other) const { return ptr != other.ptr; }
            std::strong_ordering operator<=> (const ReverseIterator& other) const {
                if (ptr == other.ptr) {
                    return std::strong_ordering::equal;
                }
                else {
                    Node* tmp = ptr;
                    while (tmp != nullptr) {
                        tmp = tmp->next;
                        if (tmp == other) {
                            return std::strong_ordering::greater;
                        }
                    }
                    return std::strong_ordering::less;
                }
            }

            int operator-(const Iterator& other) const {
                return ptr - other.ptr;
            }
        };

        class ConstReverseIterator {
        public:
            const Node* ptr;
            ConstReverseIterator() : ptr(nullptr) {}
            ConstReverseIterator(const Node* ptr) : ptr(ptr) {}
            ConstReverseIterator(Node* ptr) : ptr(ptr) {}
            ConstReverseIterator(const ConstReverseIterator& itr) : ptr(itr.ptr) {}
            const T& operator*() const { return ptr->data; }
            ConstReverseIterator& operator++() {
                ptr = ptr->prev;
                return *this;
            }
            ConstReverseIterator operator++(int) {
                ConstReverseIterator tmp = *this;
                this->ptr = this->ptr->prev;
                return tmp;
            }
            ConstReverseIterator& operator--() {
                ptr = ptr->next;
                return *this;
            }
            ConstReverseIterator operator--(int) {
                Iterator tmp = *this;
                this->ptr = this->ptr->next;
                return tmp;
            }
            ConstReverseIterator operator+(int n) const {
                Node* pptr = ptr;
                for (size_t i = 0; i < n || pptr != nullptr; i++) {
                    pptr = pptr->prev;
                }
                return ConstReverseIterator(pptr);
            }
            ConstReverseIterator operator-(int n) const {
                Node* pptr = ptr;
                for (size_t i = 0; i < n || pptr != nullptr; i++) {
                    pptr = pptr->next;
                }
                return ConstReverseIterator(pptr);
            }
            ConstReverseIterator& operator+=(int n) {
                for (size_t i = 0; i < n || ptr != nullptr; i++) {
                    ptr = ptr->prev;
                }
                return *this;
            }
            ConstReverseIterator& operator-=(int n) {
                for (size_t i = 0; i < n || ptr != nullptr; i++)
                {
                    ptr = ptr->next;
                }
                return *this;
            }

            bool operator==(const ConstReverseIterator& other) const { return ptr == other.ptr; }
            bool operator!=(const ConstReverseIterator& other) const { return ptr != other.ptr; }
            std::strong_ordering operator<=> (const ConstReverseIterator& other) const {
                if (ptr == other.ptr) {
                    return std::strong_ordering::equal;
                }
                else {
                    Node* tmp = ptr;
                    while (tmp != nullptr) {
                        tmp = tmp->next;
                        if (tmp == other) {
                            return std::strong_ordering::greater;
                        }
                    }
                    return std::strong_ordering::less;
                }
            }

            int operator-(const ConstReverseIterator& other) const {
                return ptr - other.ptr;
            }
        };

    public:

        using iterator = Iterator;
        using const_iterator = ConstIterator;
        using reverse_iterator = ReverseIterator;
        using const_reverse_iterator = ConstReverseIterator;


        List() {
            _head = new Node();
            _tail = _head;
            _size = 0;
        }
        List(const std::initializer_list<T>& initList) {
            _size = initList.size();
            if (_size == 0) {
                _head = new Node();
                _tail = _head;
            }
            else {
                _head = new Node(*initList.begin());
                Node* cur = _head;
                _tail = _head;
                for (auto i = initList.begin() + 1; i != initList.end(); i++) {
                    cur->next = new Node(*i, &cur, nullptr);
                    _tail = cur->next;
                    cur = cur->next;
                }
            }
        }
        List(const List<T>& copy) {
            _head = new Node();
            _tail = _head;
            _size = 0;
            for (auto i = copy.cbegin(); i != copy.cend(); i++) {
                push_back(*i);
            }
        }
        ~List() noexcept override final {
            Node* cur = _head;
            while (cur != nullptr) {
                _head = _head->next;
                delete cur;
                cur = _head;
            }
        }

        reverse_iterator rbegin() const noexcept { return reverse_iterator(_rbegin()); }
        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(_crbegin()); }
        reverse_iterator rend() const noexcept { return reverse_iterator(_rend()); }
        const_reverse_iterator crend() const noexcept { return const_reverse_iterator(_crend()); }
        iterator begin() const noexcept { return iterator(_begin()); }
        iterator end() const noexcept { return iterator(_end()); }
        const_iterator cbegin() const noexcept { return const_iterator(_cbegin()); }
        const_iterator cend() const noexcept { return const_iterator(_cend()); }

        T& front() {
            if (_size > 0) {
                return _head->data;
            }
            throw std::out_of_range("There are no elements!\n");
        }
        T& back() {
            if (_size > 0) {
                return _tail->data;
            }
            throw std::out_of_range("There are no elements!\n");
        }
        bool empty() const noexcept final { return _size == 0 ? true : false; }
        size_t size() const noexcept override final { return _size; }
        size_t max_size() const noexcept override { return std::numeric_limits<size_t>::max() / sizeof(Node); }

        std::strong_ordering operator<=> (const List<T>& other) const {
            if (this->_size > other._size) {
                return std::strong_ordering::greater;
            }
            if (this->_size < other._size) {
                return std::strong_ordering::less;
            }
            for (auto i = this->cbegin(), j = other.cbegin(); i != this->cend() && j != other.cend(); i++, j++) {
                if (*i < *j) {
                    return std::strong_ordering::less;
                }
                else if (*i > *j) {
                    return std::strong_ordering::greater;
                }
            }
            return std::strong_ordering::equal;
        }
        bool operator==(const Container<T>& second) const override final {
            const List<T>* lsec = dynamic_cast<const List<T>*>(&second);
            if (!lsec) return false;
            return (*this <=> *lsec) == std::strong_ordering::equal;
        }
        bool operator!=(const Container<T>& second) const override final { return !(*this == second); }
        bool operator==(const List<T>& second) const { return (*this <=> second) == std::strong_ordering::equal; }
        bool operator!=(const List<T>& second) const { return !(*this == second); }
        bool operator<(const List<T>& other) const { return (*this <=> other) == std::strong_ordering::less; }
        bool operator>(const List<T>& other) const { return (*this <=> other) == std::strong_ordering::greater; }
        bool operator<=(const List<T>& other) const { return !(*this > other); }
        bool operator>=(const List<T>& other) const { return !(*this < other); }

        void clear() {
            if (_size == 0) {
                return;
            }
            while (_tail != _head) {
                _tail = _tail->prev;
                delete _tail->next;
                _tail->next = nullptr;
            }
            _size = 0;
        }
        void insert(iterator position, const T& element) noexcept {
            Node* save = position.ptr;
            position.ptr->prev->next = new Node(element, position.ptr->prev, save);
            if (save != nullptr) {
                save->prev = position.ptr->next;
            }
            ++_size;
            if (position == _tail) {
                _tail = position.ptr;
            }
        }
        void erase(iterator position) noexcept {
            position.ptr->prev->next = position.ptr->next;
            delete position.ptr;
            position.ptr = nullptr;
            --_size;
        }
        void push_back(const T& element) noexcept {
            if (_size == 0) {
                _head->data = element;
            }
            else {
                _tail->next = new Node(element, &_tail, nullptr);
                _tail = _tail->next;
            }
            ++_size;
        }
        void pop_back() noexcept {
            if (_size == 0) {
                std::out_of_range("There are no elements!\n");
            }
            else if (_size == 1) {
                _head = _tail;
            }
            else {
                _tail = _tail->prev;
                delete _tail->next;
                _tail->next = nullptr;
            }
            --_size;
        }
        void push_front(const T& element) noexcept {
            Node* newNode = new Node(element, nullptr, &_head);
            _head->prev = newNode;
            _head = newNode;
            ++_size;
        }
        void pop_front() {
            if (_size == 0) {
                std::out_of_range("There are no elements!\n");
            }
            else if (_size == 1) {
                --_size;
            }
            else {
                _head = _head->next;
                delete _head->prev;
                _head->prev = nullptr;
                --_size;
            }
        }

        void resize(size_t elNumber) {
            if (_size == elNumber) {
                return;
            }
            else if (_size > elNumber) {
                while (_size != elNumber) {
                    _tail = _tail->prev;
                    delete _tail->next;
                    _tail->next = nullptr;
                    --_size;
                }
            }
            else {
                throw std::exception();
            }
        }
        void resize(size_t elNumber, const T& el) {
            if (_size == elNumber) {
                return;
            }
            else if (_size > elNumber) {
                resize(elNumber);
            }
            else {
                while (_size != elNumber) {
                    push_back(el);
                }
            }
        }

        static void swap(List<T>& first, List<T>& second) {
            if (first._size != second._size) {
                return;
            }
            for (auto i = first.begin(), j = second.begin(); i != first.end() && j != second.end(); i++, j++) {
                std::swap(*i, *j);
            }
        }

    };
}
#endif