#ifndef FMGDECODER_GLOBAL_H
#define FMGDECODER_GLOBAL_H

#ifdef linux
#if defined(FMGDECODER_LIBRARY)
#  define FMGDECODERSHARED_EXPORT __attribute__((visibility("default")))
#else
#  define FMGDECODERSHARED_EXPORT __attribute__((visibility("default")))
#endif
#else
#if defined(FMGDECODER_LIBRARY)
#  define FMGDECODERSHARED_EXPORT __declspec(dllexport)
#else
#  define FMGDECODERSHARED_EXPORT __declspec(dllimport)
#endif
#endif

#endif // FMGDECODER_GLOBAL_H
