#include "encoder.h"

/**
 * @brief 
 * 
 * @param output need to be malloc-ed by the callers
 * @param input 
 * @param range 
 * @param num_bits 
 * @param num_active_bits 
 */
void encode_integer(u8* output, u32 input, pair_u32 range, u32 num_bits, u32 num_active_bits) {
    u32 num_buckets = num_bits - num_active_bits + 1;

    u32 i = (num_buckets * (input - range.first)) / (range.second - range.first);

    // Let's calculate the output on the fly 
    // We could cache these results or pre-compute them so that we only need 
    //      to calculate i and then copy the cached result to the output array
    // Transforms a bunch of logic and arithmetic into 1 copy operation, nothing major saved
    // HOWEVER, if we can just pass around the POINTER to the sdr, then it's pretty major
    //      since it transforms a O(num_bits) into a O(1)
    // However this requires that the output stay unchanged in the later stages
    //      which I believe it is!

    for(u32 j = 0; j < num_bits; ++j) {
        output[j] = j >= i && j < i + num_active_bits;
    }
}
