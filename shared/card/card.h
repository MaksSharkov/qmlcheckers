#ifndef QCARD_H
#define QCARD_H

#include <QObject>
#include <QVector>

namespace HoldemCards {

class Card : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Suit suit READ suit)
    Q_PROPERTY(Value value READ value)
public:

    enum class Suit{
        Spades,
        Hearts,
        Diamonds,
        Clubs
    };

    enum class Value{
        Two,Three,Four,Five,Six,Seven,Eight,Nine,Ten,Jack,Queen,King,Ace
    };

    explicit Card(const Suit suit,const Value value,QObject *parent = 0);
    Card(const Card &card);

    const static QVector<Suit> allSuits;
    const static QVector<Value> allValues;

    Card& operator =(Card &other);

    Suit suit() const;
    Value value() const;
signals:

public slots:

private:
    Suit m_suit;
    Value m_value;
};

}

#endif // QCARD_H
