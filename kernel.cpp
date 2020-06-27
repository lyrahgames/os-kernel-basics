#if defined(__linux__)
#error \
    "You are not using a cross-compiler, you will most certainly run into trouble."
#endif

#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler."
#endif

#include <vga_terminal.hpp>

// We use C linkage to be able to call kernel_main in boot.s file.
extern "C" void kernel_main() {
  vga_terminal terminal{};
  terminal << "Hello, C++ Kernel World!\nAnd welcome newline character." << '\n'
           << "This was successful!\n";
}
