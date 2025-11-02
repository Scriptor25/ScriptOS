#pragma once

#include <scriptos/types.h>

namespace memory
{
    void Fill(
        void* dst,
        u8 value,
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

    template<
        typename T,
        typename... Args>
    T* Allocate(Args&&... args)
    {
        auto p = reinterpret_cast<T*>(Allocate(sizeof(T)));
        *p = T(args...);
        return p;
    }

    template<typename T>
    T* AllocateArray(usize size)
    {
        return reinterpret_cast<T*>(Allocate(size * sizeof(T)));
    }

    template<typename T>
    T* AllocateAligned(usize alignment)
    {
        auto size = sizeof(T);
        auto mask = alignment - 1;
        auto raw = reinterpret_cast<uptr>(Allocate(size + alignment + sizeof(void*)));
        auto aligned = (raw + sizeof(void*) + mask) & ~mask;
        reinterpret_cast<void**>(aligned)[-1] = reinterpret_cast<void*>(raw);
        return reinterpret_cast<T*>(aligned);
    }

    void FreeAligned(void* block);

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
    class UniquePointer
    {
    public:
        UniquePointer() = default;

        UniquePointer(T* ptr)
            : m_Ptr(ptr)
        {
        }

        UniquePointer(const UniquePointer& other) = delete;

        UniquePointer& operator=(const UniquePointer& other) = delete;

        UniquePointer(UniquePointer&& other) noexcept
            : m_Ptr(other.m_Ptr)
        {
            other.m_Ptr = nullptr;
        }

        UniquePointer& operator=(UniquePointer&& other) noexcept
        {
            Swap(m_Ptr, other.m_Ptr);
            return *this;
        }

        ~UniquePointer() { Reset(); }

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
    UniquePointer<T> MakeUnique(Args... args)
    {
        T* ptr = reinterpret_cast<T*>(Allocate(sizeof(T)));
        *ptr = T(args...);
        return UniquePointer(ptr);
    }
}
