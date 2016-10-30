#include "move.h"

Move::Move(const Cell &from, const Cell &to, bool isEatingMove)
    : m_from(from)
    , m_to(to)
    , m_isEating(isEatingMove)
{
}

Move::Move(const Move &other)
    : Move(other.m_from,other.m_to,other.m_isEating)
{

}

void Move::setFrom(const Cell &from)
{
    if(m_from != from){
        m_from=from;
        emit fromChanged(m_from);
    }
}

void Move::setTo(const Cell &to)
{
    if(m_to != to){
        m_to=to;
        emit toChanged(m_to);
    }
}

void Move::setIsEating(const bool isEating)
{
    if(m_isEating != isEating){
        m_isEating=isEating;
        emit isEatingChanged(m_isEating);
    }
}

QJsonObject Move::toJson(bool switchTurn)const
{
    QJsonObject result;
    result["type"]="moveMan";
    result["from"]=m_from.toJson();
    result["to"]=m_to.toJson();
    result["switchTurn"]=switchTurn;
    return result;
}
