#ifndef TENSOR_H
#define TENSOR_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "math.h"

#include "types.h"

#define DATA_TYPE(symbol) symbol

#define TENSOR_TYPE_(symbol) tensor_##symbol##_
#define TENSOR_TYPE(symbol) tensor_##symbol

// stride2 = shape3
#define DEFINE_TENSOR_STRUCT(symbol) \
    typedef struct TENSOR_TYPE_(symbol) { \
        u32 stride1; \
        u32 shape1; \
        u32 shape2; \
        u32 shape3; \
        DATA_TYPE(symbol)* data; \
    } TENSOR_TYPE(symbol)

DEFINE_TENSOR_STRUCT(u32);
DEFINE_TENSOR_STRUCT(u16);
DEFINE_TENSOR_STRUCT(u8);

#define TENSOR3D_AXIS1(t, i) ((t).data + i * (t).stride1)
#define TENSOR3D_AXIS2(t, i, j) (TENSOR3D_AXIS1(t, i) + j * (t).shape3)
#define TENSOR3D(t, i, j, k) (TENSOR3D_AXIS2(t, i, j) + k)

#define TENSOR_INIT(t, shape1, shape2, shape3, type) \
    do { \
        t->stride1 = shape2 * shape3; \
        t->shape1 = shape1; \
        t->shape2 = shape2; \
        t->shape3 = shape3; \
        t->data = (type*) calloc(shape1 * shape2 * shape3, sizeof(*t->data)); \
    } while(0)

#define BUFFER_TO_TENSOR(type, buffer, shape1, shape2, shape3) \
    (type) { \
        .data = buffer, \
        .stride1 = shape2 * shape3, \
        .shape1 = shape1, \
        .shape2 = shape2, \
        .shape3 = shape3, \
    }

#define MAT_TO_TENSOR(type, mat) \
    (type) { \
        .data = mat.data, \
        .stride1 = mat.rows * mat.cols, \
        .shape1 = 1, \
        .shape2 = mat.rows, \
        .shape3 = mat.cols, \
    }

#define TENSOR_PRINT(t, shape1, shape2, shape3) \
    do { \
        for(u32 i = 0; i < shape1; ++i) { \
            for(u32 j = 0; j < shape2; ++j) { \
                for(u32 k = 0; k < shape3; ++k) \
                    printf("%u ", *TENSOR3D(t, i, j, k)); \
                printf("\n"); \
            } \
            printf("\n"); \
        } \
    } while(0)

#define DEFINE_TENSOR_INIT(symbol) \
    void tensor_##symbol##_init(TENSOR_TYPE(symbol)* t, u32 shape1, u32 shape2, u32 shape3);

DEFINE_TENSOR_INIT(u32);
DEFINE_TENSOR_INIT(u16);
DEFINE_TENSOR_INIT(u8);

#define MAT_TYPE_(symbol) mat_##symbol##_
#define MAT_TYPE(symbol) mat_##symbol

// cols = stride
#define DEFINE_MATRIX_STRUCT(symbol) \
    typedef struct MAT_TYPE_(symbol) { \
        u32 rows; \
        u32 cols; \
        DATA_TYPE(symbol)* data; \
    } MAT_TYPE(symbol)

DEFINE_MATRIX_STRUCT(u32);
DEFINE_MATRIX_STRUCT(u16);
DEFINE_MATRIX_STRUCT(u8);

#define MAT(t, i, j) ((t).data[((i) * (t).cols) + (j)])
#define MATP(t, i, j) ((t).data + ((i) * (t).cols) + (j))

#define MATRIX_INIT(m, rows, cols, type) \
    do { \
        (m)->rows = rows; \
        (m)->cols = cols; \
        (m)->data = (type*) calloc(rows * cols, sizeof(*(m)->data)); \
    } while(0)

#define MATRIX_PRINT(m, rows, cols) \
    do { \
        for(size_t i = 0; i < rows; ++i) { \
            for(size_t j = 0; j < cols; ++j) \
                printf("%u ", MAT(*m, i, j)); \
            printf("\n"); \
        } \
    } while(0)

#define DEFINE_MATRIX_INIT(symbol) \
    void matrix_##symbol##_init(MAT_TYPE(symbol)* m, u32 rows, u32 cols)

DEFINE_MATRIX_INIT(u32);
DEFINE_MATRIX_INIT(u16);
DEFINE_MATRIX_INIT(u8);

u8 mat_u8_min(mat_u8 m);
u8 mat_u8_max(mat_u8 m);
u8 mat_u8_mean(mat_u8 m); 

#endif
