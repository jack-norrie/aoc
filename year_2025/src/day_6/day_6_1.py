import logging
import re


def solve(nums, ops):
    m, n = len(nums), len(nums[0])

    op_map = {
        "+": lambda x, y: x + y,
        "-": lambda x, y: x - y,
        "*": lambda x, y: x * y,
        "/": lambda x, y: x / y,
    }

    res = 0
    for c in range(n):
        op = ops[c]

        cur = nums[0][c]
        for r in range(1, m):
            cur = op_map[op](cur, nums[r][c])

        logging.debug(cur)
        res += cur

    return res


def main():
    pattern = re.compile(r"([\+\-\*\/\d]+)")

    with open("data/day_6/input", "r") as f:
        line_symbols = []
        for line in f.readlines():
            line = line.strip("\n")

            matches = pattern.findall(line)

            line_symbols.append(matches)

    nums = [[int(x) for x in row] for row in line_symbols[:-1]]
    logging.debug(nums)

    ops = line_symbols[-1]
    logging.debug(ops)

    res = solve(nums, ops)

    logging.info(res)


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    main()
