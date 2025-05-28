#include <iterator>
#include <iostream>
#include <initializer_list>
#include <compare>
#include <algorithm>
#include <utility>
using namespace std;

namespace my_smart_ptr {
    
    template<class T>
    class UniquePtr{
        private:
            T* ptr_ = NULL;
        public:
            constexpr UniquePtr() noexcept = default;
            explicit UniquePtr(T* ptr) noexcept : ptr_(ptr) {}
            UniquePtr(UniquePtr&& other) noexcept: ptr_(other.release()) {}
            ~UniquePtr() {this->reset();}
            
            UniquePtr(const UniquePtr&) = delete;
            UniquePtr& operator=(const UniquePtr&) = delete;
            UniquePtr<T>& operator=(UniquePtr&& other) noexcept{
                if (this != &other)
                    this->reset(other.release());
                return *this;
            }
            
            
            T* get() const noexcept { return ptr_;}
            T* operator->() const noexcept { return ptr_;}
            T& operator*() const { return *ptr_;}
            explicit operator bool() const noexcept { return ptr_ != NULL;}
            
            T* release() noexcept{
                T* old_ptr = ptr_;
                ptr_ = NULL;
                return old_ptr;
            }
            void reset(T* ptr = NULL) noexcept{
                if (ptr == ptr_)
                    return;
                T* old_ptr = ptr_;
                ptr_ = ptr;
                if (old_ptr)
                    delete old_ptr;
            }
            void swap(UniquePtr& other) noexcept {std::swap(ptr_, other.ptr_);}
    };
    
    template<class T>
    class UniquePtr<T[]>{
        private:
            T* ptr_ = NULL;
        public:
            constexpr UniquePtr() noexcept = default;
            explicit UniquePtr(T* ptr) noexcept : ptr_(ptr) {}
            UniquePtr(UniquePtr&& other) noexcept: ptr_(other.release()) {}
            ~UniquePtr() {this->reset();}
            
            UniquePtr(const UniquePtr&) = delete;
            UniquePtr& operator=(const UniquePtr&) = delete;
            UniquePtr<T>& operator=(UniquePtr&& other) noexcept{
                if (this != &other)
                    this->reset(other.release());
                return *this;
            }
            
            
            T* get() const noexcept { return ptr_;}
            T* operator->() const noexcept { return ptr_;}
            T& operator*() const { return *ptr_;}
            T& operator[](size_t i) const{ return *(ptr_ + i);}
            explicit operator bool() const noexcept { return ptr_ != NULL;}
            
            T* release() noexcept{
                T* old_ptr = ptr_;
                ptr_ = NULL;
                return old_ptr;
            }
            void reset(T* ptr = NULL) noexcept{
                if (ptr == ptr_)
                    return;
                T* old_ptr = ptr_;
                ptr_ = ptr;
                if (old_ptr)
                    delete[] old_ptr;
            }
            void swap(UniquePtr& other) noexcept {std::swap(ptr_, other.ptr_);}
    };
}

