#include "conferencemessage.h"

namespace HoldemChat {

const QString ConferenceMessage::type=QString("confMessage");

ConferenceMessage::ConferenceMessage(const TalkerPublicInfo &sender, const QString &messageText,
                                        const ConferencePublicInfo &inWhichConference, QObject *parent)
    : Message(sender,messageText,parent),m_conferenceInfo(inWhichConference)
{

}

ConferenceMessage::ConferenceMessage(const QJsonObject &json, QObject *parent)
    :Message(json,parent)
{
    m_conferenceInfo.name=json["conference"].toString();
}

ConferencePublicInfo ConferenceMessage::conference()const
{
    return m_conferenceInfo;
}

QJsonObject ConferenceMessage::toJson()const
{
    QJsonObject result = Message::toJson();
    result["type"]=type;
    result["conference"]=m_conferenceInfo.name;
    return result;
}

}
