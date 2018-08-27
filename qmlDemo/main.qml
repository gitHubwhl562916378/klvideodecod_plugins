import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import MyModel 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Rtsp Video")

    VideoRender{
        anchors.fill: parent
        videoSource: "rtsp:192.168.2.22:5554/1"
        decoder: "h264_cuvid"
        Component.onCompleted: {
            play()
        }
    }
}
