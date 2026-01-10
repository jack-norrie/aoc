import logging
import typing


def read_data(fpath) -> list[tuple[int, int]]:
    with open(fpath, "r") as f:
        data = [line.strip("\n") for line in f.readlines()]
        data = [tuple(int(x) for x in line.split(",")) for line in data]
        data = typing.cast(list[tuple[int, int]], data)

    return data


def get_area(x: tuple[int, int], y: tuple[int, int]) -> float:
    return (abs(x[0] - y[0]) + 1) * (abs(x[1] - y[1]) + 1)


def solve(data: list[tuple[int, int]]) -> int:
    n = len(data)

    areas = []
    for i in range(n):
        for j in range(i + 1, n):
            area = get_area(data[i], data[j])
            areas.append(area)

    return max(areas)


def main():
    data = read_data("data/day_9/input")
    logging.debug(data)

    res = solve(data)
    logging.info(res)


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    main()
