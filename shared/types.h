#ifndef TYPES_H
#define TYPES_H

#include <QtGlobal>

#include <QJsonObject>

namespace HoldemChat {

typedef struct{
    QString name;
} ConferencePublicInfo;

typedef struct{
    QString name;
    QString status;
}TalkerPublicInfo;

QJsonObject toJson(const TalkerPublicInfo &info);

}

#endif // TYPES_H
