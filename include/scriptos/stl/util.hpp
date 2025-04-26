#pragma once

#include <scriptos/std/types.hpp>

template<typename T>
struct remove_reference
{
    using type = T;
};

template<typename T>
struct remove_reference<T&>
{
    using type = T;
};

template<typename T>
struct remove_reference<T&&>
{
    using type = T;
};

template<typename T>
constexpr typename remove_reference<T>::type&& move(T&& t) noexcept
{
    return static_cast<typename remove_reference<T>::type&&>(t);
}

template<typename T>
struct is_lvalue_reference
{
    static constexpr bool value = false;
};

template<typename T>
struct is_lvalue_reference<T&>
{
    static constexpr bool value = true;
};

template<typename T>
constexpr T&& forward(typename remove_reference<T>::type& t) noexcept
{
    return static_cast<T&&>(t);
}

template<typename T>
constexpr T&& forward(typename remove_reference<T>::type&& t) noexcept
{
    static_assert(!is_lvalue_reference<T>::value, "forwarding an rvalue as an lvalue");
    return static_cast<T&&>(t);
}

template<typename T>
void swap(T& a, T& b) noexcept
{
    T t = move(a);
    a = move(b);
    b = move(t);
}

template<typename T, typename U>
constexpr T exchange(T& obj, U&& new_value)
{
    T old_value = move(obj);
    obj = forward<U>(new_value);
    return old_value;
}

template<typename T>
T&& declval() noexcept;

template<typename T>
struct add_const
{
    using type = const T;
};

template<typename T>
struct remove_const
{
    using type = T;
};

template<typename T>
struct remove_const<const T>
{
    using type = T;
};

template<typename T, typename U>
struct is_same
{
    static constexpr bool value = false;
};

template<typename T>
struct is_same<T, T>
{
    static constexpr bool value = true;
};

template<bool B, typename T = void>
struct enable_if
{
};

template<typename T>
struct enable_if<true, T>
{
    using type = T;
};

template<typename T>
struct add_lvalue_reference
{
    using type = T&;
};

template<>
struct add_lvalue_reference<void>
{
    using type = void;
};

template<typename T>
struct add_rvalue_reference
{
    using type = T&&;
};

template<>
struct add_rvalue_reference<void>
{
    using type = void;
};

template<typename T>
struct is_void
{
    static constexpr bool value = false;
};

template<>
struct is_void<void>
{
    static constexpr bool value = true;
};

template<typename T, T v>
struct integral_constant
{
    static constexpr T value = v;

    using value_type = T;
    using type = integral_constant;

    constexpr operator value_type() const noexcept { return value; }
};

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template<bool B, typename T, typename F>
struct conditional
{
    using type = T;
};

template<typename T, typename F>
struct conditional<false, T, F>
{
    using type = F;
};

template<typename T>
struct is_array
{
    static constexpr bool value = false;
};

template<typename T>
struct is_array<T[]>
{
    static constexpr bool value = true;
};

template<typename T, usize N>
struct is_array<T[N]>
{
    static constexpr bool value = true;
};

template<typename T>
struct is_function
{
    static constexpr bool value = false;
};

template<typename Ret, typename... Args>
struct is_function<Ret(Args...)>
{
    static constexpr bool value = true;
};

template<typename Ret, typename... Args>
struct is_function<Ret(Args..., ...)>
{
    static constexpr bool value = true;
};

template<typename T>
struct remove_extent
{
    using type = T;
};

template<typename T, usize N>
struct remove_extent<T[N]>
{
    using type = T;
};

template<typename T>
struct remove_extent<T[]>
{
    using type = T;
};

template<typename T>
struct add_pointer
{
    using type = typename remove_reference<T>::type*;
};

template<typename T>
struct remove_cv
{
    using type = T;
};

template<typename T>
struct remove_cv<const T>
{
    using type = T;
};

template<typename T>
struct remove_cv<volatile T>
{
    using type = T;
};

template<typename T>
struct remove_cv<const volatile T>
{
    using type = T;
};

template<typename T>
struct decay
{
    using U = typename remove_reference<T>::type;
    using type = typename conditional<is_array<U>::value, typename remove_extent<U>::type*, typename conditional<is_function<U>::value, typename add_pointer<U>::type, typename remove_cv<U>::type>::type>::type;
};
