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

    delegate: Rectangle{
        id: cellDelegate
        width: grid.cellWidth
        height: grid.cellHeight
        color: index==grid.currentIndex ?
                   Settings.selectedCellColor:
                   cell.isBlack ? Settings.blackCellColor : Settings.whiteCellColor

        property Cell cell:Cell{
            id:cell
            row: model.row
            col: model.col
            man: model.man
        }

        MouseArea{
            anchors.fill: parent
            enabled: cell.isEmpty
            onClicked: {
                if(grid.currentIndex !== -1){
                    checkersModel.requestMove(checkersModel.get(grid.currentIndex),checkersModel.get(index))
                    grid.currentIndex = -1
                }
            }
        }

        Text{
            id:nameOfCell
            visible: Settings.debug
            color: Settings.debugTextColor
            text: cell.name
        }

        Rectangle{
            id:manDelegate
            anchors.fill: parent
            anchors.margins: 10
            radius: width
            color: "transparent"
            scale: 0

            property bool isMine: iAmBottomPlayer ?
                                      cell.man["whoose"] === "bottomPlayer"
                                    : cell.man["whoose"] === "topPlayer"
            property string manColor: cell.man["whoose"] === "bottomPlayer" ?
                                          Settings.bottomPlayerColor
                                        : Settings.topPlayerColor
            MouseArea{
                anchors.fill: parent
                enabled: parent.isMine && checkersModel.isMyTurnNow
                onClicked: grid.currentIndex = model.index
            }

            states:[
                State{
                    name: "man"
                    when: cell.man["rank"] === "man"
                    PropertyChanges {
                        target: manDelegate
                        color: manColor
                        scale: 1.0
                    }
                }
                ,
                State{
                    name: "king"
                    extend: "man"
                    when: cell.man["rank"] === "king"
                    PropertyChanges{
                        target: manDelegate
                        color: Qt.darker(manColor)
                    }
                }

            ]

            transitions: [
                Transition {
                    NumberAnimation{properties:"scale,color"}
                }
            ]
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
