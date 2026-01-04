import logging


def process_line(line: str) -> int:
    logging.debug(line)
    return 0


def get_combined_intervals_span(intervals: list[tuple[int, int]]) -> int:
    intervals = sorted(intervals)

    combined_intervals = []

    cur = list(intervals[0])
    for interval in intervals[1:]:
        if interval[0] <= cur[1]:
            cur[1] = max(cur[1], interval[1])
        else:
            combined_intervals.append(cur)
            cur = list(interval)
    combined_intervals.append(cur)

    res = sum(u - l + 1 for l, u in combined_intervals)

    return res


def main():
    with open("data/day_5/input", "r") as f:
        res = 0

        intervals = []
        while line := f.readline().strip("\n"):
            lb, ub = line.split("-")
            intervals.append((int(lb), int(ub)))

        ids = []
        while line := f.readline().strip("\n"):
            ids.append(int(line))

        logging.debug(f"{intervals}, {ids}")

        res = get_combined_intervals_span(intervals)
        logging.info(res)


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    main()
