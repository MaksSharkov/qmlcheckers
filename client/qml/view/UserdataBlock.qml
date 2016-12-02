import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0

import "../logic"

RowLayout{
    id: block
    property Client client
    property bool editable: false
    property alias username: usernameText.text
    property alias status: statusText.text

    Text{
        id: usernameText
    }

    TextInput{
        id: statusText
        readOnly: true
        onAccepted: closeStatusEditMenu(true)
        MouseArea{
            id: clickArea
            anchors.fill: parent
            enabled: editable
            onClicked: openStatusEditMenu()
        }
    }

    RowLayout{
        id: statusEditMenu

        property string oldtext

        visible: ! statusText.visible

        Button{
            id: submitStatusButton
            visible: statusEditMenu.visible
            text: "Submit"
            onClicked: closeStatusEditMenu(true)
        }
        Button{
            id: cancelStatusButton
            visible: statusEditMenu.visible
            text: "Cancel"
            onClicked: closeStatusEditMenu(false)
        }
    }

    function openStatusEditMenu() {        
        statusEditMenu.oldtext = statusText.text
        statusText.readOnly = false
        statusEditMenu.visible = true
        statusText.focus = true
    }

    function closeStatusEditMenu(submitRequested) {
        if(!submitRequested)
            statusText.text = statusEditMenu.oldtext

        statusEditMenu.visible = false
        statusText.readOnly = true
    }

    function updateInfo() {
        var request={type:"publicInfoRequest" ,username:username}
        client.send(request)
    }

    function parseReply(reply) {
        if(reply["type"] === "publicInfo"
                && reply["username"] === username) {
            status = reply["status"]
        }
    }

    Connections{
        target:client
        onReplyReceived: parseReply(message)
    }
}
