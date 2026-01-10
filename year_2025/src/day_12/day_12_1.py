from functools import lru_cache
import logging
import re


type ShapeRow = tuple[bool, bool, bool]
type Shape = tuple[ShapeRow, ShapeRow, ShapeRow]
type Problem = tuple[int, int, tuple[int, ...]]


def read_data(
    path: str,
) -> tuple[dict[str, set[Shape]], Problem]:
    index_pattern = re.compile(r"(\d+):")
    count_pattern = re.compile(r"(\d+)x(\d+):(?:\s+?)(.+)+")

    shapes = {}
    problems = []
    with open(path, "r") as f:
        while (line := f.readline()) != "":
            line = line.strip("\n")

            if match := index_pattern.match(line):
                idx = int(match.groups()[0])
                logging.debug(idx)

                shape = []
                for _ in range(3):
                    line = f.readline().strip("\n")
                    logging.debug(line)
                    shape_row = tuple(c == "#" for c in line)
                    shape.append(shape_row)
                shapes[idx] = set([tuple(shape)])

            elif match := count_pattern.match(line):
                w, l, counts_str = match.groups()
                counts = tuple([int(count) for count in counts_str.split(" ")])
                logging.debug(f"{w}x{l}: {counts}")

                problems.append((w, l, counts))

    return shapes, problems


def add_flips(shapes: dict[str, set[Shape]]) -> None:
    for idx in shapes:
        shape_set = shapes[idx]

        for shape in shape_set.copy():
            new_shape = []
            for row in shape:
                new_row = tuple(reversed(row))
                new_shape.append(new_row)

            new_shape = tuple(new_shape)
            shapes[idx].add(new_shape)


def add_rotations(shapes: dict[str, set[Shape]]) -> None:
    for idx in shapes:
        shape_set = shapes[idx]

        for shape in shape_set.copy():
            new_shape = [list(row) for row in shape]

            for _ in range(3):
                for i in range(2):
                    temp = new_shape[0][i]
                    new_shape[0][i] = new_shape[2 - i][0]
                    new_shape[2 - i][0] = new_shape[2][2 - i]
                    new_shape[2][2 - i] = new_shape[i][2]
                    new_shape[i][2] = temp

                shapes[idx].add(tuple([tuple(row) for row in new_shape]))


def get_base_cases(shapes) -> dict[tuple[int, int], list[tuple[int, ...]]]: ...


def rec(grid: tuple[int, int], memo) -> list[tuple[int, ...]]: ...


def log_shapes(shapes):
    for idx, shape_set in shapes.items():
        logging.debug(idx)
        for shape in shape_set:
            logging.debug("-" * 5)
            for row in shape:
                logging.debug(row)
            logging.debug("-" * 5)


def solve(shapes, problems) -> int:
    logging.debug("Adding flips")
    add_flips(shapes)
    log_shapes(shapes)

    logging.debug("Adding Rotations")
    add_rotations(shapes)
    log_shapes(shapes)

    logging.debug(shapes)

    memo = get_base_cases(shapes)

    for problem in problems:
        rec(problem, memo)

    return 0


def main():
    shapes, problems = read_data("data/day_12/test")

    res = solve(shapes, problems)

    logging.info(res)


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    main()
