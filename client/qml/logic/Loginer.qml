import QtQuick 2.0

Item {
    property Client client

    property string loginName
    property string password

    signal logined()

    function login(){
        console.log("loggining")
        var message={type:"loginRequest" ,login:loginName ,password:password}
        client.send(message)
    }

    function parseLoginReply(message){
        if(message["type"]==="loginReply"){
            if(message["verdict"] === "true"){
                logined()
            }else{
                client.errorOccured(message["reason"])
                client.deactivate()
            }
        }
    }

    Connections{
        target: client
        onReplyReceived: parseLoginReply(message)
    }
}
