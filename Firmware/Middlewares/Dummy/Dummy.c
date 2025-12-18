/**
 * @file Dummy.c
 * @brief Minimal example "Dummy" module implementation used for demos and tests.
 *
 * This file implements a tiny module that stores a small version triplet and a
 * reserved buffer. It exposes an initialization function, a periodic handler,
 * and a public API to feed data into the module. The implementation allocates
 * a module context on the heap and keeps it in a static pointer.
 *
 * @author sgabre
 * @date 2025-10-19
 *
 * @note This is example code; error handling is intentionally simple. In a
 *       production system prefer static allocation or a dedicated allocator
 *       and add proper synchronization if used from multiple threads/interrupts.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "Dummy.h"


int32_t Dummy_PublicFunction(int32_t a, int32_t b)
{
    return a + b;
}
