import QtQuick 2.0

import "qrc:/settings.js" as Settings
import Checkers 1.0

CheckersBoard{
    id:model

    property Client client

    property string topPlayerColor
    property string bottomPlayerColor

    property bool isMyTurnNow: false
    property string role: "spectator"
    property bool isInitialized: false

    function requestMove(sourceCell,destinationCell){
        if(isMyTurnNow && isMoveCorrect(role,sourceCell,destinationCell)){
            var from={row:sourceCell.row,col:sourceCell.col}
            var to={row:destinationCell.row,col:destinationCell.col}
            var request={type:"moveMan",from:from,to:to}
            client.send(request)
            console.debug("Request to move . "+JSON.stringify(request))
        }
    }

    function parseReply(reply){
        if(reply["type"]==="moveMan"){
            var from=reply["from"]
            var to=reply["to"]
            moveMan(reply["player"],from,to)
            if(role !== "spectator")
                isMyTurnNow= !isMyTurnNow

        }else if(reply["type"]==="gameInit"){
            var board=reply["board"]
            var boardSize=reply["boardSize"]

            var amIbottom = client.name === reply["bottomPlayer"]
            if(amIbottom)
                role = "bottomPlayer"
            else if(client.name === reply["topPlayer"])
                role = "topPlayer"

            isMyTurnNow = amIbottom
            initialize(board,boardSize)
            isInitialized=true
        }
    }

}
