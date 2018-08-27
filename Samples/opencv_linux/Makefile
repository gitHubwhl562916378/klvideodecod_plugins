INCLUDEPATH += -I/usr/local/opencv-3.4.0/include
INCLUDEPATH += -I/usr/local/opencv-3.4.0/include/opencv2
INCLUDEPATH += -I../../package/include
INCLUDEPATH += -I../../3rd/ffmpeg/linux/include
INCLUDEPATH += -I/usr/local/cuda/include
LIBS += -L/usr/local/opencv-3.4.0/lib -L../../package/lib/linux/c++ -lopencv_core -lopencv_highgui -lopencv_imgproc -lklvideodecod

all: build

build: rtsp_opencv

main.o: main.cpp
	g++ -std=c++11 $(INCLUDEPATH) -o $@ -c $<

rtsp_opencv: main.o
	g++ -std=c++11 $(INCLUDEPATH) -o $@ $^ $(LIBS)

clean:
	rm -rf main.o rtsp_opencv
