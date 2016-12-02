#include "types.h"

namespace HoldemChat {

QJsonObject toJson(const TalkerPublicInfo &info)
{
    QJsonObject result;
    result["type"]="publicInfo";
    result["username"]=info.name;
    result["status"]=info.status;
    return result;
}

}
