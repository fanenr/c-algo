#ifndef UTIL_H
#define UTIL_H

#define attr_nonnull(...) __attribute__ ((nonnull (__VA_ARGS__)))

#define is_same_t(a, b) __builtin_types_compatible_p (a, b)
#define is_same_v(a, b) is_same_t (typeof (a), typeof (b))

#define container_of(ptr, type, member)                                       \
  ({                                                                          \
    void *mptr = (void *)(ptr);                                               \
    static_assert (is_same_v (*(ptr), ((type *)0)->member)                    \
                       || is_same_v (*(ptr), void),                           \
                   "pointer type mismatch in container_of");                  \
    ((type *)(mptr - offsetof (type, member)));                               \
  })

#endif
