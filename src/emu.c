#include "emu.h"

Chip8 init_chip8() {
  Chip8 c8 = {0};
  return c8;
}

void push_stack_chip8(Chip8 *c8, uint16_t value) {
  uint8_t stack_size = sizeof(c8->stack) / sizeof(c8->stack[0]);
  if (c8->stack_ptr < stack_size - 1) {
    c8->stack[c8->stack_ptr++] = value;
  } else {
    // stack overflow yikes
    c8->halted = true;
  }
}

uint16_t pop_stack_chip8(Chip8 *c8) {
  if (c8->stack_ptr > 0) {
    return c8->stack[c8->stack_ptr--];
  } else {
    // stack underflow
    c8->halted = true;

    // i can't just return nothing
    return c8->stack[c8->stack_ptr];
  }
}
