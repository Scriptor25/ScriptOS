#pragma once

#include <scriptos/types.h>

namespace memory
{
    void Fill(void* dst, int value, usize count);
    void Copy(void* dst, const void* src, usize count);

    template<typename T>
    class UniquePtr
    {
    public:
        template<typename... Args>
        static UniquePtr Create(Args&&... args)
        {
            // TODO: allocate ptr
            T* ptr = nullptr;
            *ptr = T(args...);
            return UniquePtr(ptr);
        }

        UniquePtr()
            : m_Ptr(nullptr)
        {
        }

        UniquePtr(T* ptr)
            : m_Ptr(ptr)
        {
        }

        UniquePtr(const UniquePtr& other) = delete;

        UniquePtr& operator=(const UniquePtr& other) = delete;

        UniquePtr(UniquePtr&& other) noexcept
        {
            m_Ptr = other.m_Ptr;
            other.m_Ptr = nullptr;
        }

        UniquePtr& operator=(UniquePtr&& other) noexcept
        {
            m_Ptr = other.m_Ptr;
            other.m_Ptr = nullptr;
            return *this;
        }

        ~UniquePtr() { Reset(); }

        void Reset()
        {
            if (!m_Ptr)
                return;

            // TODO: free ptr
            m_Ptr = nullptr;
        }

        T* operator->() const { return *m_Ptr; }
        T& operator*() const { return *m_Ptr; }

    private:
        T* m_Ptr;
    };
}
