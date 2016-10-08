#include "message.h"

#include <QJsonDocument>
#include <QDebug>
namespace HoldemChat {

const QString Message::type=QString("");

Message::Message(const TalkerPublicInfo sender,const QString messageText,QObject *parent)
    : QObject(parent),m_sender(sender),m_text(messageText)
{

}

Message::Message(const QJsonObject &json,QObject *parent)
    : QObject(parent)
{
    m_sender.name=json["sender"].toString();
    m_text=json["text"].toString();
}

QString Message::text()
{
    return m_text;
}

TalkerPublicInfo Message::sender()
{
    return m_sender;
}

QJsonObject Message::toJson()const
{
    QJsonObject result;
    result["type"]="";//Should be overriden in child classes
    result["sender"]=m_sender.name;
    result["text"]=m_text;

    return result;
}

QTextStream& operator<<(QTextStream &out, const Message &message)
{
    QJsonDocument doc(message.toJson());
    out << doc.toJson(QJsonDocument::Indented);
    return out;
}

QDataStream& operator<<(QDataStream &out, const Message &message)
{
    QJsonDocument doc(message.toJson());
    out << doc.toBinaryData();
    return out;
}

}
