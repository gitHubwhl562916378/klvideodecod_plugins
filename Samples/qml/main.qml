import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import MyModel 1.0
ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("rtsp video")

    VideoRender{
        id: rtspVieo
        anchors.fill: parent
        videoSource: "rtsp://admin:123ABCabc@192.168.2.239/h264/ch1/main/av_stream" //rtsp:192.168.2.22:5554/1
        decoder: "h264_cuvid" //传空或不传为软解；传h264_cuvid为硬解，当系统不支持时自动变为软解
        Text{
            id: errorText
            anchors.centerIn: parent
            font.pixelSize: 14
            color: "red"
        }
        onVideoStarted: {
            errorText.visible = false
        }
        onErrorOcured: {
            errorText.visible = true
            errorText.text = rtspVieo.error
        }

        Component.onCompleted: {
            play()
        }
    }
}
