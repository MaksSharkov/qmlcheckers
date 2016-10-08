#ifndef DECK_H
#define DECK_H

#include <QObject>
#include <QList>

#include "card.h"

namespace HoldemCards {

class Deck : public QObject
{
    Q_OBJECT
public:
    explicit Deck(QObject *parent = 0);

signals:

public slots:
    void shuffle();

private:
    QList<Card> m_cards;
};

}

#endif // DECK_H
