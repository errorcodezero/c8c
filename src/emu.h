#ifndef INCLUDE_SRC_EMU_H_
#define INCLUDE_SRC_EMU_H_

#include <stdbool.h>
#include <stdint.h>

// sourced from
// https://www.cs.columbia.edu/~sedwards/classes/2022/4840-spring/designs/CHIP-8.pdf
typedef struct {
  uint8_t registers[16];
  uint16_t index;
  uint8_t stack[16];
  uint8_t stack_ptr;
  uint8_t timer;
  uint8_t sound_reg;
  uint8_t frame_buf_0[1024];
  uint8_t frame_buf_1[1024];
  bool frame_buf;
} Chip8;

Chip8 init_chip8();

#endif // INCLUDE_SRC_EMU_H_
