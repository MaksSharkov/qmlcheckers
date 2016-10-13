import QtQuick 2.0
import Qt.WebSockets 1.0

Item {
    id:client

    property string name
    property alias hostUrl: socket.url

    signal errorOccured(string errorMessage)
    signal disconnected()
    signal connected()
    signal replyReceived(var message)

    WebSocket{
        id:socket
        active:true
        onStatusChanged: {
            if (status == WebSocket.Error) {
                                     errorOccured("Error: " + socket.errorString)
                                     active=false
                                 } else if (socket.status === WebSocket.Open) {
                                     console.debug("connected.")
                                     connected()
                                 } else if (socket.status === WebSocket.Closed) {
                                     disconnected()
                                     active=false
                                 }
        }

        onTextMessageReceived: {
            try{
                var received=JSON.parse(message)
                replyReceived(received)
            }catch(error){
                errorOccured("Error while parsing login reply.")
                socket.active=false
            }
        }
    }

    function send(message){
        var jsonMessage=JSON.stringify(message)
        socket.sendTextMessage(jsonMessage)
    }

    function activate(){
        socket.active = true
    }
}
