import qbs

CppApplication {
    name: "server"
    consoleApplication: true
    Depends { name: "Qt"; submodules: ["core", "websockets"] }

    Depends{ name: "shared"}

    cpp.includePaths: "include/"
    files: [
        "include/checkersgame.h",
        "include/loginmanager.h",
        "include/room.h",
        "include/server.h",
        "src/checkersgame.cpp",
        "src/loginmanager.cpp",
        "src/main.cpp",
        "src/room.cpp",
        "src/server.cpp",
    ]

}
