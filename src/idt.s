.global __load_idt
.type __load_idt, @function
__load_idt:
    lidt (%rdi)
    ret
