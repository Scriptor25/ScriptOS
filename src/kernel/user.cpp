#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/user.hpp>
#include <scriptos/std/vararg.hpp>

extern "C" USER_TEXT NORET void user_main()
{
    int a;

    auto r1 = user_print("====================\n");
    auto r2 = user_print("= Hello from User! =\n");
    auto r3 = user_print("====================\n");

    auto r4 = user_printf("address=%p\n", &a);

    (void)r1;
    (void)r2;
    (void)r3;
    (void)r4;

    for (;;)
        ;
}

USER_TEXT int user_print(cstr string)
{
    return syscall(0b10000, string);
}

USER_TEXT int user_wprint(cstr string)
{
    return syscall(0b10001, string);
}

USER_TEXT int user_printn(cstr string, usize count)
{
    return syscall(0b10010, string, count);
}

USER_TEXT int user_wprintn(cstr string, usize count)
{
    return syscall(0b10011, string, count);
}

USER_TEXT int user_printf(cstr format, ...)
{
    va_list ap;
    va_start(ap, format);
    auto result = syscall(0b10100, format, ap);
    va_end(ap);
    return result;
}

USER_TEXT int user_wprintf(cstr format, ...)
{
    va_list ap;
    va_start(ap, format);
    auto result = syscall(0b10101, format, ap);
    va_end(ap);
    return result;
}
