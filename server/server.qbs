import qbs

Product {
    type: "application"
    name: "server"
    consoleApplication: true
    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: ["core", "websockets"] }

    Depends{ name: "shared"}

    files: [
        "checkersgame.cpp",
        "checkersgame.h",
        "chessboard.cpp",
        "chessboard.h",
        "loginmanager.cpp",
        "loginmanager.h",
        "main.cpp",
        "room.cpp",
        "room.h",
        "server.cpp",
        "server.h",
    ]
}
