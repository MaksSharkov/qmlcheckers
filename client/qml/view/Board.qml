import QtQuick 2.7
import QtQuick.Controls 2.0
import "qrc:/settings.js" as Settings
import Checkers 1.0
import "../logic"

GridView{
    id: grid

    property int boardSize: Settings.defaultBoardSize
    property alias client: checkersModel.client

    verticalLayoutDirection: GridView.BottomToTop

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

        readonly property string manColor: !cell.isEmpty ?
                                             cell.man.whoose === "topPlayer" ?
                                                   checkersModel.topPlayerColor
                                                 : checkersModel.bottomPlayerColor
                                             : color

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
            visible: !cell.isEmpty
            color: manColor
            MouseArea{
                anchors.fill: parent
                enabled: parent.visible
                onClicked:{
                    grid.currentIndex = model.index
                }
            }
        }
    }

    Connections{
        target:client
        onReplyReceived:{
            checkersModel.parseReply(message)
        }

    }
}
