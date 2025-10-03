#include "motor_policy.h"

#include "stdlib.h"
#include "distributions.h"

void init_random_motor_policy(random_motor_policy_t* policy, vec2d start_location, bounds_t bounds, u32 steps) {
    policy->pregenerated_movements = calloc(steps, sizeof(*policy->pregenerated_movements));
    policy->current_step = 0;

    reset_random_motor_policy(policy, start_location, bounds, steps);
}

void reset_random_motor_policy(random_motor_policy_t* policy, vec2d start_location, bounds_t bounds, u32 steps) {
    vec2d last_location = { .x = start_location.x, .y = start_location.y };
    vec2d next_location;
    for(u32 i = 0; i < steps; ++i) {
        next_location.x = (i32) unif_rand_range_u32(bounds.min_x, bounds.max_x);
        next_location.y = (i32) unif_rand_range_u32(bounds.min_y, bounds.max_y);

        vec2d movement;
        movement.x = next_location.x - last_location.x;
        movement.y = next_location.y - last_location.y;

        printf("movement %u is (%d, %d) between (%d, %d) and (%d, %d)\n", i, movement.x, movement.y, last_location.x, last_location.y, next_location.x, next_location.y);

        policy->pregenerated_movements[i] = movement;

        last_location.x = next_location.x;
        last_location.y = next_location.y;
    }
}

/**
 * @brief 
 * 
 * @returns vec2d representing the movement
 * 
 * @param features ignored in this policy, here for the signature
 * @param pose ignored in this policy, here for the signature
 */
vec2d random_motor_policy(random_motor_policy_t* policy, features_t features, pose_t pose) {
    vec2d movement = policy->pregenerated_movements[policy->current_step];
    policy->current_step += 1;

    return movement;
}
