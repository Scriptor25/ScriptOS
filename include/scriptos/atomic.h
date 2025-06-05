#pragma once

#include <scriptos/types.h>

namespace atomic
{
    template<typename T>
    T XCHG(volatile T* dst, T val)
    {
        T pre;
        asm volatile("xchg %0, %1"
                     : "=r"(pre), "+m"(dst)
                     : "0"(val)
                     : "memory");
        return pre;
    }

    template<typename T>
    bool CMPXCHG(volatile T* dst, T exp, T val)
    {
        u8 result;
        asm volatile("lock cmpxchg %2, %1;"
                     "sete %0"
                     : "=q"(result), "+m"(dst), "+a"(exp)
                     : "r"(val)
                     : "memory");
        return result;
    }

    template<typename T>
    void SpinLockAcquire(volatile T* lock)
    {
        while (XCHG(lock, 1))
        {
        }
    }

    template<typename T>
    void SpinLockRelease(volatile T* lock)
    {
        *lock = 0;
    }
}
