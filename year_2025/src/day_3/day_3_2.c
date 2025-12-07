#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find_max(char *s, int l, int r) {
  int max_val = -1;
  int max_pos = 0;

  for (int i = l; i < r; i++) {
    if (s[i] > max_val) {
      max_val = s[i];
      max_pos = i;
    }
  }

  return max_pos;
}

int main() {
  FILE *file = fopen("data/day_3/input", "r");

  long res = 0;

  char line[1024];
  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';
    printf("%s\n", line);

    int idx = -1;
    long jolts = 0;
    for (int i = 11; i >= 0; i--) {
      idx = find_max(line, idx + 1, strlen(line) - i);
      long num = line[idx] - '0';
      printf("%d\t%ld\n", idx, num);
      jolts += num * (long)pow(10, i);
    }
    printf("%ld\n", jolts);

    res += jolts;
  }

  printf("%ld\n", res);

  fclose(file);
}
