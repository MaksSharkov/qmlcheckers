#include "move.h"

CheckersMove::CheckersMove(const Cell from, const Cell to, bool isEating,bool mustEatFuther)
    : m_from(from)
    , m_to(to)
    , m_isEating(isEating)
    , m_mustEatFuther(mustEatFuther)
{

}

Cell CheckersMove::from()const
{
    return m_from;
}

Cell CheckersMove::to()const
{
    return m_to;
}

bool CheckersMove::isEating() const
{
    return m_isEating;
}

void CheckersMove::setFrom(const Cell from)
{
    if(m_from != from){
        m_from=from;
    }
}

void CheckersMove::setTo(const Cell to)
{
    if(m_to != to){
        m_to=to;
    }
}

void CheckersMove::setIsEating(const bool isEating)
{
    if(m_isEating != isEating){
        m_isEating=isEating;
    }
}

bool CheckersMove::operator ==(const CheckersMove &other)const
{
    return (m_from == other.m_from) && (m_to == other.m_to) && (m_isEating == other.isEating());
}

bool CheckersMove::operator !=(const CheckersMove &other)const
{
    return !operator ==(other);
}

QString CheckersMove::toString() const
{
    return QString("[%1-%2]").arg(m_from.name(),m_to.name());
}

QPair<Cell,Cell> CheckersMove::toPair() const
{
    QPair<Cell,Cell> output;
    output.first = from();
    output.second = to();
    return output;
}

bool CheckersMove::operator <(const CheckersMove &other)const
{
    return m_from < other.m_from;
}

bool CheckersMove::mustEatFuther()const
{
    return m_mustEatFuther;
}

void CheckersMove::setMustReadFuther(const bool mustEatFuther)
{
    m_mustEatFuther = mustEatFuther;
}

QString toString(const MoveSequence sequence)
{
    QString result;
    foreach(const CheckersMove move,sequence)
        result +=move.toString();

    return result;
}
