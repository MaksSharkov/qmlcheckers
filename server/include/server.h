#ifndef SERVER_H
#define SERVER_H

#include <QMap>
#include <QString>

#include "chat/conferencemessage.h"
#include "chat/personalmessage.h"
#include "userdatamanager.h"
#include "room.h"

#include <QWebSocketServer>
#include <QWebSocket>

class Server : public QWebSocketServer
{
    Q_OBJECT

public:
    Server(QHostAddress listenAddress=QHostAddress::LocalHost, quint16 port=8080);

signals:

    void replyReceived(QWebSocket* client ,QJsonObject reply,QString username);
    void clientAdded(QWebSocket* client,QString username);
    void clientDisconnected(QWebSocket* client,QString username);
public slots:  
    void disconnectClient(QWebSocket* client,QString reason);
    void disconnectClient();

    void sendReply(QWebSocket* client, const QJsonObject &reply);

private slots:
    void acceptConnection();
    void acceptRequest(const QString &request);
    void addClient(QWebSocket* client,const QString username);
private:
    QMap<QString,QWebSocket*> m_clients;
    UserDataManager m_manager;
    Room m_mainRoom;
};


#endif // SERVER_H
