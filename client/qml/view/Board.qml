import QtQuick 2.7
import QtQuick.Controls 2.0
import "qrc:/settings.js" as Settings
import Checkers 1.0
import "../logic"

GridView{
    id: grid

    signal gameEnded(string winner)

    property alias boardSize: checkersModel.boardSize
    property alias client: checkersModel.client

    property bool iAmBottomPlayer: checkersModel.role === "bottomPlayer"

    property alias role: checkersModel.role

    layoutDirection: iAmBottomPlayer ?
                         GridView.LeftToRight
                       : GridView.RightToLeft

    verticalLayoutDirection: iAmBottomPlayer ?
                                 GridView.BottomToTop
                               : GridView.TopToBottom

    cellHeight: height / boardSize
    cellWidth: width / boardSize

    currentIndex: -1

    model: ChessModel{
        id:checkersModel
    }

    delegate: CellDelegate {
        id: cellDelegate
        height: grid.cellHeight
        width: grid.cellWidth
        color: index==grid.currentIndex ? Settings.selectedCellColor:cellColor
        cell:Cell{
            id:cell
            row: model.row
            col: model.col
            man: model.man
        }

        onManSelected: grid.currentIndex = model.index
        onCellSelected: {
            if(grid.currentIndex !== -1){
                checkersModel.requestMove(checkersModel.get(grid.currentIndex),checkersModel.get(index))
                grid.currentIndex = -1
            }
        }
    }

    BusyIndicator{
        id:busyIndicator
        anchors.fill: parent
        visible: !checkersModel.isInitialized
        Button{
            text: "Request Bot"
            visible: parent.visible
            anchors.centerIn: parent
            onClicked:{
                var message={type:"requestBot"}
                client.send(message)
            }
        }
    }

    function parseReply(reply){
        if(reply["type"]==="gameEnded"){
                    gameEnded(reply["winner"])
                }
    }

    Connections{
        target:client
        onReplyReceived:{
            checkersModel.parseReply(message)
            parseReply(message)
        }

        onDisconnected: checkersModel.deinitialize()
    }
}
