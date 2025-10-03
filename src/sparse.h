#ifndef SPARSE_H
#define SPARSE_H

#include "types.h"

/* SPARSE VECTOR REPRESENTATION!
    for example: with vector [0 1 1 0 0 0 0 0 1]
    we'd have sdr_t with data:
        - length = 9
        - data = [1 1 1]
        - index = [1 2 8]
*/

#define SPVEC_TYPE_(symbol) spvec_##symbol##_
#define SPVEC_TYPE(symbol) spvec_##symbol

#define DEFINE_SPVEC_STRUCT(symbol) \
    typedef struct SPVEC_TYPE_(symbol) { \
        u16 length; \
        u16 non_null_count; \
        u16* indices; \
        symbol* data; \
    } SPVEC_TYPE(symbol)

DEFINE_SPVEC_STRUCT(u8);
DEFINE_SPVEC_STRUCT(u16);
DEFINE_SPVEC_STRUCT(u32);

// implicitly any element present is a 1.
typedef struct spvec_u1_ {
        u16 length;
        u16 non_null_count;
        u16* indices;
} spvec_u1;

#define CSR_TYPE_(symbol) csr_##symbol##_
#define CSR_TYPE(symbol) csr_##symbol

#define DEFINE_CSR_STRUCT(symbol) \
    typedef struct CSR_TYPE_(symbol) { \
        symbol* rows; \
        symbol* cols; \
        u8* data; \
        symbol length; \
    } CSR_TYPE(symbol)

DEFINE_CSR_STRUCT(u8);
DEFINE_CSR_STRUCT(u16);
DEFINE_CSR_STRUCT(u32);

#define COO_TYPE_(symbol) coo_##symbol##_
#define COO_TYPE(symbol) coo_##symbol

#define COO_SUBTYPE_(symbol) coo_entry_##symbol##_
#define COO_SUBTYPE(symbol) coo_entry_##symbol

#define DEFINE_COO_SUBSTRUCT(symbol) \
    typedef struct COO_SUBTYPE_(symbol) { \
        symbol row; \
        symbol col; \
        u8 data; \
    } COO_SUBTYPE(symbol)

DEFINE_COO_SUBSTRUCT(u8);
DEFINE_COO_SUBSTRUCT(u16);
DEFINE_COO_SUBSTRUCT(u32);

#define DEFINE_COO_STRUCT(symbol) \
    typedef struct COO_TYPE_(symbol) { \
        coo_entry_##symbol* data; \
        symbol length; \
    } COO_TYPE(symbol)

DEFINE_COO_STRUCT(u8);
DEFINE_COO_STRUCT(u16);
DEFINE_COO_STRUCT(u32);

#endif
