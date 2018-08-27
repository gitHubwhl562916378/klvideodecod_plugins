#ifndef KLVIDEODECOD_GLOBAL_H
#define KLVIDEODECOD_GLOBAL_H

#ifdef linux
#if defined(KLVIDEODECOD_LIBRARY)
#  define KLVIDEODECODSHARED_EXPORT __attribute__((visibility("default")))
#else
#  define KLVIDEODECODSHARED_EXPORT __attribute__((visibility("default")))
#endif
#else
#if defined(KLVIDEODECOD_LIBRARY)
#  define KLVIDEODECODSHARED_EXPORT __declspec(dllexport)
#else
#  define KLVIDEODECODSHARED_EXPORT __declspec(dllimport)
#endif
#endif

#endif // KLVIDEODECOD_GLOBAL_H
