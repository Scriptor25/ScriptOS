#include <scriptos/std/vararg.hpp>
#include <scriptos/user/user.hpp>

#define LOOP() asm volatile("jmp .")

extern "C" void user_main()
{
    int a;

    print("====================\n");
    print("= Hello from User! =\n");
    print("====================\n");

    printf("stack address = %p\n", &a);

    LOOP();
}

void print(cstr string)
{
    syscall(0b10000, string);
}

void wprint(cstr string)
{
    syscall(0b10001, string);
}

void printn(cstr string, usize count)
{
    syscall(0b10010, string, count);
}

void wprintn(cstr string, usize count)
{
    syscall(0b10011, string, count);
}

void printf(cstr format, ...)
{
    va_list ap;
    va_start(ap, format);
    syscall(0b10100, format, ap);
    va_end(ap);
}

void wprintf(cstr format, ...)
{
    va_list ap;
    va_start(ap, format);
    syscall(0b10101, format, ap);
    va_end(ap);
}
