#ifndef GCC_TM_H
#define GCC_TM_H
#ifndef LIBC_GLIBC
# define LIBC_GLIBC 1
#endif
#ifndef LIBC_UCLIBC
# define LIBC_UCLIBC 2
#endif
#ifndef LIBC_BIONIC
# define LIBC_BIONIC 3
#endif
#ifndef LIBC_MUSL
# define LIBC_MUSL 4
#endif
#ifndef L2_MAX_OFILE_ALIGNMENT
# define L2_MAX_OFILE_ALIGNMENT 28U
#endif
#ifndef DEF_MIN_OSX_VERSION
# define DEF_MIN_OSX_VERSION "12.0"
#endif
#ifndef DEF_LD64
# define DEF_LD64 "609.0"
#endif
#ifndef STACK_USE_CUMULATIVE_ARGS_INIT
# define STACK_USE_CUMULATIVE_ARGS_INIT 0
#endif
#ifndef OFF_STACK_TRAMPOLINES_INIT
# define OFF_STACK_TRAMPOLINES_INIT 1
#endif
#ifndef TARGET_64BIT_DEFAULT
# define TARGET_64BIT_DEFAULT (OPTION_MASK_ISA_64BIT|OPTION_MASK_ABI_64)
#endif
#ifndef TARGET_BI_ARCH
# define TARGET_BI_ARCH 0
#endif
#ifndef DARWIN_AT_RPATH
# define DARWIN_AT_RPATH 1
#endif
#ifdef IN_GCC
# include "options.h"
# include "insn-constants.h"
# include "config/vxworks-dummy.h"
# include "config/i386/i386.h"
# include "config/darwin.h"
# include "config/i386/darwin.h"
# include "config/initfini-array.h"
#endif
#if defined IN_GCC && !defined GENERATOR_FILE && !defined USED_FOR_TARGET
# include "insn-flags.h"
#endif
#if defined IN_GCC && !defined GENERATOR_FILE
# include "insn-modes.h"
#endif
# include "defaults.h"
#endif /* GCC_TM_H */
