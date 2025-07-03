#include "emu.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  Chip8 c8 = init_chip8();
  printf("%d", c8.flag);
  return EXIT_SUCCESS;
}
