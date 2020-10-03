// SPDX-FileCopyrightText: © 2020 Georg Sauthoff <mail@gms.tf>
// SPDX-License-Identifier: BSL-1.0

#include <stdint.h> // SIZE_MAX

#include "heap.h"

enum { CHILD, PARENT };

GMS_HEAP_STATIC size_t gms_heap_parent_k(size_t i, size_t k)
{
    return (i - 1) / k;
}

// returns left child
// +1 for the right child, etc.
GMS_HEAP_STATIC size_t gms_heap_child_k(size_t i, size_t k)
{
    return k * i + 1;
}

GMS_HEAP_STATIC size_t gms_heap_parent(size_t i)
{
    return gms_heap_parent_k(i, 2);
}

// returns left child
// +1 for the right child
GMS_HEAP_STATIC size_t gms_heap_child(size_t i)
{
    return gms_heap_child_k(i, 2);
}

GMS_HEAP_STATIC size_t gms_heap_idx(bool lookup_parent, size_t i)
{
    if (lookup_parent)
        return gms_heap_parent(i);
    else
        return gms_heap_child(i);
}

// we are assuming sane sizes here, i.e. page_size and element size
// are a power of two
// NB: - page size is specified in #elements that fit into a page
//     - i indexes elements (not bytes)
//     - for the first page: the last element isn't used
//     - for other pages: the first and last element aren't used
GMS_HEAP_STATIC size_t gms_bheap_child(size_t iP, size_t page_size)
{
    size_t page = iP / page_size;
    size_t page_start = page * page_size;
    size_t next_page_start = (page + 1) * page_size;

    size_t i = iP - page_start;

    size_t c = gms_heap_child(i);
    if (c  + 1 < next_page_start)
        return page_start + c;

    // /2 -> we put both siblings into one page
    size_t next_level_off = (c - (page_size - 1)) / 2;

    // #leaves in the last level of the page
    size_t page_leaves = page_size / 2;

    size_t k = gms_heap_child_k(page, page_leaves);

    // skip root node element in that page
    size_t x = (k + next_level_off) * page_size + 1;

    return x;
}


GMS_HEAP_STATIC size_t gms_bheap_parent(size_t iP, size_t page_size)
{
    size_t page = iP / page_size;
    size_t page_start = page * page_size;

    size_t i = iP - page_start;

    size_t p = gms_heap_parent(i);
    if (p || !page)
        return page_start + p;

    size_t page_leaves = page_size / 2;

    size_t q = gms_heap_parent_k(page, page_leaves);

    size_t off = (page - 1) % page_leaves;

    size_t x = q * page_size + page_size - 1 - page_leaves + off;

    return x;
}

GMS_HEAP_STATIC size_t gms_heap_next(size_t i)
{
    return i + 1;
}

GMS_HEAP_STATIC size_t gms_bheap_next(size_t i, size_t page_size)
{
    ++i;
    size_t k = i % page_size;
    if (!k)
        ++i;
    else if (k + 1 == page_size)
        i += 2;
    return i;
}


GMS_HEAP_STATIC void gms_heap_ify(void *p, size_t i, size_t n,
        Gms_Heap_Idx_Func idx,
        Gms_Heap_Cmp_Func gt, Gms_Heap_Swap_Func swap, void *user)
{
    if (n < 2)
        return;

    for (;;) {
        size_t maxi = i;
        size_t l    = idx(CHILD, i);
        size_t r    = l + 1;
        if (l < n && gt(p, l, i))
            maxi = l;
        if (r < n && gt(p, r, maxi))
            maxi = r;
        if (maxi == i)
            return;
        swap(p, i, maxi, user);
        i = maxi;
    }
}

GMS_HEAP_STATIC void gms_heap_ify_up(void *p, size_t i,
        Gms_Heap_Idx_Func idx,
        Gms_Heap_Cmp_Func gt, Gms_Heap_Swap_Func swap, void *user)
{
    while (i > 0) {
        size_t j = idx(PARENT, i);
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
        gms_heap_ify(p, i, n, gms_heap_idx, gt, swap, user);
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
        Gms_Heap_Idx_Func idx,
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
        gms_heap_ify_up(p, i, idx, gt, swap, user);
    else
        gms_heap_ify(p, i, n, idx, gt, swap, user);
}

GMS_HEAP_STATIC void gms_heap_insert(void *p, size_t n,
        Gms_Heap_Idx_Func idx,
        Gms_Heap_Cmp_Func gt, Gms_Heap_Swap_Func swap, void *user)
{
    gms_heap_ify_up(p, n, idx, gt, swap, user);
}

