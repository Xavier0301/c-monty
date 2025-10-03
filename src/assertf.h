#ifndef ASSERTF_H
#define ASSERTF_H

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

// ... is ##__VA_ARGS__
//      It enables the client to not only assert with a message but also print any variable value in the message (like printf)

#define CLEAN_ERRNO() (errno == 0 ? "None" : strerror(errno))
#define LOG_ERROR(message, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " message "\n", __FILE__, __LINE__, CLEAN_ERRNO(), ##__VA_ARGS__)
#define assertf(assertion, message, ...) if(!(assertion)) {LOG_ERROR(message, ##__VA_ARGS__); assert(assertion); }

#endif // ASSERTF_H
