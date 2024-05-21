#!/bin/python3
from typing import List

def f1() -> List[tuple]:
    l = []
    l.append((1,2,"c3"))
    l.append((1,2,"c4"))
    return l

print(f1())
