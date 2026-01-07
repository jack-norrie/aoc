import logging
import heapq
from operator import mul
from functools import reduce


def read_data(fpath: str) -> list[str]:
    with open(fpath, "r") as f:
        data = [line.strip("\n") for line in f.readlines()]

    return data


def get_vertices(data: list[str]):
    return [tuple(int(x) for x in line.split(",")) for line in data]


def get_distance(x: tuple[int, ...], y: tuple[int, ...]) -> float:
    return sum((a - b) ** 2 for a, b in zip(x, y))


def get_edges(verticies: list[tuple[int, ...]]):
    n = len(verticies)

    edges = []
    for i in range(n):
        for j in range(i + 1, n):
            u = verticies[i]
            v = verticies[j]
            d = get_distance(u, v)
            edges.append((d, u, v))

    return edges


class UnionFind:
    def __init__(self, verticies) -> None:
        self.ranks = {v: 0 for v in verticies}
        self.rep = {v: v for v in verticies}
        self.n_components = len(verticies)

    def find(self, u):
        if (rep := self.rep[u]) != u:
            self.rep[u] = self.find(rep)
        return self.rep[u]

    def union(self, u, v) -> bool:
        rep_u = self.find(u)
        rep_v = self.find(v)

        if rep_u == rep_v:
            return False

        rank_u = self.ranks[rep_u]
        rank_v = self.ranks[rep_v]

        if rank_u > rank_v:
            self.rep[rep_v] = rep_u
        elif rank_u < rank_v:
            self.rep[rep_u] = rep_v
        else:
            self.rep[rep_u] = rep_v
            self.ranks[rep_v] += 1

        self.n_components -= 1

        return True

    def component_sizes(self):
        counter = {}
        for v in self.ranks:
            rep = self.find(v)
            counter[rep] = counter.get(rep, 0) + 1
        return counter


def solve(vertices, edges: list[tuple[float, tuple[int, ...], tuple[int, ...]]]) -> int:
    heapq.heapify(edges)

    uf = UnionFind(vertices)
    while uf.n_components > 1:
        _, u, v = heapq.heappop(edges)

        if uf.union(u, v):
            logging.debug(f"Edge added: {u}-{v}")

    return u[0] * v[0]


def main():
    data = read_data("data/day_8/input")
    logging.debug("Raw Data")
    for line in data:
        logging.debug(line)

    vertices = get_vertices(data)
    logging.debug(vertices)

    edges = get_edges(vertices)
    logging.debug(edges)

    res = solve(vertices, edges)
    logging.info(res)


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    main()
