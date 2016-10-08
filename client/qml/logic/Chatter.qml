import QtQuick 2.0

Item {
    property Client client

    signal messageReceived(var message)

    function parseMessageResponse(message){
        if(message["type"]==="confMessage"||message["type"]==="pmMessage"){
            messageReceived(message)
        }
    }

    Connections{
        target:client
        onReplyReceived: parseMessageResponse(message)
    }
}
