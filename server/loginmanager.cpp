#include "loginmanager.h"

LoginManager::LoginManager(const QMap<QString,QWebSocket*> *clients,QObject *parent)
    : QObject(parent),m_database(),m_clients(clients)
{
    m_database.insert("user1","user1");
    m_database.insert("user2","user2");
}

bool LoginManager::passwordValid(const QString username, const QString password)
{
    return m_database.value(username) == password;
}

bool LoginManager::usernameExists(const QString username)
{
    return m_database.contains(username);
}

void LoginManager::acceptLogin(QWebSocket *client,QString username)
{
    qDebug()<<"Accepted client's login. username="<<username<<endl;
    QJsonObject reply;
    reply["type"]="loginReply";
    reply["verdict"]="true";
    emit sendReply(client,reply);
    emit logined(client,username);
}

void LoginManager::denyLogin(QWebSocket *client, const QString &reason)
{
    QJsonObject reply;
    reply["type"]="loginReply";
    reply["verdict"]="false";
    reply["reason"]=reason;

    emit sendReply(client,reply);
    emit loginDenied(client,reason);
}

void LoginManager::onReplyReceived(QWebSocket *client, QJsonObject reply)
{
    if(reply["type"] == "loginRequest"){
        QString username=reply["login"].toString();
        QString password=reply["password"].toString();
        if(!usernameExists(username) || !passwordValid(username,password))
            denyLogin(client,"Invalid username and/or password.");

        else if(m_clients->contains(username)){
            denyLogin(client,"You are already logined.");
        }
        else{
            acceptLogin(client,username);
        }
    }
}
