#include "algorithms.h"

#include "assertf.h"

#define SWAP_IMPLEMENTATION(symbol) \
    void swap_##symbol(symbol* a, symbol* b) { \
        int temp = *a; \
        *a = *b; \
        *b = temp; \
    }

SWAP_IMPLEMENTATION(u32)
SWAP_IMPLEMENTATION(u16)
SWAP_IMPLEMENTATION(u8)

// Partition function used in Quickselect
u32 partition(u8* array, u32 left, u32 right) {
    u32 pivot = array[right]; // Choose the last element as the pivot
    u32 i = left - 1;       // Index of the smaller element

    for (u32 j = left; j < right; ++j) {
        if (array[j] <= pivot) {
            ++i;
            swap_u8(array + i, array + j);
        }
    }

    swap_u8(array + i + 1, array + right); // Place pivot in the correct position
    return i + 1;                  // Return the index of the pivot
}

// Quickselect function to find the k-th smallest element
u32 quickselect(u8* array, u32 left, u32 right, u32 k) {
    if(left == right) return array[left];

    assertf(left <= right, "quickselect arguments incorrect: low (%u) is higher than high (%u) with k = %u", left, right, k);
    assertf(left <= k && k <= right, "quickselect argument incorrect: k (%u) is not withtin [left, right] ([%u, %u])", k, left, right);

    u32 pivotIndex = partition(array, left, right);

    if (pivotIndex == k) 
        return array[pivotIndex]; // Pivot is the k-th smallest element
    else if (pivotIndex > k) 
        return quickselect(array, left, pivotIndex - 1, k); // Recur on the left subarray
    else 
        return quickselect(array, pivotIndex + 1, right, k); // Recur on the right subarray
}
