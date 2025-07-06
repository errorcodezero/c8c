#ifndef INCLUDE_SRC_EMU_H_
#define INCLUDE_SRC_EMU_H_

#include <stdbool.h>
#include <stdint.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

// sourced from
// https://www.cs.columbia.edu/~sedwards/classes/2022/4840-spring/designs/CHIP-8.pdf
typedef struct {
  // last register is for storing flags
  uint8_t registers[16];
  uint16_t index;
  uint16_t instruction;
  uint16_t stack[16];
  uint8_t stack_ptr;
  uint8_t timer;
  uint8_t sound_reg;
  bool frame[SCREEN_WIDTH / 8][SCREEN_HEIGHT];
  uint8_t memory[0xFFF];
  bool halted;
} Chip8;

Chip8 init_chip8();
void step_chip8(Chip8 *c8);
void render_sprite(Chip8 *c8, uint8_t sprite_index, uint8_t size, uint8_t x,
                   uint8_t y);

#endif // INCLUDE_SRC_EMU_H_
