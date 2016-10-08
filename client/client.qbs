import qbs

Product {
    type: "application"
    name: "client"
    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: ["qml", "quick"] }

    Depends { name: "shared" }

    files:[
        "main.cpp",
        "qml.qrc",
        "qml/logic/Chatter.qml",
        "qml/logic/ChessModel.qml",
        "qml/logic/Client.qml",
        "qml/logic/Loginer.qml",
        "qml/logic/Player.qml",
        "qml/view/Board.qml",
        "qml/view/Chat.qml",
        "qml/view/GameScreen.qml",
        "qml/view/LoginPage.qml",
        "qml/view/main.qml",
        "settings.js",
    ]
}
