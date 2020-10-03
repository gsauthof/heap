#ifndef GMS_HEAP_HH
#define GMS_HEAP_HH

// SPDX-FileCopyrightText: © 2020 Georg Sauthoff <mail@gms.tf>
// SPDX-License-Identifier: BSL-1.0

#include <utility>

#define GMS_HEAP_HEADER_ONLY 1
#include "heap.h"


namespace gms {
    namespace heap {

        template<typename T>
            struct Max {
                static bool cmp(const void *p, size_t i, size_t j)
                {
                    const T *a = static_cast<const T*>(p) + i;
                    const T *b = static_cast<const T*>(p) + j;
                    return *a > *b;
                }
            };
        template<typename T>
            struct Min {
                static bool cmp(const void *p, size_t i, size_t j)
                {
                    const T *a = static_cast<const T*>(p) + i;
                    const T *b = static_cast<const T*>(p) + j;
                    return *a < *b;
                }
            };
        template<typename T>
            struct Swap {
                static void swap(void *p, size_t i, size_t j, void *)
                {
                    T *a = static_cast<T*>(p) + i;
                    T *b = static_cast<T*>(p) + j;
                    using std::swap;
                    swap(*a, *b);
                }
            };
        template<typename T>
            struct Move {
                static void move(void *p, size_t i, size_t j, void *)
                {
                    T *a = static_cast<T*>(p) + i;
                    T *b = static_cast<T*>(p) + j;
                    using std::move;
                    *a = move(*b);
                }
            };


        template<typename T>
            inline
            bool is_heap(T *p, size_t i, size_t n)
            {
                return gms_heap_is_heap(p, i, n, Max<T>::cmp);
            }
        template<typename T, typename Cmp>
            inline
            bool is_heap(T *p, size_t i, size_t n, Cmp)
            {
                return gms_heap_is_heap(p, i, n, Cmp::cmp);
            }

        template<typename T>
            inline
            void build(T *p, size_t n)
            {
                gms_heap_build(p, n, Max<T>::cmp, Swap<T>::swap, nullptr);
            }
        template<typename T, typename Cmp, typename Swap>
            inline
            void build(T *p, size_t n, Cmp, Swap)
            {
                gms_heap_build(p, n, Cmp::cmp, Swap::swap, nullptr);
            }

        template<typename T>
            inline
            void insert(T *p, size_t n)
            {
                gms_heap_insert(p, n, gms_heap_idx, Max<T>::cmp, Swap<T>::swap, nullptr);
            }
        template<typename T, typename Cmp, typename Swap>
            inline
            void insert(T *p, size_t n, Cmp, Swap, void *user)
            {
                gms_heap_insert(p, n, gms_heap_idx, Cmp::cmp, Swap::swap, user);
            }

        template<typename T>
            inline
            void remove(T *p, size_t i, size_t n)
            {
                gms_heap_remove(p, i, n, gms_heap_idx, Max<T>::cmp, Move<T>::move,
                        Swap<T>::swap, nullptr);
            }
        template<typename T, typename Cmp, typename Move, typename Swap>
            inline
            void remove(T *p, size_t i, size_t n, Cmp, Move, Swap, void *user)
            {
                gms_heap_remove(p, i, n, gms_heap_idx, Cmp::cmp, Move::move,
                        Swap::swap, user);
            }

    }
}

#endif
