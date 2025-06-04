.global __enable_fpu
.type __enable_fpu, @function
__enable_fpu:
    mov %cr0,   %rax
    or  $0x20,  %rax
    mov %rax,   %cr0

    mov %cr4,   %rax
    or  $0x600, %rax
    mov %rax,   %cr4

    fninit
    ret
