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

/* includes ============================================================== */
#include "Dummy.h"

/* macros ============================================================== */
/**
 * @def RESERVED_SIZE
 * @brief Size of the reserved payload area in the private context.
 */
#define RESERVED_SIZE 5

/* constants =========================================================== */
/**
 * @var Counter
 * @brief Simple example counter used by the Dummy module.
 *
 * This static counter is part of the example implementation and may be used
 * by internal helpers or tests. It is intentionally file-scoped (static)
 * so it is not accessible from outside this translation unit.
 *
 * @note The counter is not used by the public API in the current example,
 *       but it is documented here for completeness. Remove it if unused.
 */
static uint8_t Counter = 0;

/* types =============================================================== */

/**
 * @enum DummyError_te
 * @brief Error codes used internally by the Dummy module.
 *
 * The module currently exposes simple error codes for future extension. They
 * are not propagated by the public API in this example.
 */
typedef enum
{
    DUMMY_NO_ERROR,      /*!< No error occurred. */
    DUMMY_UNKNONW_ERROR  /*!< An unknown error occurred. */
} DummyError_te;

/**
 * @enum DummyState_te
 * @brief Internal state of the Dummy module.
 *
 * DUMMY_IDLE_STATE indicates the module is idle. DUMMY_ACTIVE_STATE indicates
 * the module has received version information and will report it in the
 * handler.
 */
typedef enum
{
    DUMMY_IDLE_STATE,           /*!< Module is idle and waiting for input */  
    DUMMY_ACTIVE_STATE          /*!< Module is active and will print version */
} DummyState_te;

/**
 * @struct DummyPrivateContext_ts
 * @brief Private runtime context for the Dummy module.
 *
 * This structure stores the module state, a 3-part semantic version (major,
 * minor, patch) and a small reserved buffer used to hold arbitrary data. The
 * context is allocated dynamically by @ref Dummy_Init and freed by the
 * application when appropriate (not implemented in this example).
 */
typedef struct
{
    uint8_t State;                      /*!< Current state (see DummyState_te) */
    uint8_t Major;                      /*!< Major version number */
    uint8_t Minor;                      /*!< Minor version number */
    uint8_t Patch;                      /*!< Patch version number */
    uint8_t Reserved[RESERVED_SIZE];    /*!< Reserved payload buffer */
} DummyPrivateContext_ts;

/* Full struct definition hidden from users */
/**
 * @struct Dummy_PublicContext_ts
 * @brief Concrete definition of the opaque public context type.
 *
 * The public header (`Dummy.h`) exposes this type as an opaque forward
 * declaration so that external code cannot depend on its layout. The
 * definition below is internal to the implementation and should not be
 * referenced by client code. It contains a single internal byte used for
 * example purposes.
 */
struct Dummy_PublicContext_ts
{
    uint8_t value;  /*!< Internal data used by the module */
};


/*  Private Variables ==================================================== */
/**
 * @var mDummyCxt
 * @brief Pointer to the module private context.
 *
 * This pointer is NULL until @ref Dummy_Init is called. The context is
 * allocated on the heap with malloc in this example.
 */
static DummyPrivateContext_ts* mDummyCxt;


/*  Private Function ==================================================== */
/**
 * @brief Private helper function for internal tasks.
 *
 * This function is intentionally empty in the example. It is declared static
 * to limit visibility to this translation unit.
 */
static void Dummy_PrivateFunction(void);

/* Public functions =========================================================== */

void Dummy_Init(void)
{
    mDummyCxt = (DummyPrivateContext_ts*)malloc(sizeof(DummyPrivateContext_ts));
    
    if (mDummyCxt == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    memset(mDummyCxt, 0, sizeof(DummyPrivateContext_ts));

    mDummyCxt->State = DUMMY_IDLE_STATE;
}

void Dummy_Handler(void)
{
    switch (mDummyCxt->State)
    {
    case DUMMY_IDLE_STATE:
        /* code */
        printf("IDLE\n");
        break;

    case DUMMY_ACTIVE_STATE:
        /* code */      
        printf("Active\n");
        printf("Version: %d.%d.%d\n", mDummyCxt->Major, mDummyCxt->Minor, mDummyCxt->Patch );
        break;
    
    default:
        break;
    }
}

void Dummy_PublicFunction(uint8_t type, size_t length, uint8_t* data)
{
    switch (type)
    {
        case DUMMY_VERSION_MAJOR:
            /* code */
            mDummyCxt->Major = data[0];
            break;

        case DUMMY_VERSION_MINOR:
            /* code */
            mDummyCxt->Minor = data[0];
            break;

        case DUMMY_VERSION_PATCH:
            /* code */
            mDummyCxt->Patch = data[0];
            mDummyCxt->State = DUMMY_ACTIVE_STATE;
            break;  
    
        case DUMMY_RESERVED:
            /* code */
            if (length > RESERVED_SIZE)
            {
                length = RESERVED_SIZE;
            }
            memcpy(mDummyCxt->Reserved, data, length);
            break;
    
    default:
        break;
    }
}

/* Private Functions =========================================================== */

/**
 * @brief Placeholder for internal processing.
 *
 * Currently the function is intentionally empty. Keep the definition to
 * demonstrate where internal helper logic would be implemented.
 */
static void Dummy_PrivateFunction(void)
{

}