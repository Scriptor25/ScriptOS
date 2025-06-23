#pragma once

#include <scriptos/paging.h>
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

    void InitializeHeap(
        paging::PageFrameAllocator& allocator,
        usize size);
    void* Allocate(usize count);
    void* Reallocate(
        void* block,
        usize count);
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
