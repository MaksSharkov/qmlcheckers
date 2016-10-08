#include "card.h"

using namespace HoldemCards;

const QVector<Card::Suit> allSuits={
    Card::Suit::Spades,
    Card::Suit::Hearts,
    Card::Suit::Diamonds,
    Card::Suit::Clubs
    };
const QVector<Card::Value> allValues={
    Card::Value::Two,
    Card::Value::Three,
    Card::Value::Four,
    Card::Value::Five,
    Card::Value::Six,
    Card::Value::Seven,
    Card::Value::Eight,
    Card::Value::Nine,
    Card::Value::Ten,
    Card::Value::Jack,
    Card::Value::Queen,
    Card::Value::King,
    Card::Value::Ace
    };

Card::Card(const Suit suit,const Value value,QObject *parent)
    : QObject(parent),m_suit(suit),m_value(value)
{

}

Card::Card(const Card &card)
    :m_suit(card.m_suit),m_value(card.m_value),QObject(card.parent())
{

}

Card& Card::operator=(Card &other)
{
    m_suit=other.m_suit;
    m_value=other.m_value;
    return  other;
}

Card::Value Card::value() const
{
    return m_value;
}

Card::Suit Card::suit()const
{
    return m_suit;
}
