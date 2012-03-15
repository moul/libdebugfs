#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <sys/types.h>

#define		REALNAME(func)		__real_ ## func

void		__debugfs_init(void);
void		__debugfs_atexit(void);

#define			REAL(FUNC, RET_TYPE, ARGS_PROTO, ARGS_PARAM)	\
  RET_TYPE              __real_ ## FUNC ARGS_PROTO {			\
    static RET_TYPE     (*func)ARGS_PROTO;                              \
    									\
    if (!func) {							\
      func = (RET_TYPE (*)ARGS_PROTO)dlsym(RTLD_NEXT, #FUNC);           \
    }									\
    return (func ARGS_PARAM);                                           \
  }                                                                     \
  RET_TYPE      FUNC ARGS_PROTO                                         \
  {                                                                     \
    __debugfs_init();							\
    return (__real_ ## FUNC ARGS_PARAM);				\
  }

REAL(open,	int,	(char * filename, int flags, int mode),						(filename, flags, mode))
REAL(fopen,	FILE *,	(__const char *__restrict __filename, __const char *__restrict __modes),	(__filename, __modes))

void		__debugfs_atexit(void) {
  printf("[+] Exiting debugfs\n");
}

void		__debugfs_init(void) {
  static int	init = 0;

  if (0 == init) {
    printf("[+] Initializing debugfs\n");
    init = 1;
    atexit(__debugfs_atexit);
  }
}

