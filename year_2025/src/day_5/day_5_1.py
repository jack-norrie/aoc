import logging


def process_line(line: str) -> int:
    logging.debug(line)
    return 0


def find_number_of_valid_ids(ids: list[int], intervals: list[tuple[int, int]]) -> int:
    ids = sorted(ids)
    intervals = sorted(intervals)

    n = len(intervals)
    j = 0

    res = 0
    for id in ids:
        while j < n - 1 and id > intervals[j][1]:
            j += 1

        if id >= intervals[j][0] and id <= intervals[j][1]:
            res += 1
            logging.debug(f"{id} - fresh")
        else:
            logging.debug(f"{id} - spoiled")

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

        logging.info(f"{intervals}, {ids}")

        res = find_number_of_valid_ids(ids, intervals)
        logging.info(res)


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    main()
