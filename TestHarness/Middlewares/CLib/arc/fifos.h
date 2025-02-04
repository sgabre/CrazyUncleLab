#ifndef H_ED_FIFOS_20050120181603
#define H_ED_FIFOS_20050120181603

#ifdef __cplusplus
extern "C"
{
#endif

/* ---------------------------------------------------------------------
   (c) Emmanuel Delahaye 2005
   Project      : CLIB.ED
   Function     : Allocated strings FIFO
   Module       :
   File         : fifos.h
   Created      : 23-01-2005
   Modified     : 23-01-2005
   --------------------------------------------------------------------- */

/* ---------------------------------------------------------------------
   Log

   0.0 23-01-2005 Created
   1.0 23-01-2005 Initial version

   --------------------------------------------------------------------- */

#include <stddef.h>

/* macros ============================================================== */
/* constants =========================================================== */
/* types =============================================================== */

   typedef struct fifos fifos_s;

/* structures ========================================================== */
/* entry points ======================================================== */

   char const *fifos_sid (void);
   char const *fifos_sver (void);

   fifos_s *fifos_create (size_t size);
   void fifos_delete (fifos_s * self);

   int fifos_full (fifos_s * self);
   int fifos_put (fifos_s * self, char const *s);

   char const *fifos_get (fifos_s * self);
   int fifos_empty (fifos_s * self);

/* public variables ==================================================== */

#ifdef __cplusplus
}
#endif

#endif                          /* guard */

/* Guards added by GUARD (c) ED 2000-2005 Jan 17 2005 Ver. 1.7 */
