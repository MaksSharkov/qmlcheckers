import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import Qt.WebSockets 1.0

import "../logic"

ApplicationWindow {
    visible: true
    width: 700
    height: 400
    title: qsTr("QCheckers")

    Client{
        id: client
        name: loginPage.loginName
    }

    LoginPage{
        id: loginPage
        client:client
        visible: true
        anchors.centerIn: parent
        onLogined: {
            console.log("logined.")
            switchToMainPage()
        }
        Connections{
            target:client
            onErrorOccured:{
                loginPage.errorMessage = errorMessage
            }
            onDisconnected: onDisconnected()
        }
    }

    GameScreen{
        id: gameScreen
        client: client
        anchors.fill: parent
        visible: false
    }

    function switchToMainPage(){
        loginPage.visible = false
        gameScreen.visible = true
    }

    function onDisconnected(){
        loginPage.errorMessage = "Disconnected from server"
        loginPage.visible = true
        gameScreen.visible = false
    }

}
