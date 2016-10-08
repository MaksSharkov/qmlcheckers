#ifndef PERSONALMESSAGE_H
#define PERSONALMESSAGE_H

#include <QObject>
#include <QJsonObject>

#include "message.h"
#include "../types.h"

namespace HoldemChat {

class PersonalMessage : public Message
{
    Q_OBJECT
    Q_PROPERTY(TalkerPublicInfo receiver READ receiver)
public:
    static const QString type;

    explicit PersonalMessage(const TalkerPublicInfo sender, const QString messageText,TalkerPublicInfo receiver, QObject *parent = 0);
    explicit PersonalMessage(const QJsonObject &json,QObject *parent=0);

    TalkerPublicInfo receiver()const;

    QJsonObject toJson()const;
private:
    TalkerPublicInfo m_receiver;
};

}
#endif // PERSONALMESSAGE_H
