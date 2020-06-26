# Simple Operating System Kernel for x86 Systems

## Requirements

- [GCC Cross-Compiler](https://wiki.osdev.org/GCC_Cross_Compiler) for i686-elf
- GRUB
- QEMU

# Building, Testing, Running 

Build the Kernel and the ISO Image.

    make 

Test the generated kernel for validity.

    make test

Run the ISO image in QEMU.

    make run

Run the kernel itself directly in QEMU.

    make run_kernel

Remove all build artifacts and target files.

    make clean


## References

- https://wiki.osdev.org/Bare_Bones
