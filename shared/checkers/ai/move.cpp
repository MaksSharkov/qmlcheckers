#include "move.h"

Move::Move(const Cell from, const Cell to, bool isEating)
    : m_from(from)
    , m_to(to)
    , m_isEating(isEating)
{

}

Cell Move::from()const
{
    return m_from;
}

Cell Move::to()const
{
    return m_to;
}

bool Move::isEating() const
{
    return m_isEating;
}

void Move::setFrom(const Cell from)
{
    if(m_from != from){
        m_from=from;
    }
}

void Move::setTo(const Cell to)
{
    if(m_to != to){
        m_to=to;
    }
}

void Move::setIsEating(const bool isEating)
{
    if(m_isEating != isEating){
        m_isEating=isEating;
    }
}

bool Move::operator ==(const Move &other)const
{
    return (m_from == other.m_from) && (m_to == other.m_to) && (m_isEating == other.isEating());
}

bool Move::operator !=(const Move &other)const
{
    return !operator ==(other);
}

QString Move::toString() const
{
    return QString("[%1-%2]").arg(m_from.name(),m_to.name());
}

QPair<Cell,Cell> Move::toPair() const
{
    QPair<Cell,Cell> output;
    output.first = from();
    output.second = to();
    return output;
}
