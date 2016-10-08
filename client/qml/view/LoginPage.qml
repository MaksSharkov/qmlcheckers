import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import "../logic"

ColumnLayout {
    id: loginPage
    property alias errorMessage: errorLine.text
    property alias loginName: loginField.text

    property alias client:loginer.client

    signal logined()

    Loginer{
        id:loginer
        loginName: loginField.text
        password: passwordField.text
        onLogined: loginPage.logined()
    }

    TextField {
        id: loginField
        text: "user1"
        placeholderText: qsTr("Type login here.")
    }

    TextField {
        id: passwordField
        text: "user1"
        placeholderText: qsTr("Type password here.")
        echoMode: TextInput.PasswordEchoOnEdit
    }

    Row{
        Button {
            id: loginButton
            text: qsTr("Login")
            onClicked: {
                errorLine.text = ""
                client.hostUrl="ws://"+serverIpField.text+":"+serverPortBox.value
                client.activate
            }
        }

        CheckBox {
            id: advancedCheckbox
            text: qsTr("Advanced")
        }
    }


    TextArea {
        id: errorLine
        readOnly: true
        horizontalAlignment: Text.AlignHCenter
        Layout.alignment: Qt.AlignHCenter
        color: "red"
    }


    ColumnLayout {
        id: advancedLayout
        visible: advancedCheckbox.checked

        TextField {
            id: serverIpField
            text: "127.0.0.1"
            validator:RegExpValidator
            {
            regExp:/\b((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\.|$)){4}\b/
        }
    }
    SpinBox {
        id: serverPortBox
        Layout.alignment: Qt.AlignHCenter
        value: 8080
        to: 655535
    }
}

    Connections{
    target: client
    onConnected: loginer.login()
}
}

