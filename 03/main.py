#!/usr/bin/env python3

import re
import sys

instr_regex = re.compile(r"(do\(\)|don't\(\)|mul[(](\d{1,3}),(\d{1,3})[)])")

total          = 0
filtered_total = 0
mul_enabled    = True
# input() stops at the end of a line. We use sys.stdin.read() to get
# everything until EOF.
for (op, a, b) in instr_regex.findall(sys.stdin.read()):
    match op:
        case r"do()":
            mul_enabled = True
        case r"don't()":
            mul_enabled = False
        case _:
            total += int(a) * int(b)
            if mul_enabled:
                filtered_total += int(a) * int(b)

print(total, filtered_total)
