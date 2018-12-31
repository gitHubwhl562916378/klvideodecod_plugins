#ifndef KLVIDEOWIDGET_GLOBAL_H
#define KLVIDEOWIDGET_GLOBAL_H

#ifdef linux
#if defined(KLVIDEOWIDGET_LIBRARY)
#  define KLVIDEOWIDGETSHARED_EXPORT __attribute__((visibility("default")))
#else
#  define KLVIDEOWIDGETSHARED_EXPORT __attribute__((visibility("default")))
#endif
#else
#if defined(KLVIDEOWIDGET_LIBRARY)
#  define KLVIDEOWIDGETSHARED_EXPORT __declspec(dllexport)
#else
#  define KLVIDEOWIDGETSHARED_EXPORT __declspec(dllimport)
#endif
#endif

#endif // KLVIDEOWIDGET_GLOBAL_H
