#include "emu.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int main() {
  Chip8 c8 = init_chip8();
  printf("%d", c8.registers[0xF]);
  return EXIT_SUCCESS;
}
