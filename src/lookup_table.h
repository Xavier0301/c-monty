#ifndef LUT_H
#define LUT_H

#include "stdlib.h"

#include "types.h"

#define LUT_TYPE_(symbol) lookup_table_##symbol##_
#define LUT_TYPE(symbol) lookup_table_##symbol

#define DEFINE_LUT_STRUCT(symbol) \
    typedef struct LUT_TYPE_(symbol) { \
        u32 length; \
        symbol default_value; \
        symbol* data; \
    } LUT_TYPE(symbol)

DEFINE_LUT_STRUCT(u8);
DEFINE_LUT_STRUCT(i8);

#define DEFINE_LUT_INIT(symbol) \
    void lut_##symbol##_init(LUT_TYPE(symbol)* t, symbol default_value, u32 length);

DEFINE_LUT_INIT(u8);
DEFINE_LUT_INIT(i8);

#define DEFINE_LUT_LOOKUP(symbol) \
    symbol lut_##symbol##_lookup(LUT_TYPE(symbol)* t, u32 index);

DEFINE_LUT_LOOKUP(u8);
DEFINE_LUT_LOOKUP(i8);

#endif // LUT_H
