#if defined(__linux__)
#error \
    "You are not using a cross-compiler, you will most certainly run into trouble."
#endif

#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler."
#endif

#include <icxxabi.hpp>
#include <vga.hpp>

vga::terminal terminal{};

// We use C linkage to be able to call kernel_main in boot.s file.
extern "C" void kernel_main() {
  terminal << "Hello, C++ Kernel World!\n"
           << foreground(vga::COLOR::BLACK) << background(vga::COLOR::WHITE)
           << "And welcome newline character." << '\n'
           << foreground(vga::COLOR::RED) << "This was successful!\n"
           << "test\n";
}
