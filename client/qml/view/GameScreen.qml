import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.4

import "../logic"

SplitView {
    id:root
    property Client client

    Board{
        id:board
        client:root.client
        width: (parent.width )< parent.height ?
                   (parent.width ) : parent.height
        height:width
        smooth: true

        onGameEnded: endGame(winner)

        function endGame(winner){
            var message

            if(role !== "spectator")
                message = winner === client.name ? "You won." : "You lost."
            else
                message = winner+" won."

            chatArea.addMessage(message)
        }
    }

    Chat{
        id:chatArea
        client:root.client
        anchors{
            top:parent.top
            bottom:parent.bottom
            left: board.right
            right:parent.right
        }
    }
}
