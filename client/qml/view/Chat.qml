import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import "../logic"

ColumnLayout {
    id: root

    property alias sendButton: sendButton
    property alias userInput: userInput

    property alias client: chatter.client

    property string playersMessagesColor : "steelblue"
    property string enemyMessagesColor: "red"

    Chatter{
        id:chatter
        onMessageReceived: {
            chatView.model.append(message)
        }
    }

    Flickable{
        anchors{
            top: parent.top
            bottom: userLayout.top
            left: parent.left
            right: parent.right
            margins: 10
        }
        ListView{
            id:chatView
            anchors.fill: parent
            add: Transition {
                NumberAnimation { properties: "x"; from: chatView.width/2; duration: 100; easing.type: Easing.OutBounce }
            }
            model:ListModel{}
            delegate: Text{
                text: model.sender+":"+model.text
                color:model.type === "confMessage" ?
                        model.sender === client.name ? playersMessagesColor : enemyMessagesColor
                        : "blue"
            }
        }
    }

    RowLayout {
        id: userLayout

        anchors{
            left:parent.left
            right: parent.right
            bottom: parent.bottom
        }

        TextField {
            id: userInput
            Layout.fillWidth : true
            focus: true
            placeholderText: "Type message here..."
            onAccepted: root.sendMessage()
        }

        Button {
            id: sendButton
            text: qsTr("send")
            onClicked: root.sendMessage()
        }

    }

    function sendMessage(){
        if(userInput.text !== ""){
            var message={type:"confMessage",conference:"main",sender:client.name,text:userInput.text}
            client.send(message)
            userInput.text=""
        }
    }

}
