#ifndef ROOM_H
#define ROOM_H

#include <QObject>
#include <QMap>
#include <QWebSocket>
#include <QtGlobal>
#include <QJsonObject>

#include "checkersgame.h"

class Room : public QObject
{
    Q_OBJECT
public:
    explicit Room(QObject *parent = 0);

signals:
    void sendReply(QWebSocket* client,QJsonObject reply);

    void clientAdded(QWebSocket* client,QString username);
    void clientRemoved(QWebSocket* client,QString username);
public slots:

    void onReplyReceived(QWebSocket *client, QJsonObject reply);
    void notifyAbout(const QJsonObject &message);

    void addClient(QWebSocket* client,QString username);
    void removeClient(QWebSocket* client,QString username);
private:
    QMap<QString,QWebSocket*> m_clients;
    CheckersGame m_game;
};

#endif // ROOM_H
