#!/usr/bin/env python3

import sys

grid = [[int(c) for c in line.rstrip()] for line in sys.stdin.readlines()]

width  = len(grid[0])
height = len(grid)

def reachable_from(x, y):
    queue     = [(x, y)]
    reachable = set()
    while queue:
        spot = queue.pop(0)
        spotx, spoty = spot
        for xprime, yprime in adjs(*spot):
            if grid[yprime][xprime] == 9 and grid[spoty][spotx] == 8:
                reachable.add((xprime, yprime))
            elif grid[yprime][xprime] == grid[spoty][spotx] + 1:
                queue.append((xprime, yprime))

    return len(reachable)

def trails(x, y):
    result = 0
    for xprime, yprime in adjs(x, y):
        if grid[yprime][xprime] == 9 and grid[y][x] == 8:
            result += 1
        elif grid[yprime][xprime] == grid[y][x] + 1:
            result += trails(xprime, yprime)
    return result

def adjs(x, y):
    def in_bounds(test):
        xprime, yprime = test
        return 0 <= xprime < width and 0 <= yprime < width

    return filter(in_bounds, [(x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)])

score  = 0
rating = 0
for y, row in enumerate(grid):
    for x, d in enumerate(row):
        if d == 0:
            score  += reachable_from(x, y)
            rating += trails(x, y)

print(score, rating)
