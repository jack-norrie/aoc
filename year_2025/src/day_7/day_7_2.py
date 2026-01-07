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
            pos = (0, c)

    if pos is None:
        raise RuntimeError("Could not find starting position.")

    res = 0
    q = deque([pos])
    counts = {pos: 1}
    for r in range(m - 1):
        next_counts = {}
        for _ in range(len(q)):
            pos = q.popleft()
            _, c = pos

            if data[r + 1][c] == "^":
                res += 1
                left = (r + 1, c - 1)
                if left not in next_counts:
                    q.append(left)
                next_counts[left] = next_counts.get(left, 0) + counts[pos]

                right = (r + 1, c + 1)
                if right not in next_counts:
                    q.append(right)
                next_counts[right] = next_counts.get(right, 0) + counts[pos]
            else:
                center = (r + 1, c)
                if center not in next_counts:
                    q.append(center)
                next_counts[center] = next_counts.get(center, 0) + counts[pos]

        counts = next_counts

    return sum(counts.values())


def main():
    data = read_data("data/day_7/input")
    for line in data:
        logging.debug(line)

    res = solve(data)

    logging.info(res)


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    main()
