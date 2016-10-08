import QtQuick 2.0
import Qt.WebSockets 1.0

Item {
    id:client

    property string name
    property alias hostUrl: socket.url
    property alias isActive: socket.active

    signal errorOccured(string errorMessage)
    signal disconnected()
    signal connected()
    signal replyReceived(var message)

    WebSocket{
        id:socket
        active:false
        onStatusChanged: {
            if (status == WebSocket.Error) {
                                     errorOccured("Error: " + socket.errorString)
                                     deactivate()
                                 } else if (socket.status === WebSocket.Open) {
                                     connected()
                                 } else if (socket.status === WebSocket.Closed) {
                                     disconnected()
                                     deactivate()
                                 }
        }

        onTextMessageReceived: {
            try{
                var received=JSON.parse(message)
                replyReceived(received)
            }catch(error){
                errorOccured("Error while parsing login reply.")
                deactivate()
            }
        }
    }

    function send(message){
        var jsonMessage=JSON.stringify(message)
        socket.sendTextMessage(jsonMessage)
    }

    function deactivate(){
        isActive =false
    }

    function activate(){
        isActive = true
    }

}
