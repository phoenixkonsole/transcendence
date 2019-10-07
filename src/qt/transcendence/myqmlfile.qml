import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

Pane {
    width: 120
    height: 120

    Material.elevation: 6

    Label {
        text: qsTr("I'm a card!")
        anchors.centerIn: parent
    }
}