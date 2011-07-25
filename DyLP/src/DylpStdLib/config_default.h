
/* include the COIN-OR-wide system specific configure header */
#include "configall_system.h"

/* include the public project specific macros */
#include "config_dylp_default.h"

/* Include float.h for _finite, _isnan */
#include <float.h>

/***************************************************************************/
/*             HERE DEFINE THE PROJECT SPECIFIC MACROS                     */
/*    These are only in effect in a setting that doesn't use configure     */
/***************************************************************************/

/* Define to the debug sanity check level (0 is no test) */
#define COIN_DYLP_CHECKLEVEL 0

/* Define to the debug verbosity level (0 is no output) */
#define COIN_DYLP_VERBOSITY 0

/*
  But dylp was developed long before COIN came into being, so if you really
  want the paranoid checks, define DYLP_PARANOIA. The value isn't important.
*/
/* #define DYLP_PARANOIA 1 */

/*
  But dylp was developed long before COIN came into being, so if you
  want informational printing, DO NOT define DYLP_NDEBUG. The value isn't
  important.
*/
/* #undef DYLP_NDEBUG 1 */

/*
  Define this variable to enable dylp's statistics collection features.
*/
#define DYLP_STATISTICS 1

/*
  Set to the full path directory name for the location of the error text
   message file dy_errmsgs.txt. This file is distributed with dylp source and
   not normally installed elsewhere. An absolute path to DyLP/src/Dylp/ is
   appropriate. The string should end with a directory separator ("/" or "\",
   depending on your system). The surrounding quotes are part of the
   definition. There is no good default; the value given here will work from
   the test directory, on a windows system, which seems the most likely
   environment to be using this part of DylpConfig.h.
*/
#ifndef DYLP_ERRMSGDIR
#define DYLP_ERRMSGDIR "..\\src\\Dylp\\"
#endif

/*
  Define this symbol if your system is `big-endian', i.e., the most significant
  byte of a multibyte quantity is stored in the lowest byte address. Intel x86
  systems are little-endian. SPARC and Motorola are big-endian.
*/
/* #define WORDS_BIGENDIAN 1 */

/*
  Define this symbol if the quiet_nan function exists. This function should
  return the bit pattern for IEEE quiet NaN.
*/
/* #define DYLP_HAS_QUIET_NAN 1 */

/*
  Define to be the name of the C function used to check that an IEEE floating
  point value is finite. Common possibilities are finite, _finite, and
  isfinite. _finite is correct for MSVC, which is the most likely place for
  this to be used.
*/
#define DYLP_ISFINITE _finite

/*
  Define to be the name of the C function used to check that an IEEE floating
  point value is NaN. Common possibilities are isnan and _isnan. _isnan for
  MSVC, as per _finite.
*/
#define DYLP_ISNAN _isnan

/*
  Define to 1 if sunmath.h exists. As you might guess, define this only on a
  Sun/Solaris system. And really, if you're building on Sun, why are you
  using this part of the configuration file? Run configure!
*/
/* #define HAVE_SUNMATH_H 1 */
