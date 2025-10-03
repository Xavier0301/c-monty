#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "types.h"
#include "tensor.h"
#include "io.h"

void read_dataset(const char* filename, mat_u8* dataset, u32* num_samples, u32* sample_size);
void read_dataset_partial(const char* filename, mat_u8* dataset, u32 num_samples_to_fetch, u32* num_samples_total, u32* sample_size);

#define DECLARE_READ_MATRIX(type) \
    void read_matrix_##type(FILE* f, mat_##type* matrix, u32 size);
DECLARE_READ_MATRIX(u8)
DECLARE_READ_MATRIX(u16)

#define DECLARE_READ_TENSOR(type) \
    void read_tensor_##type(FILE* f, tensor_##type* tensor, u32 size);
DECLARE_READ_TENSOR(u8)
DECLARE_READ_TENSOR(u16)

void write_dataset(const char* filename, mat_u8* dataset, u32 num_samples, u32 sample_size);

#define DECLARE_WRITE_MATRIX(type) \
    void write_matrix_##type(FILE* f, mat_##type* matrix, u32 size);
DECLARE_WRITE_MATRIX(u8)
DECLARE_WRITE_MATRIX(u16)

#define DECLARE_WRITE_TENSOR(type) \
    void write_tensor_##type(FILE* f, tensor_##type* tensor, u32 size);
DECLARE_WRITE_TENSOR(u8)
DECLARE_WRITE_TENSOR(u16)

#endif // DATA_MANAGER_H
