/* opal/include/opal_config.h.  Generated from opal_config.h.in by configure.  */
/* opal/include/opal_config.h.in.  Generated from configure.ac by autoheader.  */

/* -*- c -*-
 *
 * Copyright (c) 2004-2005 The Trustees of Indiana University.
 *                         All rights reserved.
 * Copyright (c) 2004-2005 The Trustees of the University of Tennessee.
 *                         All rights reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 *
 * Function: - OS, CPU and compiler dependent configuration 
 */

#ifndef OPAL_CONFIG_H
#define OPAL_CONFIG_H


/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* BLCR cr_request_file check */
/* #undef CRS_BLCR_HAVE_CR_REQUEST */

/* BLCR cr_request_checkpoint check */
/* #undef CRS_BLCR_HAVE_CR_REQUEST_CHECKPOINT */

/* BLCRs cr_checkpoint_info.requester member availability */
/* #undef CRS_BLCR_HAVE_INFO_REQUESTER */

/* Define to 1 if you have the <aio.h> header file. */
#define HAVE_AIO_H 1

/* Define to 1 if you have the <alloca.h> header file. */
#define HAVE_ALLOCA_H 1

/* Define to 1 if you have the <arpa/inet.h> header file. */
#define HAVE_ARPA_INET_H 1

/* Define to 1 if you have the `asprintf' function. */
#define HAVE_ASPRINTF 1

/* Define to 1 if you have the `backtrace' function. */
#define HAVE_BACKTRACE 1

/* Define to 1 if you have the <catamount/dclock.h> header file. */
/* #undef HAVE_CATAMOUNT_DCLOCK_H */

/* Define to 1 if you have the `ceil' function. */
#define HAVE_CEIL 1

/* Define to 1 if you have the <crt_externs.h> header file. */
#define HAVE_CRT_EXTERNS_H 1

/* Define to 1 if you have the `cr_request_checkpoint' function. */
/* #undef HAVE_CR_REQUEST_CHECKPOINT */

/* Define to 1 if you have the `cr_request_file' function. */
/* #undef HAVE_CR_REQUEST_FILE */

/* uDAPL DAT_MEM_TYPE_SO_VIRTUAL check */
/* #undef HAVE_DAT_MEM_TYPE_SO_VIRTUAL */

/* Define to 1 if you have the declaration of `AF_INET6', and to 0 if you
   don't. */
#define HAVE_DECL_AF_INET6 1

/* Define to 1 if you have the declaration of `AF_UNSPEC', and to 0 if you
   don't. */
#define HAVE_DECL_AF_UNSPEC 1

/* Define to 1 if you have the declaration of `IBV_EVENT_CLIENT_REREGISTER',
   and to 0 if you don't. */
/* #undef HAVE_DECL_IBV_EVENT_CLIENT_REREGISTER */

/* Define to 1 if you have the declaration of `MPOL_MF_MOVE', and to 0 if you
   don't. */
/* #undef HAVE_DECL_MPOL_MF_MOVE */

/* Define to 1 if you have the declaration of `PF_INET6', and to 0 if you
   don't. */
#define HAVE_DECL_PF_INET6 1

/* Define to 1 if you have the declaration of `PF_UNSPEC', and to 0 if you
   don't. */
#define HAVE_DECL_PF_UNSPEC 1

/* Define to 1 if you have the declaration of `RLIMIT_MEMLOCK', and to 0 if
   you don't. */
#define HAVE_DECL_RLIMIT_MEMLOCK 1

/* Define to 1 if you have the declaration of `RLIMIT_NPROC', and to 0 if you
   don't. */
#define HAVE_DECL_RLIMIT_NPROC 1

/* Define to 1 if you have the declaration of `sbrk', and to 0 if you don't.
   */
/* #undef HAVE_DECL_SBRK */

/* Define to 1 if you have the declaration of `__func__', and to 0 if you
   don't. */
#define HAVE_DECL___FUNC__ 1

/* Define if /dev/poll is available */
/* #undef HAVE_DEVPOLL */

/* Define to 1 if you have the <dirent.h> header file. */
#define HAVE_DIRENT_H 1

/* Define to 1 if you have the `dirname' function. */
#define HAVE_DIRNAME 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the `dlsym' function. */
/* #undef HAVE_DLSYM */

/* Define if your system supports the epoll interface */
/* #undef HAVE_EPOLL */

/* Define to 1 if you have the `epoll_ctl' function. */
/* #undef HAVE_EPOLL_CTL */

/* Define to 1 if you have the <err.h> header file. */
#define HAVE_ERR_H 1

/* Define if your system supports event ports */
/* #undef HAVE_EVENT_PORTS */

/* Define to 1 if you have the <execinfo.h> header file. */
#define HAVE_EXECINFO_H 1

/* Define to 1 if you have the `execve' function. */
#define HAVE_EXECVE 1

/* Define to 1 if you have the `fcntl' function. */
#define HAVE_FCNTL 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the `fork' function. */
#define HAVE_FORK 1

/* Define to 1 if you have the `getpwuid' function. */
#define HAVE_GETPWUID 1

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the <grp.h> header file. */
#define HAVE_GRP_H 1

/* Define to 1 if you have the <hostLib.h> header file. */
/* #undef HAVE_HOSTLIB_H */

/* Define to 1 if you have the `ibv_create_xrc_rcv_qp' function. */
/* #undef HAVE_IBV_CREATE_XRC_RCV_QP */

/* Define to 1 if you have the `ibv_fork_init' function. */
/* #undef HAVE_IBV_FORK_INIT */

/* Define to 1 if you have the `ibv_get_device_list' function. */
/* #undef HAVE_IBV_GET_DEVICE_LIST */

/* Define to 1 if you have the `ibv_resize_cq' function. */
/* #undef HAVE_IBV_RESIZE_CQ */

/* Define to 1 if you have the <ifaddrs.h> header file. */
#define HAVE_IFADDRS_H 1

/* Define to 1 if you have the <infiniband/cm.h> header file. */
/* #undef HAVE_INFINIBAND_CM_H */

/* Define to 1 if you have the <infiniband/driver.h> header file. */
/* #undef HAVE_INFINIBAND_DRIVER_H */

/* Define to 1 if the system has the type `int16_t'. */
#define HAVE_INT16_T 1

/* Define to 1 if the system has the type `int32_t'. */
#define HAVE_INT32_T 1

/* Define to 1 if the system has the type `int64_t'. */
#define HAVE_INT64_T 1

/* Define to 1 if the system has the type `int8_t'. */
#define HAVE_INT8_T 1

/* Define to 1 if the system has the type `intptr_t'. */
#define HAVE_INTPTR_T 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <ioLib.h> header file. */
/* #undef HAVE_IOLIB_H */

/* Define to 1 if you have the `isatty' function. */
#define HAVE_ISATTY 1

/* Define to 1 if you have the `kqueue' function. */
/* #undef HAVE_KQUEUE */

/* Define to 1 if you have the <libgen.h> header file. */
#define HAVE_LIBGEN_H 1

/* Define to 1 if you have the `nsl' library (-lnsl). */
/* #undef HAVE_LIBNSL */

/* Define to 1 if you have the `socket' library (-lsocket). */
/* #undef HAVE_LIBSOCKET */

/* Define to 1 if you have the `util' library (-lutil). */
#define HAVE_LIBUTIL 1

/* Define to 1 if you have the <libutil.h> header file. */
/* #undef HAVE_LIBUTIL_H */

/* Define to 1 if the system has the type `long double'. */
#define HAVE_LONG_DOUBLE 1

/* Define to 1 if the system has the type `long long'. */
#define HAVE_LONG_LONG 1

/* Define to 1 if you have the <mach/mach_time.h> header file. */
#define HAVE_MACH_MACH_TIME_H 1

/* Define to 1 if you have the <mach/mach_vm.h> header file. */
/* #undef HAVE_MACH_MACH_VM_H */

/* Define to 1 if you have the `mach_vm_read' function. */
/* #undef HAVE_MACH_VM_READ */

/* Define to 1 if you have the `mach_vm_region' function. */
/* #undef HAVE_MACH_VM_REGION */

/* Define to 1 if you have the <malloc.h> header file. */
/* #undef HAVE_MALLOC_H */

/* Define to 1 if you have the `mallopt' function. */
/* #undef HAVE_MALLOPT */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `mkfifo' function. */
#define HAVE_MKFIFO 1

/* Define to 1 if you have the `mmap' function. */
#define HAVE_MMAP 1

/* Define to 1 if the system has the type `mode_t'. */
#define HAVE_MODE_T 1

/* Define to 1 if you have the <mx_extensions.h> header file. */
/* #undef HAVE_MX_EXTENSIONS_H */

/* Define to 1 if you have the `mx_forget' function. */
/* #undef HAVE_MX_FORGET */

/* Define to 1 if you have the `mx_register_unexp_handler' function. */
/* #undef HAVE_MX_REGISTER_UNEXP_HANDLER */

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* Define to 1 if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H 1

/* Define to 1 if you have the <netinet/tcp.h> header file. */
#define HAVE_NETINET_TCP_H 1

/* Define to 1 if you have the <net/if.h> header file. */
#define HAVE_NET_IF_H 1

/* Define to 1 if you have the <net/uio.h> header file. */
/* #undef HAVE_NET_UIO_H */

/* Define to 1 if you have the `openpty' function. */
#define HAVE_OPENPTY 1

/* Define to 1 if you have the `pipe' function. */
#define HAVE_PIPE 1

/* Define to 1 if you have the <pmapi.h> header file. */
/* #undef HAVE_PMAPI_H */

/* Define to 1 if you have the `pm_cycles' function. */
/* #undef HAVE_PM_CYCLES */

/* Define to 1 if you have the `poll' function. */
#define HAVE_POLL 1

/* Define to 1 if you have the <poll.h> header file. */
#define HAVE_POLL_H 1

/* Define to 1 if you have the `port_create' function. */
/* #undef HAVE_PORT_CREATE */

/* Define to 1 if you have the `posix_memalign' function. */
#define HAVE_POSIX_MEMALIGN 1

/* Define to 1 if `srr0' is member of `ppc_thread_state_t'. */
/* #undef HAVE_PPC_THREAD_STATE_T_SRR0 */

/* Define to 1 if you have the `printstack' function. */
/* #undef HAVE_PRINTSTACK */

/* Define to 1 if you have the <pthread.h> header file. */
#define HAVE_PTHREAD_H 1

/* Define to 1 if the system has the type `ptrdiff_t'. */
#define HAVE_PTRDIFF_T 1

/* Define to 1 if you have the `ptsname' function. */
#define HAVE_PTSNAME 1

/* Define to 1 if you have the <pty.h> header file. */
/* #undef HAVE_PTY_H */

/* Define to 1 if you have the <pwd.h> header file. */
#define HAVE_PWD_H 1

/* Define to 1 if you have the <rdma/rdma_cma.h> header file. */
/* #undef HAVE_RDMA_RDMA_CMA_H */

/* Define to 1 if you have the `regcmp' function. */
/* #undef HAVE_REGCMP */

/* Define to 1 if you have the `regexec' function. */
#define HAVE_REGEXEC 1

/* Define to 1 if you have the <regex.h> header file. */
#define HAVE_REGEX_H 1

/* Define to 1 if you have the `regfree' function. */
#define HAVE_REGFREE 1

/* Define if your system supports POSIX realtime signals */
/* #undef HAVE_RTSIG */

/* Define to 1 if you have the <sched.h> header file. */
#define HAVE_SCHED_H 1

/* Define to 1 if you have the `sched_yield' function. */
#define HAVE_SCHED_YIELD 1

/* Define to 1 if you have the `select' function. */
#define HAVE_SELECT 1

/* Define if F_SETFD is defined in <fcntl.h> */
#define HAVE_SETFD 1

/* Define to 1 if you have the `setsid' function. */
#define HAVE_SETSID 1

/* Define to 1 if you have the <shlwapi.h> header file. */
/* #undef HAVE_SHLWAPI_H */

/* Define to 1 if `si_band' is member of `siginfo_t'. */
#define HAVE_SIGINFO_T_SI_BAND 1

/* Define to 1 if `si_fd' is member of `siginfo_t'. */
/* #undef HAVE_SIGINFO_T_SI_FD */

/* Define to 1 if you have the <signal.h> header file. */
#define HAVE_SIGNAL_H 1

/* Define to 1 if you have the `sigtimedwait' function. */
/* #undef HAVE_SIGTIMEDWAIT */

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Define to 1 if you have the `socketpair' function. */
#define HAVE_SOCKETPAIR 1

/* Define to 1 if the system has the type `socklen_t'. */
#define HAVE_SOCKLEN_T 1

/* Define to 1 if you have the <sockLib.h> header file. */
/* #undef HAVE_SOCKLIB_H */

/* Define to 1 if the system has the type `ssize_t'. */
#define HAVE_SSIZE_T 1

/* Define to 1 if you have the <stdarg.h> header file. */
#define HAVE_STDARG_H 1

/* Define to 1 if you have the <stdbool.h> header file. */
#define HAVE_STDBOOL_H 1

/* Define to 1 if you have the <stddef.h> header file. */
#define HAVE_STDDEF_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strncpy_s' function. */
/* #undef HAVE_STRNCPY_S */

/* Define to 1 if you have the <stropts.h> header file. */
/* #undef HAVE_STROPTS_H */

/* Define to 1 if you have the `strsignal' function. */
#define HAVE_STRSIGNAL 1

/* Define to 1 if `d_type' is member of `struct dirent'. */
#define HAVE_STRUCT_DIRENT_D_TYPE 1

/* Define to 1 if `transport_type' is member of `struct ibv_device'. */
/* #undef HAVE_STRUCT_IBV_DEVICE_TRANSPORT_TYPE */

/* Define to 1 if the system has the type `struct sockaddr_in'. */
#define HAVE_STRUCT_SOCKADDR_IN 1

/* Define to 1 if the system has the type `struct sockaddr_in6'. */
#define HAVE_STRUCT_SOCKADDR_IN6 1

/* Define to 1 if `sa_len' is member of `struct sockaddr'. */
#define HAVE_STRUCT_SOCKADDR_SA_LEN 1

/* Define to 1 if the system has the type `struct sockaddr_storage'. */
#define HAVE_STRUCT_SOCKADDR_STORAGE 1

/* Define to 1 if you have the `syscall' function. */
/* #undef HAVE_SYSCALL */

/* Define to 1 if you have the `sysconf' function. */
#define HAVE_SYSCONF 1

/* Define to 1 if you have the `syslog' function. */
#define HAVE_SYSLOG 1

/* Define to 1 if you have the <syslog.h> header file. */
#define HAVE_SYSLOG_H 1

/* Define to 1 if you have the <sys/devpoll.h> header file. */
/* #undef HAVE_SYS_DEVPOLL_H */

/* Define to 1 if you have the <sys/epoll.h> header file. */
/* #undef HAVE_SYS_EPOLL_H */

/* Define to 1 if you have the <sys/event.h> header file. */
#define HAVE_SYS_EVENT_H 1

/* Define to 1 if you have the <sys/fcntl.h> header file. */
#define HAVE_SYS_FCNTL_H 1

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#define HAVE_SYS_IOCTL_H 1

/* Define to 1 if you have the <sys/ipc.h> header file. */
#define HAVE_SYS_IPC_H 1

/* Define to 1 if you have the <sys/mman.h> header file. */
#define HAVE_SYS_MMAN_H 1

/* Define to 1 if you have the <sys/mount.h> header file. */
#define HAVE_SYS_MOUNT_H 1

/* Define to 1 if you have the <sys/param.h> header file. */
#define HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/poll.h> header file. */
#define HAVE_SYS_POLL_H 1

/* Define to 1 if you have the <sys/queue.h> header file. */
#define HAVE_SYS_QUEUE_H 1

/* Define to 1 if you have the <sys/resource.h> header file. */
#define HAVE_SYS_RESOURCE_H 1

/* Define to 1 if you have the <sys/select.h> header file. */
#define HAVE_SYS_SELECT_H 1

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 1

/* Define to 1 if you have the <sys/sockio.h> header file. */
#define HAVE_SYS_SOCKIO_H 1

/* Define to 1 if you have the <sys/statfs.h> header file. */
/* #undef HAVE_SYS_STATFS_H */

/* Define to 1 if you have the <sys/statvfs.h> header file. */
#define HAVE_SYS_STATVFS_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/synch.h> header file. */
/* #undef HAVE_SYS_SYNCH_H */

/* Define to 1 if you have the <sys/sysctl.h> header file. */
#define HAVE_SYS_SYSCTL_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/tree.h> header file. */
/* #undef HAVE_SYS_TREE_H */

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/uio.h> header file. */
#define HAVE_SYS_UIO_H 1

/* Define to 1 if you have the <sys/utsname.h> header file. */
#define HAVE_SYS_UTSNAME_H 1

/* Define to 1 if you have the <sys/vfs.h> header file. */
/* #undef HAVE_SYS_VFS_H */

/* Define to 1 if you have the <sys/wait.h> header file. */
#define HAVE_SYS_WAIT_H 1

/* Define if TAILQ_FOREACH is defined in <sys/queue.h> */
#define HAVE_TAILQFOREACH 1

/* Define to 1 if you have the `tcgetpgrp' function. */
#define HAVE_TCGETPGRP 1

/* Define to 1 if you have the <termios.h> header file. */
#define HAVE_TERMIOS_H 1

/* Define if timeradd is defined in <sys/time.h> */
#define HAVE_TIMERADD 1

/* Define to 1 if you have the <time.h> header file. */
#define HAVE_TIME_H 1

/* Define to 1 if you have the <ucontext.h> header file. */
/* #undef HAVE_UCONTEXT_H */

/* Define to 1 if the system has the type `uint16_t'. */
#define HAVE_UINT16_T 1

/* Define to 1 if the system has the type `uint32_t'. */
#define HAVE_UINT32_T 1

/* Define to 1 if the system has the type `uint64_t'. */
#define HAVE_UINT64_T 1

/* Define to 1 if the system has the type `uint8_t'. */
#define HAVE_UINT8_T 1

/* Define to 1 if the system has the type `uintptr_t'. */
#define HAVE_UINTPTR_T 1

/* Define to 1 if you have the <ulimit.h> header file. */
#define HAVE_ULIMIT_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* whether unix byteswap routines -- htonl, htons, nothl, ntohs -- are
   available */
#define HAVE_UNIX_BYTESWAP 1

/* Define to 1 if you have the `usleep' function. */
#define HAVE_USLEEP 1

/* Define to 1 if you have the <util.h> header file. */
#define HAVE_UTIL_H 1

/* Define to 1 if you have the <utmp.h> header file. */
#define HAVE_UTMP_H 1

/* Define to 1 if you have the <valgrind/valgrind.h> header file. */
/* #undef HAVE_VALGRIND_VALGRIND_H */

/* Define to 1 if you have the `vasprintf' function. */
#define HAVE_VASPRINTF 1

/* Define to 1 if you have the `vm_read_overwrite' function. */
/* #undef HAVE_VM_READ_OVERWRITE */

/* Define to 1 if you have the `vsnprintf' function. */
#define HAVE_VSNPRINTF 1

/* Define to 1 if you have the `waitpid' function. */
#define HAVE_WAITPID 1

/* Define if kqueue works correctly with pipes */
/* #undef HAVE_WORKING_KQUEUE */

/* Whether poll works for file descriptors and devices */
#define HAVE_WORKING_POLL 0

/* Define if realtime signals work on pipes */
/* #undef HAVE_WORKING_RTSIG */

/* Define to 1 if you have the `_NSGetEnviron' function. */
#define HAVE__NSGETENVIRON 1

/* Define to 1 if you have the `_strdup' function. */
/* #undef HAVE__STRDUP */

/* Define to 1 if you have the `__mmap' function. */
/* #undef HAVE___MMAP */

/* Define to 1 if you have the `__munmap' function. */
/* #undef HAVE___MUNMAP */

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Complete set of command line arguments given to ROMIOs configure script */
#define MCA_io_romio_COMPLETE_CONFIGURE_FLAGS " CFLAGS='-O3 -DNDEBUG -finline-functions -fno-strict-aliasing ' CPPFLAGS=' -D_REENTRANT' FFLAGS='' LDFLAGS=' ' --enable-shared --disable-static  --prefix=/Users/apple/project/SwiftSnails/third/openmpi-1.4.5/../local/openmpi --with-mpi=open_mpi --disable-aio"

/* Set of user-defined configure flags given to ROMIOs configure script via
   --with-io-romio-flags */
#define MCA_io_romio_USER_CONFIGURE_FLAGS ""

/* Header to include for memcpy implementation */
#define MCA_memcpy_IMPLEMENTATION_HEADER "opal/mca/memcpy/base/memcpy_base_default.h"

/* Defined to 1 if mtl should use direct calls instead of components */
#define MCA_mtl_DIRECT_CALL 0

/* name of component to use for direct calls, if MCA_mtl_DIRECT_CALL is 1 */
#define MCA_mtl_DIRECT_CALL_COMPONENT 

/* Header mtl includes to be direct called */
#define MCA_mtl_DIRECT_CALL_HEADER ""

/* Defined to 1 if pml should use direct calls instead of components */
#define MCA_pml_DIRECT_CALL 0

/* name of component to use for direct calls, if MCA_pml_DIRECT_CALL is 1 */
#define MCA_pml_DIRECT_CALL_COMPONENT 

/* Header pml includes to be direct called */
#define MCA_pml_DIRECT_CALL_HEADER ""

/* Header to include for timer implementation */
#define MCA_timer_IMPLEMENTATION_HEADER "opal/mca/timer/darwin/timer_darwin.h"

/* Whether we want to check MPI parameters always, never, or decide at
   run-time */
#define MPI_PARAM_CHECK ompi_mpi_param_check

/* MX installation provide access to the mx_open_board and
   mx__get_mapper_state functions */
/* #undef MX_HAVE_MAPPER_STATE */

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
/* #undef NO_MINUS_C_MINUS_O */

/* Alignment of type char */
#define OMPI_ALIGNMENT_CHAR 1

/* Alignment of type bool */
#define OMPI_ALIGNMENT_CXX_BOOL 1

/* Alignment of type double */
#define OMPI_ALIGNMENT_DOUBLE 8

/* Alignment of type float */
#define OMPI_ALIGNMENT_FLOAT 4

/* Alignment of Fortran 77 COMPLEX */
#define OMPI_ALIGNMENT_FORTRAN_COMPLEX 4

/* Alignment of Fortran 77 COMPLEX*16 */
#define OMPI_ALIGNMENT_FORTRAN_COMPLEX16 4

/* Alignment of Fortran 77 COMPLEX*32 */
#define OMPI_ALIGNMENT_FORTRAN_COMPLEX32 4

/* Alignment of Fortran 77 COMPLEX*8 */
#define OMPI_ALIGNMENT_FORTRAN_COMPLEX8 4

/* Alignment of Fortran 77 DOUBLE PRECISION */
#define OMPI_ALIGNMENT_FORTRAN_DOUBLE_PRECISION 4

/* Alignment of Fortran 77 INTEGER */
#define OMPI_ALIGNMENT_FORTRAN_INTEGER 4

/* Alignment of Fortran 77 INTEGER*1 */
#define OMPI_ALIGNMENT_FORTRAN_INTEGER1 4

/* Alignment of Fortran 77 INTEGER*16 */
#define OMPI_ALIGNMENT_FORTRAN_INTEGER16 4

/* Alignment of Fortran 77 INTEGER*2 */
#define OMPI_ALIGNMENT_FORTRAN_INTEGER2 4

/* Alignment of Fortran 77 INTEGER*4 */
#define OMPI_ALIGNMENT_FORTRAN_INTEGER4 4

/* Alignment of Fortran 77 INTEGER*8 */
#define OMPI_ALIGNMENT_FORTRAN_INTEGER8 4

/* Alignment of Fortran 77 LOGICAL */
#define OMPI_ALIGNMENT_FORTRAN_LOGICAL 4

/* Alignment of Fortran 77 LOGICAL*1 */
#define OMPI_ALIGNMENT_FORTRAN_LOGICAL1 4

/* Alignment of Fortran 77 LOGICAL*2 */
#define OMPI_ALIGNMENT_FORTRAN_LOGICAL2 4

/* Alignment of Fortran 77 LOGICAL*4 */
#define OMPI_ALIGNMENT_FORTRAN_LOGICAL4 4

/* Alignment of Fortran 77 LOGICAL*8 */
#define OMPI_ALIGNMENT_FORTRAN_LOGICAL8 4

/* Alignment of Fortran 77 REAL */
#define OMPI_ALIGNMENT_FORTRAN_REAL 4

/* Alignment of Fortran 77 REAL*16 */
#define OMPI_ALIGNMENT_FORTRAN_REAL16 4

/* Alignment of Fortran 77 REAL*2 */
#define OMPI_ALIGNMENT_FORTRAN_REAL2 4

/* Alignment of Fortran 77 REAL*4 */
#define OMPI_ALIGNMENT_FORTRAN_REAL4 4

/* Alignment of Fortran 77 REAL*8 */
#define OMPI_ALIGNMENT_FORTRAN_REAL8 4

/* Alignment of type int */
#define OMPI_ALIGNMENT_INT 4

/* Alignment of type long */
#define OMPI_ALIGNMENT_LONG 8

/* Alignment of type long double */
#define OMPI_ALIGNMENT_LONG_DOUBLE 16

/* Alignment of type long long */
#define OMPI_ALIGNMENT_LONG_LONG 8

/* Alignment of type short */
#define OMPI_ALIGNMENT_SHORT 2

/* Alignment of type void * */
#define OMPI_ALIGNMENT_VOID_P 8

/* Alignment of type wchar_t */
#define OMPI_ALIGNMENT_WCHAR 4

/* set to 1 if word-size integers must be aligned to word-size padding to
   prevent bus errors */
#define OMPI_ALIGN_WORD_SIZE_INTEGERS 0

/* OMPI architecture string */
#define OMPI_ARCH "x86_64-apple-darwin14.0.0"

/* Assembly align directive expects logarithmic value */
#define OMPI_ASM_ALIGN_LOG 

/* Assembly directive for exporting symbols */
#define OMPI_ASM_GLOBAL ".globl"

/* Assembly prefix for gsym labels */
#define OMPI_ASM_GSYM "_"

/* Assembly suffix for labels */
#define OMPI_ASM_LABEL_SUFFIX ":"

/* Assembly prefix for lsym labels */
#define OMPI_ASM_LSYM "L"

/* Do we need to give a .size directive */
#define OMPI_ASM_SIZE "0"

/* Whether we can do 64bit assembly operations or not. Should not be used
   outside of the assembly header files */
#define OMPI_ASM_SUPPORT_64BIT 1

/* Assembly directive for setting text section */
#define OMPI_ASM_TEXT ".text"

/* How to set function type in .type directive */
#define OMPI_ASM_TYPE ""

/* Architecture type of assembly to use for atomic operations */
#define OMPI_ASSEMBLY_ARCH OMPI_AMD64

/* Format of assembly file */
#define OMPI_ASSEMBLY_FORMAT "default-.text-.globl-:-_-L--0-1-1-1-0"

/* OMPI underlying C compiler */
#define OMPI_CC "gcc"

/* Use static const char[] strings for C files */
#define OMPI_CC_USE_CONST_CHAR_IDENT 1

/* Use #ident strings for C files */
#define OMPI_CC_USE_IDENT 0

/* Use #pragma comment for C files */
#define OMPI_CC_USE_PRAGMA_COMMENT 

/* Use #pragma ident strings for C files */
#define OMPI_CC_USE_PRAGMA_IDENT 0

/* OMPI underlying C++ compiler */
#define OMPI_CXX "g++"

/* Whether C++ compiler supports DEC style inline assembly */
#define OMPI_CXX_DEC_INLINE_ASSEMBLY 0

/* Whether C++ compiler supports GCC style inline assembly */
#define OMPI_CXX_GCC_INLINE_ASSEMBLY 1

/* Whether C++ compiler supports __builtin_expect */
#define OMPI_CXX_HAVE_BUILTIN_EXPECT 1

/* Whether C++ compiler supports __builtin_prefetch */
#define OMPI_CXX_HAVE_BUILTIN_PREFETCH 1

/* Whether a const_cast on a 2-d array will work with the C++ compiler */
#define OMPI_CXX_SUPPORTS_2D_CONST_CAST 1

/* Use static const char[] strings for C++ files */
#define OMPI_CXX_USE_CONST_CHAR_IDENT 1

/* Use #ident strings for C++ files */
#define OMPI_CXX_USE_IDENT 0

/* Use #pragma comment for C++ files */
#define OMPI_CXX_USE_PRAGMA_COMMENT 

/* Use #pragma ident strings for C++ files */
#define OMPI_CXX_USE_PRAGMA_IDENT 0

/* Whether C++ compiler supports XLC style inline assembly */
#define OMPI_CXX_XLC_INLINE_ASSEMBLY 0

/* Whether C compiler supports DEC style inline assembly */
#define OMPI_C_DEC_INLINE_ASSEMBLY 0

/* Whether C compiler supports GCC style inline assembly */
#define OMPI_C_GCC_INLINE_ASSEMBLY 1

/* Whether C compiler supports __builtin_expect */
#define OMPI_C_HAVE_BUILTIN_EXPECT 1

/* Whether C compiler supports __builtin_prefetch */
#define OMPI_C_HAVE_BUILTIN_PREFETCH 1

/* Whether C compiler supports -fvisibility */
#define OMPI_C_HAVE_VISIBILITY 1

/* Whether C compiler supports XLC style inline assembly */
#define OMPI_C_XLC_INLINE_ASSEMBLY 0

/* Enable contributed software package vt */
#define OMPI_ENABLE_CONTRIB_vt 1

/* Whether we want developer-level debugging code or not */
#define OMPI_ENABLE_DEBUG 0

/* Enable features required for heterogeneous support */
#define OMPI_ENABLE_HETEROGENEOUS_SUPPORT 0

/* Whether we want the memory profiling or not */
#define OMPI_ENABLE_MEM_DEBUG 0

/* Whether we want the memory profiling or not */
#define OMPI_ENABLE_MEM_PROFILE 0

/* Whether we want MPI profiling or not */
#define OMPI_ENABLE_MPI_PROFILING 1

/* Whether we should enable support for multiple user threads */
#define OMPI_ENABLE_MPI_THREADS 0

/* Hardcode the OPAL progress thread to be off */
#define OMPI_ENABLE_PROGRESS_THREADS 0

/* Whether user wants PTY support or not */
#define OMPI_ENABLE_PTY_SUPPORT 1

/* OMPI underlying F77 compiler */
#define OMPI_F77 "none"

/* Whether fortran symbols are all caps or not */
#define OMPI_F77_CAPS 0

/* Whether fortran symbols have a trailing double underscore or not */
#define OMPI_F77_DOUBLE_UNDERSCORE 0

/* Whether fortran symbols have no trailing underscore or not */
#define OMPI_F77_PLAIN 0

/* Whether fortran symbols have a trailing underscore or not */
#define OMPI_F77_SINGLE_UNDERSCORE 0

/* OMPI underlying F90 compiler */
#define OMPI_F90 "none"

/* Max handle value for fortran MPI handles, effectively min(INT_MAX, max
   fortran INTEGER value) */
#define OMPI_FORTRAN_HANDLE_MAX 2147483647

/* Fortran value for LOGICAL .TRUE. value */
#define OMPI_FORTRAN_VALUE_TRUE 0

/* Greek - alpha, beta, etc - release number of Open MPI */
#define OMPI_GREEK_VERSION ""

/* Wether we want sparse process groups */
#define OMPI_GROUP_SPARSE 0

/* whether to use cnos_barrier or not */
#define OMPI_GRPCOMM_CNOS_HAVE_BARRIER 0

/* Whether there is an atomic assembly file available */
#define OMPI_HAVE_ASM_FILE 1

/* Whether your compiler has __attribute__ or not */
#define OMPI_HAVE_ATTRIBUTE 1

/* Whether your compiler has __attribute__ aligned or not */
#define OMPI_HAVE_ATTRIBUTE_ALIGNED 1

/* Whether your compiler has __attribute__ always_inline or not */
#define OMPI_HAVE_ATTRIBUTE_ALWAYS_INLINE 1

/* Whether your compiler has __attribute__ cold or not */
#define OMPI_HAVE_ATTRIBUTE_COLD 1

/* Whether your compiler has __attribute__ const or not */
#define OMPI_HAVE_ATTRIBUTE_CONST 1

/* Whether your compiler has __attribute__ deprecated or not */
#define OMPI_HAVE_ATTRIBUTE_DEPRECATED 1

/* Whether your compiler has __attribute__ format or not */
#define OMPI_HAVE_ATTRIBUTE_FORMAT 1

/* Whether your compiler has __attribute__ hot or not */
#define OMPI_HAVE_ATTRIBUTE_HOT 1

/* Whether your compiler has __attribute__ malloc or not */
#define OMPI_HAVE_ATTRIBUTE_MALLOC 1

/* Whether your compiler has __attribute__ may_alias or not */
#define OMPI_HAVE_ATTRIBUTE_MAY_ALIAS 1

/* Whether your compiler has __attribute__ nonnull or not */
#define OMPI_HAVE_ATTRIBUTE_NONNULL 1

/* Whether your compiler has __attribute__ noreturn or not */
#define OMPI_HAVE_ATTRIBUTE_NORETURN 1

/* Whether your compiler has __attribute__ no_instrument_function or not */
#define OMPI_HAVE_ATTRIBUTE_NO_INSTRUMENT_FUNCTION 1

/* Whether your compiler has __attribute__ packed or not */
#define OMPI_HAVE_ATTRIBUTE_PACKED 1

/* Whether your compiler has __attribute__ pure or not */
#define OMPI_HAVE_ATTRIBUTE_PURE 1

/* Whether your compiler has __attribute__ sentinel or not */
#define OMPI_HAVE_ATTRIBUTE_SENTINEL 1

/* Whether your compiler has __attribute__ unused or not */
#define OMPI_HAVE_ATTRIBUTE_UNUSED 1

/* Whether your compiler has __attribute__ visibility or not */
#define OMPI_HAVE_ATTRIBUTE_VISIBILITY 1

/* Whether your compiler has __attribute__ warn unused result or not */
#define OMPI_HAVE_ATTRIBUTE_WARN_UNUSED_RESULT 1

/* Whether your compiler has __attribute__ weak alias or not */
#define OMPI_HAVE_ATTRIBUTE_WEAK_ALIAS 0

/* whether qsort is broken or not */
#define OMPI_HAVE_BROKEN_QSORT 0

/* Enable features required for ConnectX XRC support */
#define OMPI_HAVE_CONNECTX_XRC 0

/* Whether or not we have compiled with C++ exceptions support */
#define OMPI_HAVE_CXX_EXCEPTION_SUPPORT 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_COMPLEX 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_COMPLEX16 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_COMPLEX32 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_COMPLEX8 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_DOUBLE_COMPLEX 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_DOUBLE_PRECISION 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_INTEGER 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_INTEGER1 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_INTEGER16 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_INTEGER2 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_INTEGER4 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_INTEGER8 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_LOGICAL 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_LOGICAL1 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_LOGICAL2 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_LOGICAL4 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_LOGICAL8 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_REAL 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_REAL16 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_REAL2 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_REAL4 0

/* Whether we have Fortran 90 $ofc_fortran_type or not */
#define OMPI_HAVE_F90_REAL8 0

/* Whether we have Fortran 77 COMPLEX or not */
#define OMPI_HAVE_FORTRAN_COMPLEX 0

/* Whether we have Fortran 77 COMPLEX*16 or not */
#define OMPI_HAVE_FORTRAN_COMPLEX16 0

/* Whether we have Fortran 77 COMPLEX*32 or not */
#define OMPI_HAVE_FORTRAN_COMPLEX32 0

/* Whether we have Fortran 77 COMPLEX*8 or not */
#define OMPI_HAVE_FORTRAN_COMPLEX8 0

/* Whether we have Fortran 77 DOUBLE PRECISION or not */
#define OMPI_HAVE_FORTRAN_DOUBLE_PRECISION 0

/* Whether we have Fortran 77 INTEGER or not */
#define OMPI_HAVE_FORTRAN_INTEGER 0

/* Whether we have Fortran 77 INTEGER*1 or not */
#define OMPI_HAVE_FORTRAN_INTEGER1 0

/* Whether we have Fortran 77 INTEGER*16 or not */
#define OMPI_HAVE_FORTRAN_INTEGER16 0

/* Whether we have Fortran 77 INTEGER*2 or not */
#define OMPI_HAVE_FORTRAN_INTEGER2 0

/* Whether we have Fortran 77 INTEGER*4 or not */
#define OMPI_HAVE_FORTRAN_INTEGER4 0

/* Whether we have Fortran 77 INTEGER*8 or not */
#define OMPI_HAVE_FORTRAN_INTEGER8 0

/* Whether we have Fortran 77 LOGICAL or not */
#define OMPI_HAVE_FORTRAN_LOGICAL 0

/* Whether we have Fortran 77 LOGICAL*1 or not */
#define OMPI_HAVE_FORTRAN_LOGICAL1 0

/* Whether we have Fortran 77 LOGICAL*2 or not */
#define OMPI_HAVE_FORTRAN_LOGICAL2 0

/* Whether we have Fortran 77 LOGICAL*4 or not */
#define OMPI_HAVE_FORTRAN_LOGICAL4 0

/* Whether we have Fortran 77 LOGICAL*8 or not */
#define OMPI_HAVE_FORTRAN_LOGICAL8 0

/* Whether we have Fortran 77 REAL or not */
#define OMPI_HAVE_FORTRAN_REAL 0

/* Whether we have Fortran 77 REAL*16 or not */
#define OMPI_HAVE_FORTRAN_REAL16 0

/* Whether we have Fortran 77 REAL*2 or not */
#define OMPI_HAVE_FORTRAN_REAL2 0

/* Whether we have Fortran 77 REAL*4 or not */
#define OMPI_HAVE_FORTRAN_REAL4 0

/* Whether we have Fortran 77 REAL*8 or not */
#define OMPI_HAVE_FORTRAN_REAL8 0

/* Whether IB CM is available or not */
#define OMPI_HAVE_IBCM 0

/* Do not use outside of mpi.h. Define to 1 if the system has the type `long
   long'. */
#define OMPI_HAVE_LONG_LONG 1

/* Do we have POSIX threads */
#define OMPI_HAVE_POSIX_THREADS 1

/* If PTHREADS implementation supports PTHREAD_MUTEX_ERRORCHECK */
#define OMPI_HAVE_PTHREAD_MUTEX_ERRORCHECK 1

/* If PTHREADS implementation supports PTHREAD_MUTEX_ERRORCHECK_NP */
#define OMPI_HAVE_PTHREAD_MUTEX_ERRORCHECK_NP 0

/* Whether RDMA CM is available or not */
#define OMPI_HAVE_RDMACM 0

/* Whether we have SA_RESTART in <signal.h> or not */
#define OMPI_HAVE_SA_RESTART 1

/* Do we have native Solaris threads */
#define OMPI_HAVE_SOLARIS_THREADS 0

/* Do not use outside of mpi.h. Define to 1 if you have the <sys/synch.h>
   header file. */
/* #undef OMPI_HAVE_SYS_SYNCH_H */

/* Do not use outside of mpi.h. Define to 1 if you have the <sys/time.h>
   header file. */
#define OMPI_HAVE_SYS_TIME_H 1

/* Whether we have __va_copy or not */
#define OMPI_HAVE_UNDERSCORE_VA_COPY 1

/* Whether we have va_copy or not */
#define OMPI_HAVE_VA_COPY 1

/* Wehther we have weak symbols or not */
#define OMPI_HAVE_WEAK_SYMBOLS 0

/* Number of arguments to ibv_create_cq */
/* #undef OMPI_IBV_CREATE_CQ_ARGS */

/* Major release number of Open MPI */
#define OMPI_MAJOR_VERSION 1

/* Version of the GM API to use */
/* #undef OMPI_MCA_BTL_GM_API_VERSION */

/* The GM build has or not a broker gm_get function */
/* #undef OMPI_MCA_BTL_GM_GET_BROKEN */

/* Whether we have get_get() or not */
/* #undef OMPI_MCA_BTL_GM_HAVE_RDMA_GET */

/* Whether we have gm_put() or gm_directed_send_with_callback() */
/* #undef OMPI_MCA_BTL_GM_HAVE_RDMA_PUT */

/* The OS support or not the virtal page registration */
/* #undef OMPI_MCA_BTL_GM_SUPPORT_REGISTERING */

/* Whether any opal memchecker mca components were found */
#define OMPI_MEMCHECKER_HAVE_COMPONENT 

/* Whether any opal memory mca components were found */
#define OMPI_MEMORY_HAVE_COMPONENT 0

/* Minor release number of Open MPI */
#define OMPI_MINOR_VERSION 4

/* Type of MPI_Offset -- has to be defined here and typedef'ed later because
   mpi.h does not get AC SUBST's */
#define OMPI_MPI_OFFSET_TYPE long long

/* Whether the C compiler supports "bool" without any other help (such as
   <stdbool.h>) */
#define OMPI_NEED_C_BOOL 1

/* MPI datatype corresponding to MPI_Offset */
#define OMPI_OFFSET_DATATYPE MPI_LONG_LONG

/* Add padding bytes to the openib control header */
#define OMPI_OPENIB_PAD_HDR 0

/* Use the Cray XT-3 implementation of Portals */
#define OMPI_PORTALS_CRAYXT3 0

/* Use the Cray XT-3 implementation of Portals using Modex */
#define OMPI_PORTALS_CRAYXT3_MODEX 0

/* Does Portals send a PTL_EVENT_UNLINK event */
#define OMPI_PORTALS_HAVE_EVENT_UNLINK 1

/* Use the UTCP reference implementation of Portals */
#define OMPI_PORTALS_UTCP 1

/* Whether r notation is used for ppc registers */
/* #undef OMPI_POWERPC_R_REGISTERS */

/* Whether OMPI should provide MPI File interface */
#define OMPI_PROVIDE_MPI_FILE_INTERFACE 1

/* type to use for ptrdiff_t */
#define OMPI_PTRDIFF_TYPE ptrdiff_t

/* Whether Fortran REAL*16 matches the bit format of the equivalent C type */
#define OMPI_REAL16_MATCHES_C 0

/* Release date of Open MPI */
#define OMPI_RELEASE_DATE "Feb 10, 2012"

/* Release release number of Open MPI */
#define OMPI_RELEASE_VERSION 5

/* Do not use outside of mpi.h. The size of a `bool', as computed by sizeof.
   */
#define OMPI_SIZEOF_BOOL 1

/* Size of Fortran 77 COMPLEX */
#define OMPI_SIZEOF_FORTRAN_COMPLEX 4

/* Size of Fortran 77 COMPLEX*16 */
#define OMPI_SIZEOF_FORTRAN_COMPLEX16 4

/* Size of Fortran 77 COMPLEX*32 */
#define OMPI_SIZEOF_FORTRAN_COMPLEX32 4

/* Size of Fortran 77 COMPLEX*8 */
#define OMPI_SIZEOF_FORTRAN_COMPLEX8 4

/* Size of Fortran 77 DOUBLE PRECISION */
#define OMPI_SIZEOF_FORTRAN_DOUBLE_PRECISION 4

/* Size of Fortran 77 INTEGER */
#define OMPI_SIZEOF_FORTRAN_INTEGER 4

/* Size of Fortran 77 INTEGER*1 */
#define OMPI_SIZEOF_FORTRAN_INTEGER1 4

/* Size of Fortran 77 INTEGER*16 */
#define OMPI_SIZEOF_FORTRAN_INTEGER16 4

/* Size of Fortran 77 INTEGER*2 */
#define OMPI_SIZEOF_FORTRAN_INTEGER2 4

/* Size of Fortran 77 INTEGER*4 */
#define OMPI_SIZEOF_FORTRAN_INTEGER4 4

/* Size of Fortran 77 INTEGER*8 */
#define OMPI_SIZEOF_FORTRAN_INTEGER8 4

/* Size of Fortran 77 LOGICAL */
#define OMPI_SIZEOF_FORTRAN_LOGICAL 4

/* Size of Fortran 77 LOGICAL*1 */
#define OMPI_SIZEOF_FORTRAN_LOGICAL1 4

/* Size of Fortran 77 LOGICAL*2 */
#define OMPI_SIZEOF_FORTRAN_LOGICAL2 4

/* Size of Fortran 77 LOGICAL*4 */
#define OMPI_SIZEOF_FORTRAN_LOGICAL4 4

/* Size of Fortran 77 LOGICAL*8 */
#define OMPI_SIZEOF_FORTRAN_LOGICAL8 4

/* Size of Fortran 77 REAL */
#define OMPI_SIZEOF_FORTRAN_REAL 4

/* Size of Fortran 77 REAL*16 */
#define OMPI_SIZEOF_FORTRAN_REAL16 4

/* Size of Fortran 77 REAL*2 */
#define OMPI_SIZEOF_FORTRAN_REAL2 4

/* Size of Fortran 77 REAL*4 */
#define OMPI_SIZEOF_FORTRAN_REAL4 4

/* Size of Fortran 77 REAL*8 */
#define OMPI_SIZEOF_FORTRAN_REAL8 4

/* Do not use outside of mpi.h. The size of a `int', as computed by sizeof. */
#define OMPI_SIZEOF_INT 4

/* Do not use outside of mpi.h. Define to 1 if you have the ANSI C header
   files. */
#define OMPI_STDC_HEADERS 1

/* Do threads have different pids (pthreads on linux) */
#define OMPI_THREADS_HAVE_DIFFERENT_PIDS 0

/* Whether to use <stdbool.h> or not */
#define OMPI_USE_STDBOOL_H 1

/* Complete release number of Open MPI */
#define OMPI_VERSION "5"

/* Whether we want MPI cxx support or not */
#define OMPI_WANT_CXX_BINDINGS 1

/* Whether we want the MPI f77 bindings or not */
#define OMPI_WANT_F77_BINDINGS 0

/* Whether we want the MPI f90 bindings or not */
#define OMPI_WANT_F90_BINDINGS 0

/* Enable per-user config files */
#define OMPI_WANT_HOME_CONFIG_FILES 1

/* Whether to include support for libltdl or not */
#define OMPI_WANT_LIBLTDL 1

/* if the memory and buffer checking should be enabled */
#define OMPI_WANT_MEMCHECKER 0

/* do we want to try to work around C++ bindings SEEK_* issue? */
#define OMPI_WANT_MPI_CXX_SEEK 1

/* if the peruse interface should be enabled */
#define OMPI_WANT_PERUSE 0

/* if want pretty-print stack trace feature */
#define OMPI_WANT_PRETTY_PRINT_STACKTRACE 1

/* whether we want to have smp locks in atomic ops or not */
#define OMPI_WANT_SMP_LOCKS 1

/* Enable fault tolerance general components and logic */
#define OPAL_ENABLE_FT 0

/* Enable fault tolerance checkpoint/restart components and logic */
#define OPAL_ENABLE_FT_CR 0

/* Enable fault tolerance thread in Open PAL */
#define OPAL_ENABLE_FT_THREAD 0

/* Enable IPv6 support, but only if the underlying system supports it */
#define OPAL_ENABLE_IPV6 1

/* Enable run-time tracing of internal functions */
#define OPAL_ENABLE_TRACE 0

/* Greek - alpha, beta, etc - release number of Open Portable Access Layer */
#define OPAL_GREEK_VERSION ""

/* Whether libltdl appears to have the lt_dladvise interface */
#define OPAL_HAVE_LTDL_ADVISE 1

/* Define to 1 ifyou have the declaration of _SC_NPROCESSORS_ONLN, and to 0
   otherwise */
#define OPAL_HAVE__SC_NPROCESSORS_ONLN 1

/* ident string for Open MPI */
#define OPAL_IDENT_STRING "1.4.5"

/* Major release number of Open Portable Access Layer */
#define OPAL_MAJOR_VERSION 1

/* Minor release number of Open Portable Access Layer */
#define OPAL_MINOR_VERSION 4

/* package/branding string for Open MPI */
#define OPAL_PACKAGE_STRING "Open MPI Chunwei@appledeMacBook-Air.local Distribution"

/* Release date of Open Portable Access Layer */
#define OPAL_RELEASE_DATE "Feb 10, 2012"

/* Release release number of Open Portable Access Layer */
#define OPAL_RELEASE_VERSION 5

/* Complete release number of Open Portable Access Layer */
#define OPAL_VERSION "5"

/* Specific ps command to use in orte-clean */
#define ORTE_CLEAN_PS_CMD "ps -A -o command,pid,user"

/* Build full RTE support */
#define ORTE_DISABLE_FULL_SUPPORT 0

/* Greek - alpha, beta, etc - release number of Open Run-Time Environment */
#define ORTE_GREEK_VERSION ""

/* Major release number of Open Run-Time Environment */
#define ORTE_MAJOR_VERSION 1

/* Minor release number of Open Run-Time Environment */
#define ORTE_MINOR_VERSION 4

/* Release date of Open Run-Time Environment */
#define ORTE_RELEASE_DATE "Feb 10, 2012"

/* Release release number of Open Run-Time Environment */
#define ORTE_RELEASE_VERSION 5

/* Complete release number of Open Run-Time Environment */
#define ORTE_VERSION "5"

/* Whether we want orterun to effect "--prefix $prefix" by default */
#define ORTE_WANT_ORTERUN_PREFIX_BY_DEFAULT 0

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "http://www.open-mpi.org/community/help/"

/* Define to the full name of this package. */
#define PACKAGE_NAME "Open MPI"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "Open MPI 1.4.5"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "openmpi"

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.4.5"

/* Version of PLPA embedded in OMPI */
/* #undef PAFFINITY_LINUX_PLPA_VERSION */

/* Whether we are in debugging more or not */
/* #undef PLPA_DEBUG */

/* The PLPA symbol prefix */
/* #undef PLPA_SYM_PREFIX */

/* The PLPA symbol prefix in all caps */
/* #undef PLPA_SYM_PREFIX_CAPS */

/* Whether we want Valgrind support or not */
/* #undef PLPA_WANT_VALGRIND_SUPPORT */

/* The size of `bool', as computed by sizeof. */
#define SIZEOF_BOOL 1

/* The size of `char', as computed by sizeof. */
#define SIZEOF_CHAR 1

/* The size of `double', as computed by sizeof. */
#define SIZEOF_DOUBLE 8

/* The size of `float', as computed by sizeof. */
#define SIZEOF_FLOAT 4

/* The size of `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 8

/* The size of `long double', as computed by sizeof. */
#define SIZEOF_LONG_DOUBLE 16

/* The size of `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG 8

/* The size of `pid_t', as computed by sizeof. */
#define SIZEOF_PID_T 4

/* The size of `ptrdiff_t', as computed by sizeof. */
#define SIZEOF_PTRDIFF_T 8

/* The size of `short', as computed by sizeof. */
#define SIZEOF_SHORT 2

/* The size of `size_t', as computed by sizeof. */
#define SIZEOF_SIZE_T 8

/* The size of `ssize_t', as computed by sizeof. */
#define SIZEOF_SSIZE_T 8

/* The size of `void *', as computed by sizeof. */
#define SIZEOF_VOID_P 8

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* Whether to use the legacy Solaris munmap prototype or not */
/* #undef USE_SOLARIS_LEGACY_MUNMAP_PROTOTYPE */

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* Additional CFLAGS to pass through the wrapper compilers */
#define WRAPPER_EXTRA_CFLAGS " "

/* Additional CXXFLAGS to pass through the wrapper compilers */
#define WRAPPER_EXTRA_CXXFLAGS " "

/* Additional FCFLAGS to pass through the wrapper compilers */
#define WRAPPER_EXTRA_FCFLAGS " "

/* Additional FFLAGS to pass through the wrapper compilers */
#define WRAPPER_EXTRA_FFLAGS " "

/* Additional LDFLAGS to pass through the wrapper compilers */
#define WRAPPER_EXTRA_LDFLAGS "    "

/* Additional LIBS to pass through the wrapper compilers */
#define WRAPPER_EXTRA_LIBS "     -lutil "

/* Define to 1 if `lex' declares `yytext' as a `char *' by default, not a
   `char[]'. */
#define YYTEXT_POINTER 1

/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif

/* Emulated value */
/* #undef __NR_sched_getaffinity */

/* Emulated value */
/* #undef __NR_sched_setaffinity */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
#define inline __inline__
#endif

/* A bogus type that allows us to have sentinel type values that are still
   valid */
#define ompi_fortran_bogus_type_t int

/* C type corresponding to Fortran 77 COMPLEX*16 */
/* #undef ompi_fortran_complex16_t */

/* C type corresponding to Fortran 77 COMPLEX*32 */
/* #undef ompi_fortran_complex32_t */

/* C type corresponding to Fortran 77 COMPLEX*8 */
/* #undef ompi_fortran_complex8_t */

/* C type corresponding to Fortran 77 COMPLEX */
/* #undef ompi_fortran_complex_t */

/* C type corresponding to Fortran 77 DOUBLE PRECISION */
#define ompi_fortran_double_precision_t ompi_fortran_bogus_type_t

/* C type corresponding to Fortran 77 INTEGER*16 */
#define ompi_fortran_integer16_t ompi_fortran_bogus_type_t

/* C type corresponding to Fortran 77 INTEGER*1 */
#define ompi_fortran_integer1_t ompi_fortran_bogus_type_t

/* C type corresponding to Fortran 77 INTEGER*2 */
#define ompi_fortran_integer2_t ompi_fortran_bogus_type_t

/* C type corresponding to Fortran 77 INTEGER*4 */
#define ompi_fortran_integer4_t ompi_fortran_bogus_type_t

/* C type corresponding to Fortran 77 INTEGER*8 */
#define ompi_fortran_integer8_t ompi_fortran_bogus_type_t

/* C type corresponding to Fortran 77 INTEGER */
#define ompi_fortran_integer_t ompi_fortran_bogus_type_t

/* C type corresponding to Fortran 77 LOGICAL*1 */
#define ompi_fortran_logical1_t ompi_fortran_bogus_type_t

/* C type corresponding to Fortran 77 LOGICAL*2 */
#define ompi_fortran_logical2_t ompi_fortran_bogus_type_t

/* C type corresponding to Fortran 77 LOGICAL*4 */
#define ompi_fortran_logical4_t ompi_fortran_bogus_type_t

/* C type corresponding to Fortran 77 LOGICAL*8 */
#define ompi_fortran_logical8_t ompi_fortran_bogus_type_t

/* C type corresponding to Fortran 77 LOGICAL */
#define ompi_fortran_logical_t ompi_fortran_bogus_type_t

/* C type corresponding to Fortran 77 REAL*16 */
#define ompi_fortran_real16_t ompi_fortran_bogus_type_t

/* C type corresponding to Fortran 77 REAL*2 */
#define ompi_fortran_real2_t ompi_fortran_bogus_type_t

/* C type corresponding to Fortran 77 REAL*4 */
#define ompi_fortran_real4_t ompi_fortran_bogus_type_t

/* C type corresponding to Fortran 77 REAL*8 */
#define ompi_fortran_real8_t ompi_fortran_bogus_type_t

/* C type corresponding to Fortran 77 REAL */
#define ompi_fortran_real_t ompi_fortran_bogus_type_t

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define to the equivalent of the C99 'restrict' keyword, or to
   nothing if this is not supported.  Do not define if restrict is
   supported directly.  */
#define restrict __restrict
/* Work around a bug in Sun C++: it does not support _Restrict, even
   though the corresponding Sun C compiler does, which causes
   "#define restrict _Restrict" in the previous line.  Perhaps some future
   version of Sun C++ will work with _Restrict; if so, it'll probably
   define __RESTRICT, just as Sun C does.  */
#if defined __SUNPRO_CC && !defined __RESTRICT
# define _Restrict
#endif

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */


#include "opal_config_bottom.h"
#endif /* OPAL_CONFIG_H */

