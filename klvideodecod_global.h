#ifndef KLVIDEODECOD_GLOBAL_H
#define KLVIDEODECOD_GLOBAL_H

#if defined(KLVIDEODECOD_LIBRARY)
#  define KLVIDEODECODSHARED_EXPORT __attribute__((visibility("default")))
#else
#  define KLVIDEODECODSHARED_EXPORT __attribute__((visibility("default")))
#endif

#endif // KLVIDEODECOD_GLOBAL_H
