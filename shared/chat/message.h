#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QJsonObject>
#include <QJsonParseError>
#include <QTextStream>
#include <QDataStream>
#include "../types.h"

namespace HoldemChat {

class Message : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text)
    Q_PROPERTY(TalkerPublicInfo sender READ sender)
public:
    explicit Message(const TalkerPublicInfo sender, const QString messageText, QObject *parent = 0);
    explicit Message(const QJsonObject &json,QObject *parent=0);

    static const QString type;
    TalkerPublicInfo sender();
    QString text();

    virtual QJsonObject toJson()const;
signals:

public slots:

private:
    TalkerPublicInfo m_sender;
    QString m_text;

};

QTextStream& operator<<(QTextStream &out, const Message &message);
QDataStream& operator<<(QDataStream &out, const Message &message);

}
#endif // MESSAGE_H
