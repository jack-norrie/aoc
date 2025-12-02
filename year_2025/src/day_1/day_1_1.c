#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  FILE *file = fopen("data/day_1/input", "r");

  int res = 0;
  int pos = 50;
  char line[16];
  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';

    char direction = line[0];

    char *slice = &line[0] + 1;
    int rotations = atoi(slice);

    int new_pos;
    if (direction == 'L') {
      new_pos = pos - rotations;
    } else {
      new_pos = pos + rotations;
    }

    pos = ((new_pos % 100) + 100) % 100;
    if (pos == 0) {
      res += 1;
    }
  }

  printf("%d\n", res);

  fclose(file);

  return 0;
}
