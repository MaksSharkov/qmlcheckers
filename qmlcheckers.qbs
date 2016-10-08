import qbs 1.0

Project {
    minimumQbsVersion: 1.6

    references: [
        "client/client.qbs",
        "server/server.qbs",
        "shared/shared.qbs"
    ]
}

