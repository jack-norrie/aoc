import re
from collections import deque
import logging


def parse_line(line: str) -> tuple[list[int], int, list[int]]:
    pattern = re.compile(r"\[(.*)\] (.*) \{(.*)\}")

    matching = pattern.match(line)
    if not matching:
        raise RuntimeError("Input did not match pattern.")

    light = matching.group(1)
    button_idx_sets = [x.strip("()").split(",") for x in matching.group(2).split(" ")]
    jolt = matching.group(3)

    buttons = []
    for button_idx_set in button_idx_sets:
        button = 0
        for button_idx in button_idx_set:
            button |= 1 << int(button_idx)
        buttons.append(button)

    target_state = 0
    for i, c in enumerate(light):
        if c == "#":
            target_state |= 1 << i

    target_counts = [int(x) for x in jolt.split(",")]

    return buttons, target_state, target_counts


def find_minimum_clicks_for_target_state(
    buttons: list[int], target_counts: list[int]
) -> int:
    n_lights = len(target_counts)

    cur_state = tuple(target_counts)
    q = deque([cur_state])

    zero_state = (0,) * n_lights

    seen = set()
    clicks = 0
    while q:
        for _ in range(len(q)):
            cur_state = q.popleft()

            if cur_state == zero_state:
                return clicks

            for button in buttons:
                new_state_list = list(cur_state)

                valid = True
                for i in range(n_lights):
                    if button & (1 << i):
                        new_state_list[i] -= 1
                        if new_state_list[i] < 0:
                            valid = False
                            break

                if valid and (new_state := tuple(new_state_list)) not in seen:
                    q.append(new_state)
                    seen.add(new_state)

        clicks += 1

    raise RuntimeError("No solution found")


def compute_line(line: str) -> int:
    logging.debug(line)
    buttons, _, target_counts = parse_line(line)

    res = find_minimum_clicks_for_target_state(buttons, target_counts)
    logging.debug(res)

    return res


def main():
    res = 0

    with open("data/day_10/input") as f:
        for line in f.readlines():
            line = line.strip("\n")

            res += compute_line(line)

    logging.info(res)


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    main()
