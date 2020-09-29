# SPDX-FileCopyrightText: © 2020 Georg Sauthoff <mail@gms.tf>
# SPDX-License-Identifier: BSL-1.0

import itertools
import pytest
import random

from heap import *

@pytest.mark.parametrize('inp', ([4, 1, 3, 2, 16, 9, 10, 14, 8, 7],
    [1, 2, 3, 4, 7, 8, 9, 10, 14, 16],
    [2, 2, 1, 16],
    [2, 16] ))
def test_build_max(inp):
    xs = build_max_int(inp)
    assert xs[0] == 16
    assert len(xs) == len(inp)
    assert list(sorted(xs)) == list(sorted(inp))
    assert not is_heap_max_int(inp)
    assert is_heap_max_int(xs)

@pytest.mark.parametrize('inp', ([4, 1, 3, 2, 16, 9, 10, 14, 8, 7],
    [10, 2, 3, 4, 7, 8, 9, 1, 14, 16],
    [2, 2, 1, 16],
    [6, 1] ))
def test_build_min(inp):
    xs = build_min_int(inp)
    assert xs[0] == 1
    assert len(xs) == len(inp)
    assert list(sorted(xs)) == list(sorted(inp))
    assert not is_heap_min_int(inp)
    assert is_heap_min_int(xs)

@pytest.mark.parametrize('inp', ([23], []))
def test_small(inp):
    assert build_min_int(inp) == inp
    assert build_max_int(inp) == inp
    assert is_heap_min_int(inp)
    assert is_heap_max_int(inp)

def test_all():
    for inp in itertools.permutations(range(8)):
        xs = build_max_int(inp)
        assert is_heap_max_int(xs)
        assert list(sorted(xs)) == list(sorted(inp))
        assert xs[0] == max(inp)


def test_rand():
    pop = list(range(100000))
    for i in range(10):
        n = random.randint(100, 128 * 1024)
        inp = random.choices(pop, k=n)
        xs = build_max_int(inp)
        assert is_heap_max_int(xs)
        assert list(sorted(xs)) == list(sorted(inp))
        assert xs[0] == max(inp)

def test_insert():
    xs = []
    inp = [1, 23, 6, 17]
    for x in inp:
        xs = insert_max_int(xs, x)
    assert is_heap_max_int(xs)
    assert list(sorted(xs)) == list(sorted(inp))


def test_remove():
    inp = [1, 23, 6, 17]
    xs = build_max_int(inp)
    ys = []
    for i in (2, 0, 1, 0):
        x = xs[i]
        ys.append(x)
        xs = remove_max_int(xs, i)
        assert x not in xs
    assert list(sorted(ys)) == list(sorted(inp))

