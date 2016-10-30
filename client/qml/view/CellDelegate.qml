import QtQuick 2.7
import QtQuick.Controls 2.0
import "qrc:/settings.js" as Settings
import Checkers 1.0
import "../logic"

Rectangle{
    id: cellDelegate
    property Cell cell

    property string cellColor: cell.isBlack ? Settings.blackCellColor : Settings.whiteCellColor

    signal manSelected
    signal cellSelected

    MouseArea{
        id:area
        anchors.fill: parent
        enabled: cell.isEmpty
        onClicked: cellSelected()
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
            onClicked: manSelected()
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
                NumberAnimation{properties:"scale,color" ;duration:600}
            }
        ]
    }
}
