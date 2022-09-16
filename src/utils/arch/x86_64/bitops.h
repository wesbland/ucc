/**
* Copyright (C) Mellanox Technologies Ltd. 2001-2015.  ALL RIGHTS RESERVED.
*
* See file LICENSE for terms.
*/

#ifndef UCC_X86_64_BITOPS_H_
#define UCC_X86_64_BITOPS_H_

#include <ucc/sys/compiler_def.h>
#include <stdint.h>


static UCC_F_ALWAYS_INLINE unsigned ucc_ffs32(uint32_t n)
{
    uint32_t result;
    asm("bsfl %1,%0"
        : "=r" (result)
        : "r" (n));
    return result;
}

static UCC_F_ALWAYS_INLINE unsigned ucc_ffs64(uint64_t n)
{
    uint64_t result;
    asm("bsfq %1,%0"
        : "=r" (result)
        : "r" (n));
    return result;
}

static UCC_F_ALWAYS_INLINE unsigned __ucc_ilog2_u32(uint32_t n)
{
    uint32_t result;
    asm("bsrl %1,%0"
        : "=r" (result)
        : "r" (n));
    return result;
}

static UCC_F_ALWAYS_INLINE unsigned __ucc_ilog2_u64(uint64_t n)
{
    uint64_t result;
    asm("bsrq %1,%0"
        : "=r" (result)
        : "r" (n));
    return result;
}

#endif