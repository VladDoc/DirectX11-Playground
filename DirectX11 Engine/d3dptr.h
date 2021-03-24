#pragma once

#include "util.h"

template<typename T>
class D3DPtr
{
    T* data;
    void delete_(T* t)
    {
        util::safeRelease(t);
    }
public:
    D3DPtr() noexcept : data(nullptr) {}

    explicit D3DPtr(T* data) noexcept : data(data) {}
    
    ~D3DPtr()
    {
        delete_(data);
    }

    D3DPtr(std::nullptr_t) noexcept : data(nullptr) {}
    
    D3DPtr& operator=(std::nullptr_t) 
    {
        reset();
        return *this;
    }

    D3DPtr(D3DPtr&& moving) noexcept
    {
        moving.swap(*this);
    }

    D3DPtr& operator=(D3DPtr&& moving) noexcept
    {
        moving.swap(*this);
        return *this;
    }

    template<typename U>
    D3DPtr(D3DPtr<U>&& moving)
    {
        D3DPtr<T> tmp(moving.release());
        tmp.swap(*this);
    }

    template<typename U>
    D3DPtr& operator=(D3DPtr<U>&& moving)
    {
        D3DPtr<T> tmp(moving.release());
        tmp.swap(*this);
        return *this;
    }

    // Remove copy semantics.
    D3DPtr(D3DPtr const&) = delete;
    D3DPtr& operator=(D3DPtr const&) = delete;

    T* operator->() const { return data; }
    T& operator*()  const { return *data; }

    T* get() const { return data; }
    
    T* const* addressof() const { return &data; }
    T** addressof() { return &data; }

    T** operator& () { return &data; }
    T* const* operator& () const { return &data; }

    explicit operator bool() const { return data; }

    T* release() noexcept
    {
        T* result = nullptr;
        std::swap(result, data);
        return result;
    }

    void swap(D3DPtr& src) noexcept
    {
        std::swap(data, src.data);
    }
    void reset()
    {
        T* tmp = release();
        delete_(tmp);
    }

    void reset(T* t)
    {
        reset();
        data = t;
    }
};

template<typename T>
void swap(D3DPtr<T>& lhs, D3DPtr<T>& rhs)
{
    lhs.swap(rhs);
}