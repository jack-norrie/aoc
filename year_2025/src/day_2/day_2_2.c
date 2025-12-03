#include <math.h>
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

int is_k_copies(const char *s, int k) {
  int n = strlen(s);
  int m = n / k;

  int i;
  for (i = 0; i < strlen(s); i++) {
    if (s[i] != s[i % m]) {
      return 0;
    }
  }
  return 1;
}

int main() {
  FILE *file = fopen("data/day_2/input", "r");

  long res = 0;

  char line[1024];
  fgets(line, sizeof(line), file);
  line[strcspn(line, "\n")] = '\0';

  char *comma_savetr = NULL;
  char *comma_token = strtok_r(line, ",", &comma_savetr);
  while (comma_token != NULL) {
    printf("comma_token: %s\n", comma_token);

    Bounds bounds = get_bounds(comma_token);

    long i;
    for (i = bounds.lower; i <= bounds.upper; i++) {
      char s[32];
      sprintf(s, "%ld", i);
      int n = strlen(s);

      int k;
      for (k = 2; k <= strlen(s); k++) {
        if (n % k == 0 && is_k_copies(s, k) == 1) {
          printf("%ld\n", i);
          res += i;
          break;
        }
      }
    }

    comma_token = strtok_r(NULL, ",", &comma_savetr);
    printf("\n");
  }

  printf("result: %ld\n", res);

  fclose(file);
}
