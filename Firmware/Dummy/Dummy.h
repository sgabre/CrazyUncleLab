#ifndef _DUMMY_H_
#define _DUMMY_H_

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @file Dummy.h
 * @brief Public API for the Dummy module.
 *
 * The Dummy module provides a tiny example interface to set and report a
 * semantic version number and store a small reserved payload. This header
 * exposes initialization, a periodic handler and a single public function to
 * feed data into the module.
 *
 * @author sgabre
 * @date 2025-10-19
 */

/* includes ============================================================== */
#include <stddef.h>
#include <stdint.h>

/* macros ============================================================== */
/* constants =========================================================== */
/* types =============================================================== */
    
/* Forward declaration = opaque type / incomplete type */
/**
 * @brief Opaque public context type.
 *
 * The concrete layout of `Dummy_PublicContext_ts` is intentionally hidden in
 * the implementation (`Dummy.c`). Client code should treat this as an opaque
 * handle and must not access its members directly. Use accessor functions if
 * the module provides them.
 */
typedef struct Dummy_PublicContext_ts Dummy_PublicContext_ts;
    
/* structures ========================================================== */

/**
 * @enum DummyType_te
 * @brief Identifiers for the types of data accepted by @ref Dummy_PublicFuntion.
 *
 * The values map to the operations interpreted by the public function:
 * - DUMMY_VERSION_MAJOR: set major version
 * - DUMMY_VERSION_MINOR: set minor version
 * - DUMMY_VERSION_PATCH: set patch version
 * - DUMMY_RESERVED: write into the reserved buffer
 */
typedef enum
{
    DUMMY_VERSION_MAJOR,
    DUMMY_VERSION_MINOR,
    DUMMY_VERSION_PATCH,
    DUMMY_RESERVED
} DummyType_te;

/* functions =========================================================== */
/**
 * @brief Initialize the Dummy module.
 *
 * This function allocates and initializes the module private context. The
 * context is zeroed and the module state is set to @c DUMMY_IDLE_STATE.
 *
 * @note The function uses malloc to allocate the private context. No matching
 *       free is provided by this module; the application is responsible for
 *       freeing the context if required. If allocation fails the function
 *       prints an error message to stdout and returns without initializing
 *       the module.
 *
 * @pre None.
 * @post On success, @c mDummyCxt points to a valid context and the module is
 *       in the idle state. On allocation failure @c mDummyCxt remains NULL.
 */
void Dummy_Init(void);

/**
 * @brief Periodic handler for the Dummy module.
 *
 * Call periodically (for example from a main loop or scheduler) to perform
 * module actions such as printing the version when the module is active.
 */
void Dummy_Handler(void);

/**
 * @brief Public API to feed data into the Dummy module.
 *
 * The interpretation of the provided data depends on @p type. Use the values
 * defined in @ref DummyType_te:
 * - @c DUMMY_VERSION_MAJOR: set Major version from data[0]
 * - @c DUMMY_VERSION_MINOR: set Minor version from data[0]
 * - @c DUMMY_VERSION_PATCH: set Patch version from data[0] and transition to
 *   @c DUMMY_ACTIVE_STATE
 * - @c DUMMY_RESERVED: copy up to @c RESERVED_SIZE bytes from @p data into
 *   the reserved buffer in the private context
 *
 * @param[in] type   Selector for the operation (see description above).
 * @param[in] length Number of bytes available in @p data. When copying into
 *                   the reserved buffer the value is capped to
 *                   @c RESERVED_SIZE.
 * @param[in] data   Pointer to the input buffer. Must be non-NULL when
 *                   @p length > 0. The caller is responsible for ensuring
 *                   the buffer is large enough for the requested operation.
 *
 * @pre The module must be initialized with @ref Dummy_Init before calling.
 * @note No bounds-checking is performed for cases 0-2 beyond reading
 *       data[0]; passing an empty buffer may cause undefined behavior. The
 *       implementation also ignores requests when the module is not
 *       initialized (i.e. @c Dummy_Init has not been called) — the call will
 *       simply return.
 */
void Dummy_PublicFunction(uint8_t type, size_t length, uint8_t* data);

#ifdef __cplusplus
}
#endif

#endif /* _DUMMY_H_ */