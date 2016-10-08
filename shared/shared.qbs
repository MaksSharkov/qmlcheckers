import qbs 1.0

Product {
    type: "staticlibrary"
    name: "shared"

    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: ["core", "websockets"] }

    files: [
        "chat/conferencemessage.cpp",
        "chat/conferencemessage.h",
        "chat/message.cpp",
        "chat/message.h",
        "chat/personalmessage.cpp",
        "chat/personalmessage.h",
        "checkers/cell.cpp",
        "checkers/cell.h",
        "types.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [product.sourceDirectory]
    }
}
