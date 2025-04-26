![ScriptOS Logo](./logo/repository-banner.png)

# ScriptOS - WIP

WARNING: this project is still work in process, and will be aproximatly for the next 20 years or so.

## About

With ScriptOS I try to make my own fully custom x86 compatible Unix-like operating system.

A large part of the system is written in modern C++ (sorry [Linus](https://github.com/torvalds)), but things like the "bootloader" and some others you just simply cannot do in higher level languages are written in assembler.

## Features

Here some goals / features the OS supports / will support:

- [x] Multiboot2 compatible (GRUB bootable)
- [x] Memory mapping
- [x] Memory paging
- [x] Framebuffer drawing
- [x] Kernel Mode ---> User Mode
- [x] Serial IO
- [x] Interrupts and Syscalls
- [x] Dynamic Memory Heap
- [ ] Filesystem
- [ ] Task Switching / Multitasking
- [ ] 64-Bit Long Mode
- [ ] Persistent System
- [ ] Process-Thread-Model
- [ ] Smooth error handling
- [x] Completely panic if something goes wrong

There are many more features and things I'll have to add in the future, but for now I'll work on these.

## Building

### Prerequisites

You'll need to have following packages installed beforehand for ...

... building the toolchain:
- gcc
- curl
- make
- libgmp-dev
- libmpfr-dev
- libmpc-dev
- texinfo

... building the os:
- the toolchain
- make
- grub-common
- grub-pc-bin
- xorriso

... testing the os:
- qemu-system-x86
- (optional for debugging) gdb or gdb-multiarch

I worked on the os in debian 12, so all the packages are available via apt.

### Building the Toolchain

First, you'll need to build the cross-compiler toolchain for the os:

```shell
cd toolchain
make
```

This will download the source code, build it and install the toolchain in `/usr/local`.

### Building the OS

Now you can build the os using the cross-compiler toolchain:

```shell
make clean build
```

If this is the first time you build the system from source, you can omit the `clean` target from the make command.

### Testing the OS

Now that you have successfully built the system, you can either launch or debug it using a target in the makefile:

```shell
make launch
```

```shell
make debug
```

The difference is that `debug` tells qemu to open a gdb server on port `1234`, so you can connect to it as a remote gdb target.

### Debugging

For debugging you can use any tool that uses gdb for debugging and can connect to a remote target. I personally use [vscode](https://code.visualstudio.com/) with the [native debug extension](https://marketplace.visualstudio.com/items?itemName=webfreak.debug), so I can connect to the remote target using a configuration like this:

(my .vscode/launch.json)
```json
{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "type": "gdb",
            "request": "attach",
            "name": "Attach to QEMU",
            "executable": "./bin/kernel.sym",
            "target": ":1234",
            "remote": true,
            "cwd": "${workspaceRoot}",
            "valuesFormatting": "prettyPrinters"
        }
    ]
}
```

## Credits

8x8 Font: [dhepper - font8x8](https://github.com/dhepper/font8x8)

## Resources

For anyone who wants to attempt something similar, here some useful resources:

- [OS Dev Wiki](https://wiki.osdev.org/)
- [Multiboot2 Specification](https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html)
- [YouTube: nanobyte - Building an OS](https://www.youtube.com/playlist?list=PLFjM7v6KGMpiH2G-kT781ByCNC_0pKpPN)
- [YouTube: Poncho - OS Dev](https://www.youtube.com/playlist?list=PLxN4E629pPnKKqYsNVXpmCza8l0Jb6l8-)
- [YouTube: Poncho - OS Dev - S2](https://www.youtube.com/playlist?list=PLxN4E629pPnJxCQCLy7E0SQY_zuumOVyZ)

Beware of errors and mistakes, even the official multiboot2 documentation does not clearly point out everything you'd have to know, e.g. every multiboot header tag has to be 8-byte aligned. I had to find out the hard way, so please just read carefully for the sake of your own mental sanity.
