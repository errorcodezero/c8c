#include "emu.h"
#include <stdio.h>

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

void step_chip8(Chip8 *c8) {
  // ignoring the SYS instruction
  if (c8->memory[c8->instruction] == 0x0) {
    c8->instruction++;
    if (c8->memory[c8->instruction] == 0xE0) {
    } else if (c8->memory[c8->instruction] == 0xEE) {
    } else {
      c8->halted = true;
    }
  } else if ((c8->memory[c8->instruction] >> 4) == 0x1) {
  } else if ((c8->memory[c8->instruction] >> 4) == 0x2) {
  } else if ((c8->memory[c8->instruction] >> 4) == 0x3) {
  } else if ((c8->memory[c8->instruction] >> 4) == 0x4) {
  } else if ((c8->memory[c8->instruction] >> 4) == 0x5) {
  } else if ((c8->memory[c8->instruction] >> 4) == 0x6) {
  } else if ((c8->memory[c8->instruction] >> 4) == 0x7) {
  } else if ((c8->memory[c8->instruction] >> 4) == 0x8) {
  } else if ((c8->memory[c8->instruction] >> 4) == 0xA) {
  } else if ((c8->memory[c8->instruction] >> 4) == 0xB) {
  } else if ((c8->memory[c8->instruction] >> 4) == 0xC) {
  } else if ((c8->memory[c8->instruction] >> 4) == 0xD) {
  } else if ((c8->memory[c8->instruction] >> 4) == 0xE) {
  } else if ((c8->memory[c8->instruction] >> 4) == 0xF) {
  }
}
