#pragma once

#include <stddef.h>
#include <stdint.h>

struct vga_terminal {
  static constexpr size_t width = 80;
  static constexpr size_t height = 25;

  enum class COLOR : uint8_t {
    BLACK = 0,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHT_GREY,
    DARK_GREY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_MAGENTA,
    LIGHT_BROWN,
    WHITE
  };

  struct entry {
    entry(char c = ' ', COLOR fg_color = COLOR::RED,
          COLOR bg_color = COLOR::BLACK)
        : data{static_cast<uint16_t>(c) |
               uint16_t{static_cast<uint8_t>(fg_color) |
                        static_cast<uint8_t>(bg_color) << 4u}
                   << 8u} {}
    operator uint16_t() { return data; }
    uint16_t data{};
  };

  vga_terminal() {
    for (size_t y = 0; y < height; ++y) {
      for (size_t x = 0; x < width; ++x) {
        const size_t index = y * width + x;
        buffer[index] = entry{};
      }
    }
  }
  ~vga_terminal() {}

  entry& operator()(size_t row, size_t col) {
    const size_t index = row * width + col;
    return *reinterpret_cast<entry*>(&buffer[index]);
  }

  void putchar(char c) {
    if (c == '\n') {
      if (++current_row == height) current_row = 0;
      current_col = 0;
      return;
    }

    const size_t index = current_row * width + current_col;
    buffer[index] = entry{c};
    if (++current_col == width) {
      current_col = 0;
      if (++current_row == height) current_row = 0;
    }
  }

  friend vga_terminal& operator<<(vga_terminal& terminal, char c);
  friend vga_terminal& operator<<(vga_terminal& terminal, const char* str);

  size_t current_row{0};
  size_t current_col{0};
  COLOR fg_color{COLOR::WHITE};
  COLOR bg_color{COLOR::BLACK};
  uint16_t* buffer = reinterpret_cast<uint16_t*>(0xb8000);
};

inline vga_terminal& operator<<(vga_terminal& terminal, char c) {
  terminal.putchar(c);
  return terminal;
}

inline vga_terminal& operator<<(vga_terminal& terminal, const char* str) {
  for (auto it = str; *it != '\0'; ++it) terminal.putchar(*it);
  return terminal;
}