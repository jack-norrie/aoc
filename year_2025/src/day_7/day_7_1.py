import logging
from collections import deque


def read_data(fpath: str) -> list[str]:
    with open(fpath, "r") as f:
        data = [line.strip("\n") for line in f.readlines()]
    return data


def solve(data: list[str]) -> int:
    m, n = len(data), len(data[0])

    pos = None
    for c in range(n):
        if data[0][c] == "S":
            pos = (0, c, 1)

    if pos is None:
        raise RuntimeError("Could not find starting position.")

    res = 0
    q = deque([pos])
    while q:
        seen = set()
        for _ in range(len(q)):
            r, c = q.popleft()

            if r < m - 1:
                if data[r + 1][c] == "^":
                    res += 1
                    left = (r + 1, c - 1)
                    if left not in seen:
                        q.append(left)
                        seen.add(left)

                    right = (r + 1, c + 1)
                    if right not in seen:
                        q.append(right)
                        seen.add(right)
                elif (center := (r + 1, c)) not in seen:
                    q.append(center)
                    seen.add(center)

    return res


def main():
    data = read_data("data/day_7/input")
    for line in data:
        logging.debug(line)

    res = solve(data)

    logging.info(res)


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    main()
