import QtQuick 2.7
import QtQuick.Controls 2.0
import "qrc:/settings.js" as Settings
import Checkers 1.0
import "../logic"

GridView{
    id: grid

    property alias boardSize: checkersModel.boardSize
    property alias client: checkersModel.client

    property bool iAmBottomPlayer: checkersModel.role === "bottomPlayer"
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
        topPlayerColor: Settings.aiManColor
        bottomPlayerColor: Settings.playerManColor
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

        readonly property string manColor: cell.man["whoose"] === "topPlayer" ?
                                                   checkersModel.topPlayerColor
                                                 : checkersModel.bottomPlayerColor

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
            Behavior on color {
                ColorAnimation {
                    duration: 600
                }
            }

            property bool isMine: iAmBottomPlayer ?
                                      cell.man["whoose"] === "bottomPlayer"
                                    : cell.man["whoose"] === "topPlayer"
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
                    }
                }
                ,
                State{
                    name: "king"
                    when: cell.man["rank"] === "king"
                    PropertyChanges{
                        target: manDelegate
                        color: Qt.darker(manColor)
                    }
                }

            ]
        }
    }

    BusyIndicator{
        id:busyIndicator
        anchors.fill: parent
        visible: !checkersModel.isInitialized
    }

    Connections{
        target:client
        onReplyReceived:{
            checkersModel.parseReply(message)
        }

    }
}
