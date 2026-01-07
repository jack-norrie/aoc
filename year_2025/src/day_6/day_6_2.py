import logging
import re
from string import digits


def solve(data):
    m, n = len(data), len(data[0])
    print({len(line) for line in data})
    for line in data:
        print(line, len(line))

    res = 0

    ops = {
        "+": lambda x, y: x + y,
        "*": lambda x, y: x * y,
    }

    l = 0
    r = 1

    res = 0
    while l < n:
        # Get operation associated with current problem
        op = ops[data[-1][l]]

        # Find start of next problem
        while r < n and data[-1][r] not in ops:
            r += 1

        # Iterate to one before start of next problem
        cur_res = None
        while l < r - 1:
            # Find start of current number
            i = 0
            while i < m and data[i][l] not in digits:
                i += 1

            # Get number in current column
            cur_num = 0
            while i < m and (num := data[i][l]) in digits:
                cur_num = cur_num * 10 + int(num)
                i += 1
            logging.debug(f"num: {cur_num}")

            if cur_res is None:
                cur_res = cur_num
            else:
                cur_res = op(cur_res, cur_num)

            l += 1

        logging.debug(f"res: {cur_res}")

        if cur_res is not None:
            res += cur_res
        else:
            raise RuntimeError()

        l += 1
        r += 1

    return res


def main():
    pattern = re.compile(r"([\+\-\*\/\d]+)")

    with open("data/day_6/input", "r") as f:
        data = [line.strip("\n") for line in f.readlines()]
        # Add extra space at end so that "end of problem"  is always designated by a empty column
        max_line_length = max(len(line) for line in data) + 1
        data = [f"{line:<{max_line_length}}" for line in data]

    res = solve(data)

    logging.info(res)


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    main()
