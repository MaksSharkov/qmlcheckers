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

    header:     UserdataBlock{
        id:userData
        visible: gameScreen.visible
        client: client
        username: client.name
        editable: true
        status: "Testing..."
    }

    LoginPage{
        id: loginPage
        client:client
        visible: true
        anchors.centerIn: parent
        onLogined: {
            console.log("logined.")
            client.disconnected.connect(onDisconnected)
            switchToMainPage()
        }
        Connections{
            target:client
            onErrorOccured:{
                loginPage.errorMessage = errorMessage
            }
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
        client.disconnected.disconnect(onDisconnected)

        loginPage.visible = true
        gameScreen.visible = false
    }

}
