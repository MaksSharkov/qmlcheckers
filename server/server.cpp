#include "server.h"

#include <QNetworkInterface>
#include <QJsonDocument>
#include <QJsonObject>

#include <QDebug>

Server::Server(QHostAddress listenAddress,quint16 port)
    : QWebSocketServer("holdemServer",QWebSocketServer::SslMode::NonSecureMode)
    , m_clients()
    , m_manager(&m_clients,this)
    , m_mainRoom(this)
{
    if (!this->listen(listenAddress,port)) {
        qDebug() << QObject::tr("Unable to start the server: %1.").arg(this->errorString());
        this->close();
    }

    qDebug() << QObject::tr("Started to listen on adress %1").arg(this->serverAddress().toString());
    qDebug() << "Port: "<< this->serverPort();
    connect(this,SIGNAL(newConnection()),this,SLOT(acceptConnection()));

    connect(this,SIGNAL(replyReceived(QWebSocket*,QJsonObject)),&m_manager,SLOT(onReplyReceived(QWebSocket*,QJsonObject)));
    connect(&m_manager,SIGNAL(logined(QWebSocket*,QString)),this,SLOT(addClient(QWebSocket*,QString)));
    connect(&m_manager,SIGNAL(loginDenied(QWebSocket*,QString)),this,SLOT(disconnectClient(QWebSocket*)));
    connect(&m_manager,SIGNAL(sendReply(QWebSocket*,QJsonObject)),this,SLOT(sendReply(QWebSocket*,QJsonObject)));

    connect(this,SIGNAL(replyReceived(QWebSocket*,QJsonObject)),&m_mainRoom,SLOT(onReplyReceived(QWebSocket*,QJsonObject)));
    connect(&m_mainRoom,SIGNAL(sendReply(QWebSocket*,QJsonObject)),this,SLOT(sendReply(QWebSocket*,QJsonObject)));

    connect(this,SIGNAL(clientAdded(QWebSocket*,QString)),&m_mainRoom,SLOT(addClient(QWebSocket*,QString)));
    connect(this,SIGNAL(clientDisconnected(QWebSocket*,QString)),&m_mainRoom,SLOT(removeClient(QWebSocket*,QString)));
}

void Server::acceptConnection()
{
    QWebSocket *newClient = this->nextPendingConnection();

    connect(newClient,SIGNAL(textMessageReceived(const QString&)),this,SLOT(acceptRequest(const QString&)));
    connect(newClient,SIGNAL(disconnected()),this,SLOT(disconnectClient()));

    qDebug() << "AcceptedConnection"<<endl;
}

void Server::acceptRequest(const QString &request)
{
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    QJsonParseError error;
    QJsonDocument doc =  QJsonDocument::fromJson(request.toUtf8(),&error);
    if(error.error != QJsonParseError::NoError)
    {
        qDebug() << "Error while parsing received message : "<< error.errorString()<<endl;
        qDebug() << "rawReply= "<<request<<endl;
    }

    QJsonObject reply=doc.object();
    emit replyReceived(client,reply);
}

void Server::disconnectClient(QWebSocket* client)
{
    QString username=m_clients.key(client);
    if(m_clients.remove(username)){
        emit clientDisconnected(client,username);
    }
        qDebug()<<"Client disconnected.Username="<<username<<endl;
}

void Server::disconnectClient()
{
    disconnectClient(qobject_cast<QWebSocket*>(sender()));
}

void Server::sendReply(QWebSocket *client, const QJsonObject &reply)
{
    QJsonDocument doc(reply);
    client->sendTextMessage(doc.toJson(QJsonDocument::Compact));
}

void Server::addClient(QWebSocket *client, const QString username)
{
    qDebug()<<"Adding client to client database.username="<<username<<endl;
    m_clients.insert(username,client);
    emit clientAdded(client,username);
}
