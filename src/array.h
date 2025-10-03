#ifndef ARRAY_H
#define ARRAY_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "math.h"

#include "types.h"

#define DEFINE_ARRAY(symbol) \
    typedef struct array_##symbol##_ { \
        u32 length; \
        symbol* data; \
    } array_##symbol;

DEFINE_ARRAY(u32)
DEFINE_ARRAY(u16)
DEFINE_ARRAY(u8)

#define ARRAY(v, i) ((v).data[(i)])

#endif
