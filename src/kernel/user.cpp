#include <scriptos/kernel/io.hpp>
#include <scriptos/kernel/user.hpp>

extern "C" USERFN void user_main()
{
    user_print("====================\n");
    user_print("= Hello from User! =\n");
    user_print("====================\n");

    LOOP();
}

USERFN void user_print(cstr string)
{
    syscall(0x10, string);
}
