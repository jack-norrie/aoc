import logging
from copy import deepcopy
import typing


def read_data(fpath) -> list[tuple[int, int]]:
    with open(fpath, "r") as f:
        data = [line.strip("\n") for line in f.readlines()]
        data = [tuple(int(x) for x in line.split(",")) for line in data]
        data = typing.cast(list[tuple[int, int]], data)

    return data


def fill_line(grid, prev_pos, pos, vec) -> None:
    r, c = prev_pos

    grid[r][c] = "#"

    while (r, c) != pos:
        r += vec[0]
        c += vec[1]
        grid[r][c] = "#"


def print_grid(grid: list[list[str]]) -> None:
    # m, n = len(grid), len(grid[0])
    # logging.debug("-" * n * 5)
    # for row in grid:
    #     logging.debug(row)
    # logging.debug("-" * n * 5)
    return


def get_direciton(pos, prev_pos) -> tuple[int, int]:
    dx = pos[1] - prev_pos[1]
    dy = pos[0] - prev_pos[0]

    if dx > 0:
        vec = (0, 1)
    elif dx < 0:
        vec = (0, -1)
    elif dy > 0:
        vec = (1, 0)
    elif dy < 0:
        vec = (-1, 0)
    else:
        raise RuntimeError

    return vec


def draw_perimeter(data: list[tuple[int, int]], grid):
    logging.debug("Drawing Perimeter")
    prev_pos = data[0]
    r, c = prev_pos
    grid[r][c] = "#"

    data = data.copy()
    data.append(data[0])

    for pos in data[1:]:
        vec = get_direciton(pos, prev_pos)
        fill_line(grid, prev_pos, pos, vec)
        print_grid(grid)

        prev_pos = pos


def _fill_grid(
    grid: list[list[str]], pos: tuple[int, int]
) -> tuple[list[list[str]], bool]:
    grid = deepcopy(grid)

    m, n = len(grid), len(grid[0])

    vecs = ((1, 0), (-1, 0), (0, 1), (0, -1))

    def dfs(pos):
        r, c = pos
        grid[r][c] = "#"

        res = True
        for vec in vecs:
            d_r, d_c = vec
            n_r, n_c = r + d_r, c + d_c

            if n_r < 0 or n_r >= m or n_c < 0 or n_c >= n:
                return False

            if grid[n_r][n_c] == ".":
                res = res and dfs((n_r, n_c))

        return res

    interior = dfs(pos)

    return grid, interior


def fill_grid(grid: list[list[str]], pos: tuple[int, int]) -> list[list[str]]:
    logging.debug("Filling Grid")
    vecs = ((1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1), (0, -1), (1, -1))
    for vec in vecs:
        new_pos = (pos[0] + vec[0], pos[1] + vec[1])
        filled_grid, interior = _fill_grid(grid, new_pos)
        if interior:
            return filled_grid
    raise RuntimeError("Could not fill grid.")


def get_grid(data: list[tuple[int, int]]) -> list[list[str]]:
    logging.debug("Getting Grid")
    m = max(pos[0] for pos in data)
    n = max(pos[1] for pos in data)
    logging.debug(f"limits: {m}-{n}")

    grid = [["." for c in range(n + 1)] for r in range(m + 1)]

    draw_perimeter(data, grid)

    grid = fill_grid(grid, data[0])
    print_grid(grid)

    return grid


def get_area(x: tuple[int, int], y: tuple[int, int]) -> float:
    return (abs(x[0] - y[0]) + 1) * (abs(x[1] - y[1]) + 1)


def check_edge_valid(x, y, grid) -> bool:
    if x == y:
        return True

    vec = get_direciton(y, x)

    r, c = x
    while (r, c) != y:
        r += vec[0]
        c += vec[1]

        if grid[r][c] == ".":
            return False

    return True


def check_valid(x, y, grid) -> bool:
    corner_1 = (x[0], y[1])
    corner_2 = (y[0], x[1])

    edge_1_valid = check_edge_valid(x, corner_1, grid)
    edge_2_valid = check_edge_valid(x, corner_2, grid)
    edge_3_valid = check_edge_valid(corner_1, y, grid)
    edge_4_valid = check_edge_valid(corner_2, y, grid)

    return edge_1_valid and edge_2_valid and edge_3_valid and edge_4_valid


def solve(data: list[tuple[int, int]], grid: list[list[str]]) -> int:
    n = len(data)

    areas = []
    for i in range(n):
        for j in range(i + 1, n):
            area = get_area(data[i], data[j])
            if area > 0 and check_valid(data[i], data[j], grid):
                areas.append(area)

    return max(areas)


def main():
    data = read_data("data/day_9/input")
    logging.debug(data)

    grid = get_grid(data)

    res = solve(data, grid)
    logging.info(res)


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    main()
