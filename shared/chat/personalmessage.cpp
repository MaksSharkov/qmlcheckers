#include "personalmessage.h"

namespace HoldemChat{

const QString PersonalMessage::type=QString("pmMessage");

PersonalMessage::PersonalMessage(const TalkerPublicInfo sender, const QString messageText,TalkerPublicInfo receiver, QObject *parent)
    :Message(sender,messageText,parent),m_receiver(receiver)
{

}

PersonalMessage::PersonalMessage(const QJsonObject &json,QObject *parent)
    :Message(json,parent)
{
    m_receiver.name=json["receiver"].toString();
}

TalkerPublicInfo PersonalMessage::receiver() const
{
    return m_receiver;
}

QJsonObject PersonalMessage::toJson()const
{
    QJsonObject result= Message::toJson();
    result["type"]=type;
    result["receiver"]=m_receiver.name;

    return result;
}

}
