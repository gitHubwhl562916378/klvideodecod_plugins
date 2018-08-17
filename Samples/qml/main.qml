import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import MyModel 1.0
ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    VideoRender{
        anchors.fill: parent
        decoder: "h264_cuvid" //h264_cuvid硬解 空字符串软解
        videoSource: "rtsp://admin:123ABCabc@192.168.2.239/h264/ch1/main/av_stream"
        Component.onCompleted: {
            play()
        }
    }
}
