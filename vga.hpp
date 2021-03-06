#pragma once

#include <stddef.h>
#include <stdint.h>

namespace vga {

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

struct terminal {
  static constexpr size_t width = 80;
  static constexpr size_t height = 25;

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

  terminal() {
    for (size_t y = 0; y < height; ++y) {
      for (size_t x = 0; x < width; ++x) {
        const size_t index = y * width + x;
        buffer[index] = entry{};
      }
    }

    const char text[] = "Terminal started.\n";
    for (size_t i = 0; i < sizeof(text) - 1; ++i) putchar(text[i]);
    for (size_t i = 0; i < width; ++i) putchar('-');
  }
  ~terminal() {
    fg_color = COLOR::WHITE;
    bg_color = COLOR::BLACK;
    if (current_col != 0) putchar('\n');
    for (size_t i = 0; i < width; ++i) putchar('-');
    const char text[] = "Terminal ended.\n";
    for (size_t i = 0; i < sizeof(text) - 1; ++i) putchar(text[i]);
  }

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
    buffer[index] = entry{c, fg_color, bg_color};
    if (++current_col == width) {
      current_col = 0;
      if (++current_row == height) current_row = 0;
    }
  }

  size_t current_row{0};
  size_t current_col{0};
  COLOR fg_color{COLOR::WHITE};
  COLOR bg_color{COLOR::BLACK};
  uint16_t* buffer = reinterpret_cast<uint16_t*>(0xb8000);
};

struct fg_color_t {
  COLOR color;
};
inline fg_color_t foreground(COLOR color) { return fg_color_t{color}; }
inline terminal& operator<<(terminal& t, fg_color_t fg_color) {
  t.fg_color = fg_color.color;
  return t;
}

struct bg_color_t {
  COLOR color;
};
inline bg_color_t background(COLOR color) { return bg_color_t{color}; }
inline terminal& operator<<(terminal& t, bg_color_t bg_color) {
  t.bg_color = bg_color.color;
  return t;
}

inline terminal& operator<<(terminal& t, char c) {
  t.putchar(c);
  return t;
}

inline terminal& operator<<(terminal& t, const char* str) {
  for (auto it = str; *it != '\0'; ++it) t.putchar(*it);
  return t;
}

}  // namespace vga