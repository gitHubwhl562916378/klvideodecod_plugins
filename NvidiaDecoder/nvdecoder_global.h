#ifndef NVDECODER_GLOBAL_H
#define NVDECODER_GLOBAL_H

#ifdef linux
#if defined(NVDECODER_LIBRARY)
#  define NVDECODERSHARED_EXPORT __attribute__((visibility("default")))
#else
#  define NVDECODERSHARED_EXPORT __attribute__((visibility("default")))
#endif
#else
#if defined(NVDECODER_LIBRARY)
#  define NVDECODERSHARED_EXPORT __declspec(dllexport)
#else
#  define NVDECODERSHARED_EXPORT __declspec(dllimport)
#endif
#endif

#endif // NVDECODER_GLOBAL_H
