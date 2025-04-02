#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/user.hpp>
#include <scriptos/std/vararg.hpp>

extern "C" USER_TEXT NORET void user_main()
{
    int a;

    user_print("====================\n");
    user_print("= Hello from User! =\n");
    user_print("====================\n");

    user_printf("address=%p\n", &a);

    for (;;)
        ;
}

USER_TEXT void user_print(cstr string)
{
    syscall(0b10000, string);
}

USER_TEXT void user_wprint(cstr string)
{
    syscall(0b10001, string);
}

USER_TEXT void user_printn(cstr string, usize count)
{
    syscall(0b10010, string, count);
}

USER_TEXT void user_wprintn(cstr string, usize count)
{
    syscall(0b10011, string, count);
}

USER_TEXT void user_printf(cstr format, ...)
{
    va_list ap;
    va_start(ap, format);
    syscall(0b10100, format, ap);
    va_end(ap);
}

USER_TEXT void user_wprintf(cstr format, ...)
{
    va_list ap;
    va_start(ap, format);
    syscall(0b10101, format, ap);
    va_end(ap);
}
