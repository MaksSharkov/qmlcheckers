import QtQuick 2.0

import "qrc:/settings.js" as Settings
import Checkers 1.0

CheckersBoard{
    id:model

    property Client client

    property string topPlayerColor
    property string bottomPlayerColor

    property bool isMyTurnNow: true
    property bool iAmBottomPlayer: true

    function requestMove(sourceCell,destinationCell){
//        if(isMoveCorrect(sourceCell,destinationCell)){
        if(isMyTurnNow){
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
            moveMan(from,to)
            isMyTurnNow= !isMyTurnNow
        }else if(reply["type"]==="gameInit"){
            var board=reply["board"]
            var boardSize=reply["boardSize"]
            initialize(board,boardSize)
        }
    }

}
