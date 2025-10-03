#include "lookup_table.h"

#define INSTANTIATE_LUT_INIT(symbol) \
    void lut_##symbol##_init(LUT_TYPE(symbol)* t, symbol default_value, u32 length) { \
        t->default_value = default_value; \
        t->length = length; \
        t->data = (symbol*) calloc(length, sizeof(*t->data)); \
    }

INSTANTIATE_LUT_INIT(u8);
INSTANTIATE_LUT_INIT(i8);

#define INSTANTIATE_LUT_LOOKUP(symbol) \
    symbol lut_##symbol##_lookup(LUT_TYPE(symbol)* t, u32 index) { \
        if(index < t->length) return t->data[index]; \
        return t->default_value; \
    } 

INSTANTIATE_LUT_LOOKUP(u8);
INSTANTIATE_LUT_LOOKUP(i8);

