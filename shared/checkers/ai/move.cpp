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
