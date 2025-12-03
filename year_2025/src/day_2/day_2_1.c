#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  long lower;
  long upper;
} Bounds;

Bounds get_bounds(char *s) {
  long bounds[2];

  long i = 0;
  char *hyphen_saveptr = NULL;
  char *hyphen_token = strtok_r(s, "-", &hyphen_saveptr);
  while (hyphen_token != NULL) {
    bounds[i] = atol(hyphen_token);
    i++;
    hyphen_token = strtok_r(NULL, "-", &hyphen_saveptr);
  }

  Bounds res = {bounds[0], bounds[1]};

  return res;
}

int main() {
  FILE *file = fopen("data/day_2/input", "r");

  long res = 0;

  char line[1024];
  fgets(line, sizeof(line), file);
  line[strcspn(line, "\n")] = '\0';
  printf("%s\n", line);

  char *comma_savetr = NULL;
  char *comma_token = strtok_r(line, ",", &comma_savetr);
  while (comma_token != NULL) {
    printf("comma_token: %s\n", comma_token);

    Bounds bounds = get_bounds(comma_token);

    long i;
    for (i = bounds.lower; i <= bounds.upper; i++) {
      char s[32];
      sprintf(s, "%ld", i);
      if (strlen(s) % 2 == 0) {
        long half = strlen(s) / 2;
        if (strncmp(s, s + half, half) == 0) {
          printf("%ld\n", i);
          res += i;
        }
      }
    }

    comma_token = strtok_r(NULL, ",", &comma_savetr);
    printf("\n");
  }

  printf("result: %ld\n", res);

  fclose(file);
}
