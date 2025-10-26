#pragma once

#include <scriptos/types.h>

namespace memory
{
    void Fill(
        void* dst,
        int value,
        usize count);
    void Copy(
        void* dst,
        const void* src,
        usize count);

    bool Compare(
        cstr s1,
        cstr s2,
        usize count);

    template<typename T>
    usize StringLength(const T* value)
    {
        usize length = 0;
        for (auto p = const_cast<T*>(value); *p; ++p, ++length)
            ;
        return length;
    }

    void InitializeHeap(usize size);

    void* Allocate(usize count);

    template<typename T>
    T* Allocate()
    {
        return reinterpret_cast<T*>(Allocate(sizeof(T)));
    }

    template<typename T>
    T* Allocate(usize size)
    {
        return reinterpret_cast<T*>(Allocate(size * sizeof(T)));
    }

    void* Reallocate(
        void* block,
        usize count);

    template<typename T>
    T* Reallocate(T* block)
    {
        return reinterpret_cast<T*>(Reallocate(block, sizeof(T)));
    }

    template<typename T>
    T* Reallocate(
        T* block,
        usize size)
    {
        return reinterpret_cast<T*>(Reallocate(block, size * sizeof(T)));
    }

    void Free(void* block);

    template<typename T>
    T Move(T&& value)
    {
        return value;
    }

    template<typename T>
    void Swap(
        T& a,
        T& b)
    {
        T c = a;
        a = Move(b);
        b = Move(c);
    }

    template<typename T>
    class UniquePtr
    {
    public:
        UniquePtr() = default;

        UniquePtr(T* ptr)
            : m_Ptr(ptr)
        {
        }

        UniquePtr(const UniquePtr& other) = delete;

        UniquePtr& operator=(const UniquePtr& other) = delete;

        UniquePtr(UniquePtr&& other) noexcept
            : m_Ptr(other.m_Ptr)
        {
            other.m_Ptr = nullptr;
        }

        UniquePtr& operator=(UniquePtr&& other) noexcept
        {
            Swap(m_Ptr, other.m_Ptr);
            return *this;
        }

        ~UniquePtr() { Reset(); }

        void Reset()
        {
            Free(m_Ptr);
            m_Ptr = nullptr;
        }

        T* operator->() const { return m_Ptr; }
        T& operator*() const { return *m_Ptr; }

        operator bool() const { return m_Ptr != nullptr; }

    private:
        T* m_Ptr{};
    };

    template<
        typename T,
        typename... Args>
    UniquePtr<T> MakeUnique(Args... args)
    {
        T* ptr = reinterpret_cast<T*>(Allocate(sizeof(T)));
        *ptr = T(args...);
        return UniquePtr(ptr);
    }
}
