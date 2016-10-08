import QtQuick 2.0
import "qrc:/settings.js" as Settings

ListModel{
    id:model

    property Client client

    property string topPlayerColor
    property string bottomPlayerColor

    property bool isMyTurnNow: true
    property bool iAmBottomPlayer: true

    signal manMoved(var sourceCell,var destinationCell)

    function isMoveCorrect(sourceCell,destinationCell){
        if((iAmBottomPlayer&&sourceCell.man.whoose !=="bottomPlayer")||(!iAmBottomPlayer && sourceCell.man.whoose !=="topPlayer")){
            console.log("You cannot control enemy's mans.")
            return false
        }else if(destinationCell.man.rank !== "none"){
            console.log("Destination cell isn't empty error.")
            return false
        }else if(sourceCell.man.rank==="none"){
            console.log("There is nothing to move in cell "+Settings.getName(sourceCell.row,sourceCell.col))
            return false
        }else if(sourceCell === destinationCell){
            console.log("Source = destination error.")
            return false
        }else if(!isMyTurnNow){
            console.log("It's not your turn error.")
            return false
        }else if(!Settings.isBlack(sourceCell.row,sourceCell.col)
                 || !Settings.isBlack(destinationCell.row,destinationCell.col)){
            console.log("Only black fields are usable.")
            return false
        }else if(iAmBottomPlayer && (destinationCell.row-sourceCell.row !== 1)){
            console.log("Bottom player cannot move back or too far front."+sourceCell.row+">"+destinationCell.row)
            return false
        }else if(!iAmBottomPlayer && (sourceCell.row-destinationCell.row !== 1)){
            console.log("Top player cannot move back or too far front."+sourceCell.row+"<"+destinationCell.row)
            return false
        }else
            return true
    }

    function requestMove(sourceCell,destinationCell){
        if(isMoveCorrect(sourceCell,destinationCell)){
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
            clear()
            var board=reply["board"]
            for(var i in board){
                if (!board.hasOwnProperty(i)) continue;
                append(board[i])
            }
        }
    }

    function moveMan(from,to){
        var fromIndex = Settings.indexOf(from["row"],from["col"])
        var toIndex = Settings.indexOf(to["row"],to["col"])

        set(fromIndex,from)
        set(toIndex,to)
    }
}
