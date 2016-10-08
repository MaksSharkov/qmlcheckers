#ifndef SERVER_H
#define SERVER_H

#include <QMap>
#include <QString>

#include "chat/conferencemessage.h"
#include "chat/personalmessage.h"
#include "loginmanager.h"
#include "room.h"

#include <QWebSocketServer>
#include <QWebSocket>

class Server : public QWebSocketServer
{
    Q_OBJECT

public:
    Server(QHostAddress listenAddress=QHostAddress::LocalHost, quint16 port=8080);

signals:

    void replyReceived(QWebSocket* client ,QJsonObject reply);
    void clientAdded(QWebSocket* client,QString username);
    void clientDisconnected(QWebSocket* client,QString username);
public slots:  
    void disconnectClient(QWebSocket* client);
    void disconnectClient();

    void sendReply(QWebSocket* client, const QJsonObject &reply);

private slots:
    void acceptConnection();
    void acceptRequest(const QString &request);
    void addClient(QWebSocket* client,const QString username);
private:
    QMap<QString,QWebSocket*> m_clients;
    LoginManager m_manager;
    Room m_mainRoom;
};


#endif // SERVER_H
