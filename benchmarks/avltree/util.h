#ifndef UTIL_H
#define UTIL_H

#define attr_nonnull(...) __attribute__ ((nonnull (__VA_ARGS__)))

#define is_same_type(a, b)                                                    \
  __builtin_types_compatible_p (typeof (a), typeof (b))

#define container_of(ptr, type, member)                                       \
  ({                                                                          \
    void *mptr = (void *)(ptr);                                               \
    static_assert (is_same_type (*(ptr), ((type *)0)->member)                 \
                       || is_same_type (*(ptr), void),                        \
                   "pointer type mismatch in container_of");                  \
    ((type *)(mptr - offsetof (type, member)));                               \
  })

#endif
