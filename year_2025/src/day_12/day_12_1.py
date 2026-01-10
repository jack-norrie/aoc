from functools import lru_cache
import logging
import re

type Problem = tuple[int, int, tuple[int, ...]]


def read_data(
    path: str,
) -> list[Problem]:
    count_pattern = re.compile(r"(\d+)x(\d+):(?:\s+?)(.+)+")

    problems = []
    with open(path, "r") as f:
        while (line := f.readline()) != "":
            if match := count_pattern.match(line):
                w, l, counts_str = match.groups()
                counts = tuple([int(count) for count in counts_str.split(" ")])
                logging.debug(f"{w}x{l}: {counts}")

                problems.append((int(w), int(l), counts))

    return problems


def main():
    problems = read_data("data/day_12/input")

    res = 0
    for w, l, counts in problems:
        print(w, l, counts)
        if (w // 3) * (l // 3) >= sum(counts):
            res += 1
    logging.info(res)


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    main()
