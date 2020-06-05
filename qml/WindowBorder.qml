import QtQuick 2.0
import QtQuick.Window 2.3

Rectangle {
    id: control

    property string foregroundColor: "#242424"
    property string backgroundColor: "#969696"

    property bool changeColor: true
    property bool active: changeColor?Window.active: true

    anchors.fill: parent

    border.width: 1
    color: "transparent"
    border.color: active ? foregroundColor : backgroundColor

}
