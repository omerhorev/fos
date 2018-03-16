#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef void (*fos_task_entry_t)(void* context);

/**
 * Represents a value of a register
 */
typedef uint32_t regval_t;

/**
 * Represents a systick in the system.
 */
typedef uint32_t tick_t;

#ifdef __cplusplus
}
#endif
