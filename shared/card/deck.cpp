#include "deck.h"

namespace HoldemCards {

Deck::Deck(QObject *parent)
    : QObject(parent)
{
    m_cards.clear();
    foreach (Card::Suit suit, Card::allSuits)
        foreach (Card::Value value, Card::allValues) {
            m_cards.append(Card(suit,value));
        }
}

void Deck::shuffle()
{
    std::random_shuffle(m_cards.begin(),m_cards.end());
}

}
