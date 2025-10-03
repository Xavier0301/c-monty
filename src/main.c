#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <unistd.h> // For sleep function

#include "grid_environment.h"
#include "sensor_module.h"
#include "learning_module.h"
#include "motor_policy.h"

int main(int argc, char *argv[]) {  

// Put the error message in a char array:
    const char error_message[] = "Error: usage: %s X\n\t \
        0 is for training from scratch\n";


    /* Error Checking */
    if(argc < 0) {
        printf(error_message, argv[0]);
        exit(1);
    }

    u32 num_step = 10;

    grid_t env;
    u32 env_sidelen = 10;
    init_grid_env(&env, env_sidelen, env_sidelen);
    populate_grid_env_random(&env);

    grid_t patch; // grid_t is an ugly abstraction for the patch
    u32 patch_sidelen = 3;
    init_grid_env(&patch, patch_sidelen, patch_sidelen);
    vec2d patch_center = (vec2d) {.x = patch_sidelen / 2, .y = patch_sidelen / 2};

    bounds_t bounds = get_bounds(env_sidelen, env_sidelen, patch_sidelen, patch_sidelen);

    vec2d agent_location = {.x = 5, .y = 1}; // start location

    random_motor_policy_t motor_policy;
    init_random_motor_policy(&motor_policy, agent_location, bounds, num_step);

    features_t f;
    pose_t p;

    grid_lm lm;
    init_learning_module(&lm);

    vec2d movement;

    print_grid(&env);

    for(u32 step = 0; step < num_step; ++step) {
        printf("--- step %u: agent at location (%u, %u)\n", step, agent_location.x, agent_location.y);
        extract_patch(&patch, &env, agent_location, patch_sidelen);

        print_grid(&patch);

        sensor_module(&f, &p, patch, patch_center);

        print_features(f);
        print_pose(p);

        learning_module_explore(&lm, f, p); // TODO: during matching, the lm should override the motor_policy's movements

        movement = random_motor_policy(&motor_policy, f, p);

        agent_location.x += movement.x;
        agent_location.y += movement.y;

        printf("\n");
    }

    return 0;
}
