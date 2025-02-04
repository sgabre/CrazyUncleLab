#ifndef H_ED_FIFOX_20031003130059
#define H_ED_FIFOX_20031003130059

#ifdef __cplusplus
extern "C"
{
#endif

/* FIFOX.H
   ------
 */

#include "ed/inc/fifo.h"

/* macros ============================================================== */

#define FIFO_TRACE 0

/* constants =========================================================== */
/* types =============================================================== */
/* structures ========================================================== */
/* entry points ======================================================== */

   void FIFO_dump (sFIFO * f);  /* ecrire (en la lisant)
                                   le contenu de la FIFO vers STDOUT
                                 */


#if FIFO_TRACE
   void FIFO_dumpall (void);
#else
#define FIFO_dumpall()
#endif



#define fdump(f) FIFO_dump(f)
#define fdumpall() FIFO_dumpall()

/* public variables ==================================================== */

#ifdef __cplusplus
}
#endif

#endif                          /* H_ED_FIFOX_20031003130059 */

/* Guards added by GUARD (c) ED 2000-2003 Feb 14 2003 Ver. 1.5 */
