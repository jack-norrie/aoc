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

    int idx_1 = find_max(line, 0, strlen(line) - 1);
    int num_1 = line[idx_1] - '0';
    printf("%d\t%d\n", idx_1, num_1);

    int idx_2 = find_max(line, idx_1 + 1, strlen(line));
    int num_2 = line[idx_2] - '0';
    printf("%d\t%d\n", idx_2, num_2);

    int jolts = 10 * num_1 + num_2;
    printf("%d\n", jolts);

    res += jolts;
  }

  printf("%ld\n", res);

  fclose(file);
}
