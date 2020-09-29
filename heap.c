// SPDX-FileCopyrightText: © 2020 Georg Sauthoff <mail@gms.tf>
// SPDX-License-Identifier: BSL-1.0

#include <stdint.h> // SIZE_MAX

#include "heap.h"


GMS_HEAP_STATIC void gms_heap_ify(void *p, size_t i, size_t n,
        Gms_Heap_Cmp_Func gt, Gms_Heap_Swap_Func swap, void *user)
{
    if (n < 2)
        return;

    size_t last = n / 2 - 1;

    // assert(i <= last);

    while (i < last) {
        size_t maxi = i;
        size_t l    = gms_heap_child(i);
        size_t r    = l + 1;
        if (gt(p, l, i))
            maxi = l;
        if (gt(p, r, maxi))
            maxi = r;
        if (maxi == i)
            return;
        swap(p, i, maxi, user);
        i = maxi;
    }
    if (i == last) {
        size_t maxi = i;
        size_t l    = gms_heap_child(i);
        size_t r    = l + 1;
        if (gt(p, l, i))
            maxi = l;
        if (r < n && gt(p, r, maxi))
            maxi = r;
        if (maxi != i)
            swap(p, i, maxi, user);
    }
}

GMS_HEAP_STATIC void gms_heap_ify_up(void *p, size_t i,
        Gms_Heap_Cmp_Func gt, Gms_Heap_Swap_Func swap, void *user)
{
    while (i > 0) {
        size_t j = gms_heap_parent(i);
        if (gt(p, i, j)) {
            swap(p, i, j, user);
            i = j;
        } else {
            break;
        }
    }
}

GMS_HEAP_STATIC void gms_heap_build(void *p, size_t n,
        Gms_Heap_Cmp_Func gt, Gms_Heap_Swap_Func swap, void *user)
{
    if (n  < 2)
        return;

    for (size_t i = n / 2 - 1; i != SIZE_MAX; --i)
        gms_heap_ify(p, i, n, gt, swap, user);
}


GMS_HEAP_STATIC bool gms_heap_is_heap(void *p, size_t i, size_t n,
        Gms_Heap_Cmp_Func gt)
{
    if (n  < 2)
        return true;

    size_t first = n / 2 - 1;
    if (i > first)
        return true;

    for (; i < first; ++i) {
        size_t l = gms_heap_child(i);
        size_t r = l + 1;
        if (gt(p, l, i))
            return false;
        if (gt(p, r, i))
            return false;
    }
    i = first;
    size_t l = gms_heap_child(i);
    size_t r = l + 1;
    if (gt(p, l, i))
        return false;
    if (r < n && gt(p, r, i))
        return false;

    return true;
}

GMS_HEAP_STATIC void gms_heap_remove(void *p, size_t i, size_t n,
        Gms_Heap_Cmp_Func gt, Gms_Heap_Move_Func mv, Gms_Heap_Swap_Func swap,
        void *user)
{
    //assert(i < n);

    --n;
    if (i == n)
        return;

    bool is_gt = gt(p, n, i);
    mv(p, i, n, user);

    if (is_gt)
        gms_heap_ify_up(p, i, gt, swap, user);
    else
        gms_heap_ify(p, i, n, gt, swap, user);
}

GMS_HEAP_STATIC void gms_heap_insert(void *p, size_t n,
        Gms_Heap_Cmp_Func gt, Gms_Heap_Swap_Func swap, void *user)
{
    gms_heap_ify_up(p, n, gt, swap, user);
}

