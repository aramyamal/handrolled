#ifndef HANDROLLED_COMMON_H
#define HANDROLLED_COMMON_H

#ifndef HR_MAYBE_UNUSED
#if defined(_MSC_VER)
#define HR_MAYBE_UNUSED __pragma(warning(suppress : 4505))
#elif defined(__GNUC__) || defined(__clang__)
#define HR_MAYBE_UNUSED __attribute__((unused))
#else
#define HR_MAYBE_UNUSED
#endif
#endif

#endif // HANDROLLED_COMMON_H
