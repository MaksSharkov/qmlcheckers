#ifndef CONFERENCEMESSAGE_H
#define CONFERENCEMESSAGE_H

#include <QObject>
#include <QJsonObject>

#include "message.h"
#include "types.h"

namespace HoldemChat {

class ConferenceMessage : public Message
{
    Q_OBJECT
    Q_PROPERTY(ConferencePublicInfo conference READ conference)

public:
    static const QString type;

    explicit ConferenceMessage(const TalkerPublicInfo &sender,const QString &messageText,
                      const ConferencePublicInfo &inWhichConference,QObject *parent = 0);

    explicit ConferenceMessage(const QJsonObject &json,QObject *parent=0);

    ConferencePublicInfo conference()const;

    QJsonObject toJson()const;

private:
    ConferencePublicInfo m_conferenceInfo;
};

}
#endif // CONFERENCEMESSAGE_H
