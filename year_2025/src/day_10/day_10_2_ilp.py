import re
import logging
import numpy as np
from scipy.optimize import LinearConstraint, milp, Bounds


def parse_line(line: str) -> tuple[np.ndarray, np.ndarray]:
    pattern = re.compile(r"\[(.*)\] (.*) \{(.*)\}")

    matching = pattern.match(line)
    if not matching:
        raise RuntimeError("Input did not match pattern.")

    light = matching.group(1)
    button_idx_sets = [x.strip("()").split(",") for x in matching.group(2).split(" ")]
    jolt = matching.group(3)

    n_light = len(light)

    buttons = []
    for button_idx_set in button_idx_sets:
        button = np.zeros(n_light)
        for button_idx in button_idx_set:
            button[int(button_idx)] = 1
        buttons.append(button)
    buttons = np.array(buttons).T

    target_counts = np.array([int(x) for x in jolt.split(",")])

    return buttons, target_counts


def find_minimum_clicks_for_target_state(
    buttons: np.ndarray, target_counts: np.ndarray
):
    n_lights, n_buttons = buttons.shape

    c = np.ones(n_buttons)

    integrality = np.ones(n_buttons)
    constraints = LinearConstraint(buttons, target_counts, target_counts)
    bounds = Bounds(0, np.inf)

    res = int(
        milp(c, constraints=constraints, bounds=bounds, integrality=integrality).fun
    )

    return res


def compute_line(line: str) -> int:
    logging.debug(line)
    buttons, target_counts = parse_line(line)

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
