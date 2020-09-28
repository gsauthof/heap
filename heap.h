#ifndef GMS_HEAP_H
#define GMS_HEAP_H

// SPDX-FileCopyrightText: © 2020 Georg Sauthoff <mail@gms.tf>
// SPDX-License-Identifier: BSL-1.0

#include <stddef.h> // size_t
#include <stdbool.h>

static inline size_t gms_heap_parent(size_t i)
{
    return (i - 1) / 2;
}

// returns left child
// +1 for the right child
static inline size_t gms_heap_child(size_t i)
{
    return 2 * i + 1;
}

typedef bool (*Gms_Heap_Cmp_Func) (const void *p, size_t i, size_t j);
typedef void (*Gms_Heap_Move_Func)(      void *p, size_t i, size_t j, void *user);
typedef void (*Gms_Heap_Swap_Func)(      void *p, size_t i, size_t j, void *user);

#if GMS_HEAP_HEADER_ONLY
#define GMS_HEAP_STATIC static inline
#else
#define GMS_HEAP_STATIC
#endif

GMS_HEAP_STATIC void gms_heap_ify   (void *p, size_t i, size_t n,
        Gms_Heap_Cmp_Func gt, Gms_Heap_Swap_Func swap, void *user);
GMS_HEAP_STATIC void gms_heap_ify_up(void *p, size_t i,
        Gms_Heap_Cmp_Func gt, Gms_Heap_Swap_Func swap, void *user);

GMS_HEAP_STATIC bool gms_heap_is_heap(void *p, size_t i, size_t n,
        Gms_Heap_Cmp_Func gt);

GMS_HEAP_STATIC void gms_heap_build  (void *p, size_t n,
        Gms_Heap_Cmp_Func gt, Gms_Heap_Swap_Func swap, void *user);

GMS_HEAP_STATIC void gms_heap_insert (void *p, size_t *n,
        Gms_Heap_Cmp_Func gt, Gms_Heap_Swap_Func swap, void *user);
GMS_HEAP_STATIC void gms_heap_remove(void *p, size_t i, size_t *n,
        Gms_Heap_Cmp_Func gt, Gms_Heap_Move_Func mv, Gms_Heap_Swap_Func swap,
        void *user);

#if GMS_HEAP_HEADER_ONLY
#include "heap.c"
#endif

#endif
