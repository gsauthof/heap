#ifndef GMS_INT_MATH_H
#define GMS_INT_MATH_H

// SPDX-FileCopyrightText: © 2020 Georg Sauthoff <mail@gms.tf>
// SPDX-License-Identifier: BSL-1.0

static inline unsigned log2_zu(size_t x)
{
    if (sizeof x == 8)
        return sizeof x * 8 - __builtin_clzll(x) - 1;
    else
        return sizeof x * 8 - __builtin_clz(x) - 1;
}

static inline unsigned logk_zu(unsigned k, size_t x)
{
    return log2_zu(x) / log2_zu(k);
}

// i.e. k^x = (2^log_2(k))^x = 2^{log_2(k)*x}
static inline size_t pow_zu(size_t k, size_t x)
{
    size_t r = 1 << ( log2_zu(k) * x);
    return r;
}


#endif
