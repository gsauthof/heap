#ifndef GMS_HEAP_H
#define GMS_HEAP_H

// SPDX-FileCopyrightText: © 2020 Georg Sauthoff <mail@gms.tf>
// SPDX-License-Identifier: BSL-1.0

#include <stddef.h> // size_t
#include <stdbool.h>


typedef size_t (*Gms_Heap_Idx_Func)(bool lookup_parent, size_t i);
typedef bool (*Gms_Heap_Cmp_Func)  (const void *p, size_t i, size_t j);
typedef void (*Gms_Heap_Move_Func) (      void *p, size_t i, size_t j, void *user);
typedef void (*Gms_Heap_Swap_Func) (      void *p, size_t i, size_t j, void *user);

#if GMS_HEAP_HEADER_ONLY
#define GMS_HEAP_STATIC static inline
#else
#define GMS_HEAP_STATIC
#endif

GMS_HEAP_STATIC size_t gms_heap_parent_k(size_t i, size_t k);
GMS_HEAP_STATIC size_t gms_heap_child_k(size_t i, size_t k);

GMS_HEAP_STATIC size_t gms_heap_parent(size_t i);
GMS_HEAP_STATIC size_t gms_heap_child(size_t i);
GMS_HEAP_STATIC size_t gms_heap_idx(bool lookup_parent, size_t i);

GMS_HEAP_STATIC size_t gms_heap_level_start_k(size_t i, size_t k);
GMS_HEAP_STATIC size_t gms_bheap_child(size_t i, size_t page_size);
GMS_HEAP_STATIC size_t gms_bheap_parent(size_t iP, size_t page_size);

GMS_HEAP_STATIC size_t gms_heap_next(size_t i);
GMS_HEAP_STATIC size_t gms_bheap_next(size_t i, size_t page_size);

GMS_HEAP_STATIC void gms_heap_ify   (void *p, size_t i, size_t n,
        Gms_Heap_Idx_Func idx,
        Gms_Heap_Cmp_Func gt, Gms_Heap_Swap_Func swap, void *user);
GMS_HEAP_STATIC void gms_heap_ify_up(void *p, size_t i,
        Gms_Heap_Idx_Func idx,
        Gms_Heap_Cmp_Func gt, Gms_Heap_Swap_Func swap, void *user);

GMS_HEAP_STATIC bool gms_heap_is_heap(void *p, size_t i, size_t n,
        Gms_Heap_Cmp_Func gt);

GMS_HEAP_STATIC void gms_heap_build  (void *p, size_t n,
        Gms_Heap_Cmp_Func gt, Gms_Heap_Swap_Func swap, void *user);

GMS_HEAP_STATIC void gms_heap_insert (void *p, size_t n,
        Gms_Heap_Idx_Func idx,
        Gms_Heap_Cmp_Func gt, Gms_Heap_Swap_Func swap, void *user);
GMS_HEAP_STATIC void gms_heap_remove(void *p, size_t i, size_t n,
        Gms_Heap_Idx_Func idx,
        Gms_Heap_Cmp_Func gt, Gms_Heap_Move_Func mv, Gms_Heap_Swap_Func swap,
        void *user);

#if GMS_HEAP_HEADER_ONLY
#include "heap.c"
#endif

#endif
