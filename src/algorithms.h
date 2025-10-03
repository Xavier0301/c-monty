#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "math.h"

#include "types.h"
#include "tensor.h"
#include "distributions.h"

// Quickselect function to find the k-th smallest element
u32 quickselect(u8* arr, u32 low, u32 high, u32 k);

#define DEFINE_SWAP(symbol) \
    void swap_##symbol(symbol* a, symbol* b)

DEFINE_SWAP(u32);
DEFINE_SWAP(u16);
DEFINE_SWAP(u8);

#endif // ALGORITHMS_H
