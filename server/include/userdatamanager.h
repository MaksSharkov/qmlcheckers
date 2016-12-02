#ifndef USERDATAMANAGER_H
#define USERDATAMANAGER_H

#include <QObject>
#include <QString>
#include <QWebSocket>
#include <QMap>
#include <QtSql>
#include <QJsonObject>

#include "types.h"

class UserDataManager : public QObject
{
    Q_OBJECT
public:
    explicit UserDataManager(const QMap<QString,QWebSocket*> *clients,QObject *parent = 0);

    bool usernameExists(const QString username);
    bool passwordValid(const QString username,const QString password);

    HoldemChat::TalkerPublicInfo getPublicInfo(const QString username);

signals:
    void logined(QWebSocket* client,const QString username);
    void loginDenied(QWebSocket* client,const QString reason);

    void sendReply(QWebSocket* client,QJsonObject reply);

public slots:

    void onReplyReceived(QWebSocket *client,QJsonObject reply);

private slots:
    void acceptLogin(QWebSocket* client, QString username);
    void denyLogin(QWebSocket* client, const QString &reason);

private:
    QSqlDatabase m_database;
    const QMap<QString,QWebSocket*> *m_clients;

    QString getStatus(const QString username);
    QString getPassword(const QString username);
    QString getFirstResult(QSqlQuery query);
};

#endif // USERDATAMANAGER_H
