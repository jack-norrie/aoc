#include <stdio.h>
#include <string.h>

int is_coord_valid(const int i, const int j, const int m, const int n) {
  return (i >= 0 && i < m && j >= 0 && j < n);
}

int main() {
  int res = 0;

  FILE *file = fopen("data/day_4/input", "r");

  char grid[1024][1024];
  int m = 0;
  while (fgets(grid[m], sizeof(grid[m]), file)) {
    grid[m][strcspn(grid[m], "\n")] = '\0';
    m++;
  }
  int n = strlen(grid[0]);

  int directions[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0},
                          {1, 0},   {-1, 1}, {0, 1},  {1, 1}};

  for (int r = 0; r < m; r++) {
    for (int c = 0; c < n; c++) {
      if (grid[r][c] == '@') {
        int count = 0;
        for (int k = 0; k < 8; k++) {
          int n_r = r + directions[k][0];
          int n_c = c + directions[k][1];
          if (is_coord_valid(n_r, n_c, m, n) && grid[n_r][n_c] == '@') {
            count++;
          }
        }

        if (count < 4) {
          res++;
        }
      }
    }
  }

  printf("%d\n", res);

  fclose(file);
}
