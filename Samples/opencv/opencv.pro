TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt

SOURCES += main.cpp

INCLUDEPATH +=/usr/local/ffmpeg/include \
              /usr/local/klvideodecod/include \
              /usr/local/opencv-3.0.0/include \
              /usr/local/opencv-3.0.0/include/opencv2 \
              /usr/local/cuda/include

LIBS += -L/usr/local/opencv-3.0.0/lib -L$$PWD/../.. -lklvideodecod -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs\
