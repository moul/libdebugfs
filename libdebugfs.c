#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <sys/types.h>

# define REAL(FUNC, RET_TYPE, ARGS_PROTO, ARGS_PARAM)                   \
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

REAL(open, int, (char * filename, int flags, int mode), (filename, flags, mode))

int __debugfs_init(void) {
  // do nothing
}
