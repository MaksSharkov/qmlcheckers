import qbs 1.0

Product {
    type: "staticlibrary"
    name: "shared"

    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: ["core", "websockets"] }

    cpp.includePaths: "."

    files: [
        "chat/conferencemessage.cpp",
        "chat/conferencemessage.h",
        "chat/message.cpp",
        "chat/message.h",
        "chat/personalmessage.cpp",
        "chat/personalmessage.h",
        "checkers/ai/bot.cpp",
        "checkers/ai/bot.h",
        "checkers/cell.cpp",
        "checkers/cell.h",
        "checkers/chessboard.cpp",
        "checkers/chessboard.h",
        "types.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [product.sourceDirectory]
    }
}
