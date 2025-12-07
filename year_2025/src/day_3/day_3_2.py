def main():
    with open("data/day_3/input", "r") as f:
        lines = f.readlines()
        lines = [line.split("\n")[0] for line in lines]

    res = 0
    for line in lines:
        stack = []
        for i, c in enumerate(line):
            digit = int(c)

            num_remaining = len(line) - i
            while len(stack) + num_remaining > 12 and stack and digit > stack[-1]:
                stack.pop()
            stack.append(digit)

        jolt = sum(d * pow(10, 12 - i) for i, d in enumerate(stack[:12], start=1))
        res += jolt

    print(res)


if __name__ == "__main__":
    main()
