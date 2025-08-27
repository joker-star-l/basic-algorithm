#pragma once
#include <atomic>
#include <functional>
#include <algorithm>
#include <stdexcept>

template<typename T>
class SharedPtr;

template<typename T>
class SharedPtrImpl {
public:
    SharedPtrImpl(T* ptr) : ptr_(ptr), count_(1) {}
    
    ~SharedPtrImpl() {
        delete ptr_;
    }
    
    bool release() {
        return --count_ == 0;
    }
    
    void hold() {
        ++count_;
    }

private:
    friend class SharedPtr<T>;
    T* ptr_;
    std::atomic<uint> count_;
};

template<typename T>
class SharedPtr {
public:
    SharedPtr(T* ptr = nullptr) : impl_(new SharedPtrImpl<T>(ptr)) {}

    ~SharedPtr() {
        if (impl_ && impl_->release()) {
            delete impl_;
        }
    }
    
    SharedPtr(const SharedPtr& other) : impl_(other.impl_) { // 拷贝构造
        if (impl_) {
            impl_->hold();
        }
    }

    SharedPtr(SharedPtr&& other) : impl_(other.impl_) { // 移动构造
        other.impl_ = nullptr;
    }

    void swap(SharedPtr<T>& other) {
        std::swap(impl_, other.impl_); // 交换指针
    }

    SharedPtr<T>& operator=(const SharedPtr<T>& other) { // 拷贝赋值
        if (this == &other) {
            return *this; // 自赋值检查
        }
        SharedPtr<T> tmp(other); // 使用拷贝构造创建临时对象
        swap(tmp);
        return *this;
    }

    SharedPtr<T>& operator=(SharedPtr<T>&& other) { // 移动赋值
        if (this == &other) {
            return *this; // 自赋值检查
        }
        SharedPtr<T> tmp(std::move(other)); // 使用移动构造创建临时对象
        swap(tmp);
        return *this;
    }

    T* operator->() const {
        return impl_ ? impl_->ptr_ : nullptr;
    }

    T* get() const {
        return impl_ ? impl_->ptr_ : nullptr;
    }

    T& operator*() const {
        if (!impl_ || !impl_->ptr_) {
            throw std::runtime_error("Dereferencing null pointer");
        }
        return *(impl_->ptr_);
    }

    void reset(T* ptr) {
        if (impl_ && impl_->release()) {
            delete impl_;
        }
        impl_ = new SharedPtrImpl<T>(ptr);
    }

    uint use_count() const {
        return impl_ ? impl_->count_.load() : 0u;
    }

private:
    SharedPtrImpl<T>* impl_;
};
