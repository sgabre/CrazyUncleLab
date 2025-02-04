#ifndef H_ED_FIFO_20031121182656
#define H_ED_FIFO_20031121182656

/* ---------------------------------------------------------------------
   (c) ED 2003-2006
   Project      : CLIB
   Function     : Generic FIFO based on OBJ
   Module       : GFIFO
   File         : GFIFO.H
   Created      : 21-11-2003
   Modified     : 15-05-2006
   --------------------------------------------------------------------- */

/* ---------------------------------------------------------------------
   Log

   1.1 15-05-2006 sysalloc.h added
   1.0 27-11-2003 Initial version
   0.0 21-11-2003 Creation

   --------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

/* gfifo.h */
#include "ed/inc/obj.h"
#include <stddef.h>

/* macros ============================================================== */
/* constants =========================================================== */
/* types =============================================================== */

   typedef struct gfifo gfifo_s;

/* structures ========================================================== */
/* internal public data ================================================ */
/* internal public functions =========================================== */
/* entry points ======================================================== */

   char const *gfifo_sver (void);
   char const *gfifo_sid (void);

   gfifo_s *gfifo_create (size_t size);
   void gfifo_delete (gfifo_s * this);

   int gfifo_init (gfifo_s * this, char const *sid);
   int gfifo_put (gfifo_s * this, OBJ_RO data);
   int gfifo_get (gfifo_s * this, OBJ_RO * p_data);
   int gfifo_empty (gfifo_s * this);

/* public data ========================================================= */

#ifdef __cplusplus
}
#endif

#endif                          /* H_ED_FIFO_20031121182656 */

/* Guards added by GUARD (c) ED 2000-2003 Feb 14 2003 Ver. 1.5 */
