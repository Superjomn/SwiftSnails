/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.in by autoheader.  */


#ifndef _CONFIG_H
#define _CONFIG_H


/* Command to list symbols from object files. */
#define DEFAULT_NM "/usr/bin/nm"

/* Define to 1 to disable clock synchronization */
#define DISABLE_CLOCK_SYNC 0

/* Define to 1 if you have the <asm/intrinsics.h> header file. */
/* #undef HAVE_ASM_INTRINSICS_H */

/* Define to 1 if you have the `asprintf' function. */
#define HAVE_ASPRINTF 1

/* Define to 1 if you have the <catamount/data.h> header file. */
/* #undef HAVE_CATAMOUNT_DATA_H */

/* Define to 1 if you have the <catamount/dclock.h> header file. */
/* #undef HAVE_CATAMOUNT_DCLOCK_H */

/* Define to 1 if you have the `creat64' function. */
/* #undef HAVE_CREAT64 */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <fnmatch.h> header file. */
#define HAVE_FNMATCH_H 1

/* Define to 1 if you have the `fopen64' function. */
/* #undef HAVE_FOPEN64 */

/* Define to 1 if you have the `fseeko' function. */
/* #undef HAVE_FSEEKO */

/* Define to 1 if you have the `fseeko64' function. */
/* #undef HAVE_FSEEKO64 */

/* Define to 1 if you have the `fsetpos64' function. */
/* #undef HAVE_FSETPOS64 */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <linux/mmtimer.h> header file. */
/* #undef HAVE_LINUX_MMTIMER_H */

/* Define to 1 if you have the `lseek64' function. */
/* #undef HAVE_LSEEK64 */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <mmtimer.h> header file. */
/* #undef HAVE_MMTIMER_H */

/* Define to 1 if MPI supports file access. */
#define HAVE_MPIO 1

/* Define to 1 if you have the `open64' function. */
/* #undef HAVE_OPEN64 */

/* Define if OpenMP is enabled */
/* #undef HAVE_OPENMP */

/* Define to 1 if you have the `pread64' function. */
/* #undef HAVE_PREAD64 */

/* Define to 1 if you have the `pwrite64' function. */
/* #undef HAVE_PWRITE64 */

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Define to 1 if you have the <sn/mmtimer.h> header file. */
/* #undef HAVE_SN_MMTIMER_H */

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/param.h> header file. */
#define HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the `__va_copy' function. */
#define HAVE_UNDERSCORE_VA_COPY 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the `vasprintf' function. */
#define HAVE_VASPRINTF 1

/* Define to 1 if you have the `va_copy' function. */
#define HAVE_VA_COPY 1

/* Define to 1 if you have the `vsnprintf' function. */
#define HAVE_VSNPRINTF 1

/* Define to 1 if you have the `__fprintf_chk' function. */
/* #undef HAVE___FPRINTF_CHK */

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
/* #undef NO_MINUS_C_MINUS_O */

/* Name of package */
#define PACKAGE "VampirTrace"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "vampirsupport@zih.tu-dresden.de"

/* Define to the full name of this package. */
#define PACKAGE_NAME "VampirTrace"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "VampirTrace 5.4.14"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "vampirtrace"

/* Define to the version of this package. */
#define PACKAGE_VERSION "5.4.14"

/* Path for node-local temporary directory */
/* #undef PFORM_LDIR */

/* The size of `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 8

/* The size of `mode_t', as computed by sizeof. */
#define SIZEOF_MODE_T 2

/* The size of `void*', as computed by sizeof. */
#define SIZEOF_VOIDP 8

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Use timer (see below) */
#define TIMER TIMER_CYCLE_COUNTER

/* Use `clock_gettime' function */
/* #undef TIMER_CLOCK_GETTIME */

/* CRAY Real-Time-Clock */
/* #undef TIMER_CRAY_RTCLOCK */

/* Cycle counter (e.g. TSC) */
#define TIMER_CYCLE_COUNTER 1

/* Use `dclock' function */
/* #undef TIMER_DCLOCK */

/* gethrtime */
/* #undef TIMER_GETHRTIME */

/* Use `gettimeofday' function */
#define TIMER_GETTIMEOFDAY 2

/* Intel Multimedia Timer */
/* #undef TIMER_MMTIMER */

/* PAPI_get_real_cyc */
/* #undef TIMER_PAPI_REAL_CYC */

/* PAPI_get_real_usec */
/* #undef TIMER_PAPI_REAL_USEC */

/* IBM Power family Real-Time-Clock */
/* #undef TIMER_POWER_REALTIME */

/* RTC (DOES NOT WORK YET WITH FORTRAN CODES) */
/* #undef TIMER_RTC */

/* Read PowerPC 440 time base registers */
/* #undef TIMER_RTS_GET_TIMEBASE */

/* Hardware Switch-Clock (it's necessary to link your application with
   '-lswclock') */
/* #undef TIMER_SWITCH_CLOCK */

/* NEC SX HGTIME */
/* #undef TIMER_SYSSX_HGTIME */

/* Version number of package */
#define VERSION "5.4.14"

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */


#include "config_bottom.h"
#endif /* _CONFIG_H */

