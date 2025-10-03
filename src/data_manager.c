#include "data_manager.h"

#define READ_PTR(ptr, fp) FREAD_CHECK(ptr, sizeof(*ptr), 1, fp)
#define READ_FIELD(structure, name, fp) FREAD_CHECK(&(structure)->name, sizeof((structure)->name), 1, fp)
#define READ_BUFFER(structure, name, num, fp) FREAD_CHECK((structure)->name, sizeof(*(structure)->name), num, fp)

void read_dataset(const char* filename, mat_u8* dataset, u32* num_samples, u32* sample_size) {
    FILE* f = fopen(filename, "r");

    READ_PTR(num_samples, f);
    READ_PTR(sample_size, f);

    read_matrix_u8(f, dataset, *num_samples * *sample_size);

    fclose(f);
}

void read_dataset_partial(const char* filename, mat_u8* dataset, u32 num_samples_to_fetch, u32* num_samples_total, u32* sample_size) {
    FILE* f = fopen(filename, "r");

    READ_PTR(num_samples_total, f);
    READ_PTR(sample_size, f);

    assert(num_samples_to_fetch <= *num_samples_total);

    read_matrix_u8(f, dataset, num_samples_to_fetch * *sample_size);

    fclose(f);
}

#define DEFINE_READ_MATRIX(type) \
    void read_matrix_##type(FILE* f, mat_##type* matrix, u32 size) { \
        READ_FIELD(matrix, rows, f); \
        READ_FIELD(matrix, cols, f); \
        READ_BUFFER(matrix, data, size, f); \
    }

DEFINE_READ_MATRIX(u8)
DEFINE_READ_MATRIX(u16)

#define DEFINE_READ_TENSOR(type) \
    void read_tensor_##type(FILE* f, tensor_##type* tensor, u32 size) { \
        READ_FIELD(tensor, stride1, f); \
        READ_FIELD(tensor, shape1, f); \
        READ_FIELD(tensor, shape2, f); \
        READ_FIELD(tensor, shape3, f); \
        READ_BUFFER(tensor, data, size, f); \
    }

DEFINE_READ_TENSOR(u8)
DEFINE_READ_TENSOR(u16)

#define SAVE_VAR(var, fp) FWRITE_CHECK(&var, sizeof(var), 1, fp)
#define SAVE_FIELD(structure, name, fp) FWRITE_CHECK(&(structure)->name, sizeof((structure)->name), 1, fp)
#define SAVE_BUFFER(structure, name, num, fp) FWRITE_CHECK((structure)->name, sizeof(*(structure)->name), num, fp)

void write_dataset(const char* filename, mat_u8* dataset, u32 num_samples, u32 sample_size) {
    FILE* f = fopen(filename, "w");

    SAVE_VAR(num_samples, f);
    SAVE_VAR(sample_size, f);

    write_matrix_u8(f, dataset, num_samples * sample_size);

    fclose(f);
}

#define DEFINE_WRITE_MATRIX(type) \
    void write_matrix_##type(FILE* f, mat_##type* matrix, u32 size) { \
        SAVE_FIELD(matrix, rows, f); \
        SAVE_FIELD(matrix, cols, f); \
        SAVE_BUFFER(matrix, data, size, f); \
    }
DEFINE_WRITE_MATRIX(u8)
DEFINE_WRITE_MATRIX(u16)

#define DEFINE_WRITE_TENSOR(type) \
    void write_tensor_##type(FILE* f, tensor_##type* tensor, u32 size) { \
        SAVE_FIELD(tensor, stride1, f); \
        SAVE_FIELD(tensor, shape1, f); \
        SAVE_FIELD(tensor, shape2, f); \
        SAVE_FIELD(tensor, shape3, f); \
        SAVE_BUFFER(tensor, data, size, f); \
    }
DEFINE_WRITE_TENSOR(u8)
DEFINE_WRITE_TENSOR(u16)
