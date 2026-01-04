import re
import math
from collections import deque
import logging
from functools import lru_cache


def parse_line(line: str) -> tuple[tuple[int, ...], int, tuple[int, ...]]:
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
    buttons = tuple(buttons)

    target_state = 0
    for i, c in enumerate(light):
        if c == "#":
            target_state |= 1 << i

    target_counts = tuple([int(x) for x in jolt.split(",")])

    return buttons, target_state, target_counts


@lru_cache(None)
def find_all_button_clicks_and_combos_for_target_state(
    buttons: tuple[int], target_state: int
) -> list:
    res = []

    n_buttons = len(buttons)

    # state, button_mask, next_button_idx
    q = deque([(0, 0, 0)])

    clicks = 0
    while q:
        for _ in range(len(q)):
            cur_state, button_combo, next_button_idx = q.popleft()

            if cur_state == target_state:
                res.append((clicks, button_combo))

            for i in range(next_button_idx, n_buttons):
                button = buttons[i]
                next_button_combo = button_combo | (1 << i)
                q.append((cur_state ^ button, next_button_combo, i + 1))

        clicks += 1

    return res


def find_minimum_number_of_button_presses_for_target_counts(
    buttons: tuple[int, ...], target_counts: tuple[int, ...]
) -> int:
    n_lights = len(target_counts)
    n_buttons = len(buttons)

    zero_vector = (0,) * n_lights

    @lru_cache(None)
    def rec(target_counts: tuple[int, ...]) -> int:
        if target_counts == zero_vector:
            return 0

        target_state = 0
        for i, count in enumerate(target_counts):
            if count % 2:
                target_state |= 1 << i

        res = 1_000_000
        button_clicks_and_combos = find_all_button_clicks_and_combos_for_target_state(
            buttons, target_state
        )
        for clicks, button_combo in button_clicks_and_combos:
            next_target_counts = list(target_counts)

            valid = True
            for i in range(n_lights):
                for j in range(n_buttons):
                    if buttons[j] & (1 << i) and button_combo & (1 << j):
                        next_target_counts[i] -= 1

                if next_target_counts[i] % 2 == 1 or next_target_counts[i] < 0:
                    valid = False
                    break
                else:
                    next_target_counts[i] //= 2

            if valid:
                res = min(res, clicks + 2 * rec(tuple(next_target_counts)))

        return res

    return rec(target_counts)


def compute_line(line: str) -> int:
    logging.debug(line)
    buttons, _, target_counts = parse_line(line)

    res = find_minimum_number_of_button_presses_for_target_counts(
        buttons, target_counts
    )
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
