#include "room.h"

#include "chat/conferencemessage.h"

Room::Room(QObject *parent)
    : QObject(parent)
    , m_clients()
    , m_game(this)
{
    connect(&m_game,SIGNAL(notifyAbout(QJsonObject))
            ,this,SLOT(notifyAbout(QJsonObject)));

    connect(this,SIGNAL(clientAdded(QWebSocket*,QString)),&m_game,SLOT(onClientAdded(QWebSocket*,QString)));
    connect(this,SIGNAL(clientRemoved(QWebSocket*,QString)),&m_game,SLOT(onClientRemoved(QWebSocket*,QString)));
}

void Room::notifyAbout(const QJsonObject &message)
{
    foreach (QWebSocket *client, m_clients) {
        emit sendReply(client,message);
    }
}

void Room::onReplyReceived(QWebSocket *client, QJsonObject reply, QString username)
{
    Q_UNUSED(client)
    if(reply["type"] == HoldemChat::ConferenceMessage::type){
        HoldemChat::ConferenceMessage message(reply);
        qDebug() <<QObject::tr("Received message from %1 in conference %2. Text: %3")
                    .arg(message.sender().name,message.conference().name,message.text())<<endl;
        notifyAbout(reply);
    }
    m_game.onReplyReceived(client,reply,username);
}

void Room::addClient(QWebSocket *client, QString username)
{
    m_clients.insert(username,client);
    emit clientAdded(client,username);
}

void Room::removeClient(QWebSocket *client, QString username)
{
    if(m_clients.remove(username))
        emit clientRemoved(client,username);
}
