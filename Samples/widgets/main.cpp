#include <QApplication>
#include "Videowidget/klvideowidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Klvideowidget w;
    w.startPlay("rtsp:192.168.2.22:5554/1","h264_cuvid");
    w.show();

    return a.exec();
}
