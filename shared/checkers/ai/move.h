#ifndef MOVE_H
#define MOVE_H

#include <QObject>

#include "checkers/cell.h"

#include <QVector>


class CheckersMove
{
    Q_GADGET
    Q_PROPERTY(Cell from READ from WRITE setFrom )
    Q_PROPERTY(Cell to READ to WRITE setTo)
    Q_PROPERTY(bool isEating READ isEating WRITE setIsEating)
    Q_PROPERTY(bool mustEatFuther  READ mustEatFuther WRITE setMustReadFuther);
public:
    CheckersMove(const Cell from=Cell(), const Cell to=Cell(), bool isEating=false, bool mustEatFuther=false);

    Q_INVOKABLE Cell from()const;
    Q_INVOKABLE Cell to() const;
    Q_INVOKABLE bool isEating()const;
    Q_INVOKABLE bool mustEatFuther()const;

    bool operator ==(const CheckersMove &other)const;
    bool operator !=(const CheckersMove &other)const;
    bool operator <(const CheckersMove &other)const;

    QString toString()const;
    QPair<Cell,Cell> toPair()const;

public slots:
    void setFrom(const Cell from);
    void setTo(const Cell to);
    void setIsEating(const bool isEating);
    void setMustReadFuther(const bool mustEatFuther);

private:
    Cell m_from;
    Cell m_to;
    bool m_isEating;
    bool m_mustEatFuther;
};

Q_DECLARE_METATYPE(CheckersMove)

typedef QVector<CheckersMove> MoveVariants;
typedef QVector<CheckersMove> MoveSequence;
QString toString(const MoveSequence sequence);

#endif // MOVE_H
