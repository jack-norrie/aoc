import logging
from functools import lru_cache
from collections import defaultdict


def read_data(path: str) -> list[str]:
    with open(path, "r") as f:
        data = [line.strip("\n") for line in f.readlines()]

    return data


def get_adj_list(data: list[str]) -> dict[str, str]:
    adj_list = defaultdict(list)
    for line in data:
        u, neighbours = line.split(":")
        for neighbour in neighbours.strip(" ").split(" "):
            adj_list[u].append(neighbour)

    return adj_list


def count_paths(adj_list: dict[str, str], start: str, end: str) -> int:
    @lru_cache(None)
    def dfs(v: str) -> int:
        if v == end:
            return 1

        res = 0
        for u in adj_list[v]:
            res += dfs(u)

        return res

    return dfs(start)


def main():
    data = read_data("data/day_11/input")
    for line in data:
        logging.debug(line)

    adj_list = get_adj_list(data)
    logging.debug(adj_list)

    res = 0
    res += (
        count_paths(adj_list, "svr", "dac")
        * count_paths(adj_list, "dac", "fft")
        * count_paths(adj_list, "fft", "out")
    )
    res += (
        count_paths(adj_list, "svr", "fft")
        * count_paths(adj_list, "fft", "dac")
        * count_paths(adj_list, "dac", "out")
    )
    logging.info(res)


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    main()
