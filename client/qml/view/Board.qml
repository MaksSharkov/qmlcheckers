import QtQuick 2.7
import QtQuick.Controls 2.0
import "qrc:/settings.js" as Settings

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
                   Settings.isBlack(row,col) ? Settings.blackCellColor : Settings.whiteCellColor

        readonly property string name: Settings.getName(row,col)
        readonly property bool hasMan: model.man.rank !== "none"
        readonly property string manColor: hasMan ?
                                             model.man.whoose === "topPlayer" ?
                                                   checkersModel.topPlayerColor
                                                 : checkersModel.bottomPlayerColor
                                             : color

        MouseArea{
            anchors.fill: parent
            enabled: !cellDelegate.hasMan
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
            text: name
        }

        Rectangle{
            id:manDelegate
            anchors.fill: parent
            anchors.margins: 10
            radius: width
            visible: cellDelegate.hasMan
            color: manColor
            MouseArea{
                anchors.fill: parent
                enabled: cellDelegate.hasMan
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
