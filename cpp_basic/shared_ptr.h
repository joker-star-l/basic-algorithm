// copied from https://github.com/yalekseev/shared_ptr/blob/master/shared_ptr.h and modified

#pragma once

#include <functional>
#include <algorithm>
#include <atomic>

namespace util {

namespace impl {

class DefaultDeleter {
public:
    template <typename T>
    void operator()(T* ptr) const {
        delete ptr;
    }
};

class DestructorDeletor {
public:
    template <typename T>
    void operator()(T* ptr) const {
        ptr->~T(); // 仅调用析构函数，不释放内存
    }
};

template <typename T>
class SharedPtrImpl {
public:
    template <typename D = DefaultDeleter>
    SharedPtrImpl(T* ptr = nullptr, D deleter = D()) : ptr_(ptr), deleter_(deleter), counter_(1) {}
    
    template <typename D = DestructorDeletor, typename... Args>
    SharedPtrImpl(T* ptr, D deleter, Args&&... args) : ptr_(ptr), deleter_(deleter), counter_(1) {
        new (ptr) T(std::forward<Args>(args)...); // 在已分配的内存上调用构造函数
    }

    ~SharedPtrImpl() {
        deleter_(ptr_);
    }

    T* ptr() {
        return ptr_;
    }

    void hold() {
        ++counter_;
    }

    uint use_count() const {
        return counter_.load();
    }

    bool release() {
        --counter_;
        return counter_ == 0;
    }
    
private:
    T* ptr_ = nullptr;
    std::function<void(T*)> deleter_;
    std::atomic<uint> counter_;
};

} // namespace impl end

template <typename T>
class SharedPtr {
public:
    explicit SharedPtr(impl::SharedPtrImpl<T>* impl) : impl_(impl) {} // 仅 make_shared 使用

    template <typename D = impl::DefaultDeleter>
    explicit SharedPtr(T* ptr = nullptr, D deleter = D()) : impl_(new impl::SharedPtrImpl<T>(ptr, deleter)) {}
    
    template <typename U, typename D = impl::DefaultDeleter> // 允许从 U* 转换
    explicit SharedPtr(U* ptr = nullptr, D deleter = D()) : impl_(new impl::SharedPtrImpl<T>(ptr, deleter)) {}

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

    template <typename U>
    SharedPtr<T>& operator=(const SharedPtr<U> other) { // 允许从 SharedPtr<U> 转换
        SharedPtr<T> tmp(other); // 使用拷贝构造创建临时对象
        swap(tmp);
        return *this;
    }

    T* operator->() const {
        return impl_->ptr();
    }

    T& operator*() const {
        return *(impl_->ptr());
    }

    T* get() const {
        return impl_->ptr();
    }

    operator bool() const {
        return impl_->ptr() != nullptr;
    }

    template <typename D = impl::DestructorDeletor>
    void reset(T* ptr = nullptr, D deleter = D()) {
        if (impl_->release()) {
            delete impl_;
        }
        impl_ = new impl::SharedPtrImpl<T>(ptr, deleter);
    }

    uint use_count() const {
        return impl_->use_count();
    }

private:
    impl::SharedPtrImpl<T>* impl_;
};

template <typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args) {
    char* ptr = static_cast<char*>(operator new (sizeof(T) + sizeof(impl::SharedPtrImpl<T>))); // 只分配内存，不调用构造函数
    if (ptr == nullptr) {
        throw std::bad_alloc();
    }

    auto t_ptr = reinterpret_cast<T*>(ptr + sizeof(impl::SharedPtrImpl<T>)); // t_ptr 有偏移，无法使用 delete 或 operator delete 删除

    try {
        auto impl = new (ptr) impl::SharedPtrImpl<T>(t_ptr, impl::DestructorDeletor(), std::forward<Args>(args)...);
        return SharedPtr<T>(impl);
    } catch(...) {
        operator delete(ptr); // 如果构造函数抛出异常，释放内存
        throw;
    }
}

} // namespace util end