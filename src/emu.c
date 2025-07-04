#include "emu.h"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

Chip8 init_chip8() {
  srand(time(NULL));
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

uint16_t get_instruction(Chip8 *c8) {
  uint8_t big = c8->memory[c8->instruction++];
  uint8_t little = c8->memory[c8->instruction++];
  return (((uint16_t)big) << 8) + little;
}

// bad habit of shoving all the opcode handling into one function but there's so
// little opcodes on this system that it should be alright?
void step_chip8(Chip8 *c8) {
  uint16_t instruction = get_instruction(c8);

  if ((instruction & 0xF000) == 0x1000) {
    c8->instruction = instruction & 0x0FFF;
  } else if ((instruction & 0xF000) == 0x2000) {
    push_stack_chip8(c8, c8->instruction);
    c8->instruction = instruction & 0x0FFF;
  } else if ((instruction & 0xF000) == 0x3000) {
    uint8_t reg = c8->registers[(instruction >> 4) & 0x0F];
    uint8_t value = (uint8_t)instruction;

    if (reg == value) {
      c8->instruction += 2;
    }
  } else if ((instruction & 0xF000) == 0x4000) {
    uint8_t reg = c8->registers[(instruction >> 4) & 0x0F];
    uint8_t value = (uint8_t)instruction;

    if (reg != value) {
      c8->instruction += 2;
    }
  } else if ((instruction & 0xF000) == 0x5000) {
    uint8_t reg1 = c8->registers[(instruction >> 4) & 0x0F];
    uint8_t reg2 = c8->registers[(instruction >> 2) & 0x0F];

    if (reg1 == reg2) {
      c8->instruction += 2;
    }
  } else if ((instruction & 0xF000) == 0x6000) {
    uint8_t *reg1 = &c8->registers[(instruction >> 4) & 0x0F];
    uint8_t value = (uint8_t)instruction;

    *reg1 = value;
  } else if ((instruction & 0xF000) == 0x7000) {
    uint8_t reg1 = c8->registers[(instruction >> 4) & 0x0F];
    uint8_t value = (uint8_t)instruction;

    reg1 += value;
  } else if ((instruction & 0xF000) == 0x8000) {
    uint8_t secondary_instruction = (instruction & 0xF);
    uint8_t *reg_x = &c8->registers[(secondary_instruction >> 4) & 0x0F];
    uint8_t *reg_y = &c8->registers[(secondary_instruction >> 2) & 0x0F];
    switch (secondary_instruction) {
    case 0x0: {
      *reg_x = *reg_y;
      break;
    }
    case 0x1: {
      *reg_x = *reg_x | *reg_y;
      break;
    }
    case 0x2: {
      *reg_x = *reg_x & *reg_y;
      break;
    }
    case 0x3: {
      *reg_x = *reg_x ^ *reg_y;
      break;
    }
    case 0x4: {
      uint16_t sum = *reg_x + *reg_y;
      c8->registers[0xF] = sum > 0xFF;
      *reg_x = sum;
      break;
    }
    case 0x5: {
      c8->registers[0xF] = *reg_x > *reg_y;
      *reg_x = *reg_y - *reg_x;
      break;
    }
    case 0x6: {
      if ((*reg_x & 1) == true) {
        *reg_x = *reg_y - *reg_x;
      } else {
        *reg_x /= 2;
      }
      break;
    }
    case 0x7: {
      c8->registers[0xF] = *reg_x < *reg_y;
      *reg_x = *reg_y - *reg_x;
      break;
    }
    case 0xE: {
      bool flag = *reg_x >> 7;
      c8->registers[0xF] = flag;
      if (flag) {
        *reg_x *= 2;
      }
      break;
    }
    }
  } else if ((instruction & 0xF000) == 0x9000) {
    uint8_t reg1 = c8->registers[(instruction >> 4) & 0x0F];
    uint8_t reg2 = c8->registers[(instruction >> 2) & 0x0F];

    if (reg1 != reg2) {
      c8->instruction += 2;
    }
  } else if ((instruction & 0xF000) == 0xA000) {
    c8->index = instruction & 0x0FFF;
  } else if ((instruction & 0xF000) == 0xB000) {
    c8->instruction = (instruction & 0x0FFF) + c8->index;
  } else if ((instruction & 0xF000) == 0xC000) {
    uint8_t random = rand();
    uint8_t *reg = &c8->registers[(instruction >> 4) & 0x0F];
    *reg = ((uint8_t)instruction) & random;
  } else if ((instruction & 0xF000) == 0xD000) {
  } else if ((instruction & 0xF000) == 0xE000) {
  } else if ((instruction & 0xF000) == 0xF000) {
  } else {
    if (instruction == 0x00E0) {
    } else if (instruction == 0x00EE) {
      c8->instruction = pop_stack_chip8(c8);
      // treat SYS instruction as noop
    } else if ((instruction >> 12) != 0) {
      c8->halted = true;
    }
  }
}

void render_sprite(Chip8 *c8, uint8_t sprite_index, uint8_t height, uint8_t x,
                   uint8_t y) {
  x = x % SCREEN_WIDTH;
  y = y % SCREEN_HEIGHT;
  for (uint8_t i = 0; i < height; i++) {
    uint8_t byte = c8->memory[sprite_index];
    uint8_t mask = 0b1;
    for (uint8_t i = 0; i < 8; i++) {
      c8->frame[x + i][y] ^= (byte & mask);
      if (x + i >= SCREEN_WIDTH) {
        break;
      }
    }
    if (++y >= SCREEN_HEIGHT) {
      break;
    }
    mask <<= 1;
  }
}
