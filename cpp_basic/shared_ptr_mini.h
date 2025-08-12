#pragma once
#include <atomic>
#include <functional>
#include <algorithm>

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
        if (impl_->release()) {
            delete impl_;
        }
    }
    
    SharedPtr(const SharedPtr& other) : impl_(other.impl_) { // 拷贝构造
        impl_->hold();
    }

    void swap(SharedPtr<T>& other) {
        std::swap(impl_, other.impl_); // 交换指针
    }

    SharedPtr<T>& operator=(const SharedPtr<T>& other) { // 拷贝赋值
        SharedPtr<T> tmp(other); // 使用拷贝构造创建临时对象
        swap(tmp);
        return *this;
    }

    T* operator->() const {
        return impl_->ptr_;
    }

    T* get() const {
        return impl_->ptr_;
    }

    T& operator*() const {
        return *(impl_->ptr_);
    }

    void reset(T* ptr) {
        if (impl_->release()) {
            delete impl_;
        }
        impl_ = new SharedPtrImpl<T>(ptr);
    }

    uint use_count() const {
        return impl_->count_;
    }

private:
    SharedPtrImpl<T>* impl_;
};
