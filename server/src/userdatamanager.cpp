#include "userdatamanager.h"

UserDataManager::UserDataManager(const QMap<QString,QWebSocket*> *clients,QObject *parent)
    : QObject(parent),m_database(),m_clients(clients)
{
    m_database=QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName("data.sqlite");
    if (!m_database.open()) {
        qDebug()<<"Unable to open login database."<<m_database.lastError().text();
        QCoreApplication::quit();
    }
}

QString UserDataManager::getFirstResult(QSqlQuery query)
{
    query.exec();

    QString result=QString();
    if(query.next()){
        result = query.value(0).toString();
    }

    return result;
}

QString UserDataManager::getPassword(const QString username)
{
    QString queryString="SELECT password FROM logins WHERE username = '"+username+"'";
    QSqlQuery query(queryString,m_database);
    return getFirstResult(query);

}

bool UserDataManager::passwordValid(const QString username, const QString password)
{
    QString userPassword = getPassword(username);
    return userPassword == password;
}

bool UserDataManager::usernameExists(const QString username)
{
    QString userPassword  = getPassword(username);
    return ! userPassword.isEmpty();

}

void UserDataManager::acceptLogin(QWebSocket *client,QString username)
{
    qDebug()<<"Accepted client's login. username="<<username<<endl;
    QJsonObject reply;
    reply["type"]="loginReply";
    reply["verdict"]="true";
    emit sendReply(client,reply);
    emit logined(client,username);
}

void UserDataManager::denyLogin(QWebSocket *client, const QString &reason)
{
    QJsonObject reply;
    reply["type"]="loginReply";
    reply["verdict"]="false";
    reply["reason"]=reason;

    emit sendReply(client,reply);
    emit loginDenied(client,reason);

}

void UserDataManager::onReplyReceived(QWebSocket *client, QJsonObject reply)
{
    if(reply["type"] == "loginRequest"){
        QString username=reply["login"].toString();
        QString password=reply["password"].toString();
        if(!passwordValid(username,password))
            denyLogin(client,"Invalid username and/or password.");

        else if(m_clients->contains(username)){
            denyLogin(client,"You are already logined.");
        }
        else{
            acceptLogin(client,username);
        }
    }else if(reply["type"] == "publicInfoRequest"){
        QString username=reply["username"].toString();
        if(usernameExists(username)){
            QJsonObject reply = HoldemChat::toJson(getPublicInfo(username));
            emit sendReply(client,reply);
        }
    }
}

QString UserDataManager::getStatus(const QString username)
{
    QString queryString="SELECT status FROM statuses WHERE username = '"+username+"'";
    QSqlQuery query(queryString,m_database);
    return getFirstResult(query);
}

HoldemChat::TalkerPublicInfo UserDataManager::getPublicInfo(const QString username)
{
    HoldemChat::TalkerPublicInfo result;
    result.name = username;
    result.status = getStatus(username);
    return result;
}
