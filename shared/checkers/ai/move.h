#ifndef MOVE_H
#define MOVE_H

#include <QObject>

#include "checkers/cell.h"

class Move
{
    Q_GADGET
    Q_PROPERTY(Cell from READ from WRITE setFrom )
    Q_PROPERTY(Cell to READ to WRITE setTo)
    Q_PROPERTY(bool isEating READ isEating WRITE setIsEating)

public:
    Move(const Cell from=Cell(),const Cell to=Cell(),bool isEating=false);

    Q_INVOKABLE Cell from()const;
    Q_INVOKABLE Cell to() const;
    Q_INVOKABLE bool isEating()const;

    bool operator ==(const Move &other)const;
    bool operator !=(const Move &other)const;

    QString toString()const;
    QPair<Cell,Cell> toPair()const;

public slots:
    void setFrom(const Cell from);
    void setTo(const Cell to);
    void setIsEating(const bool isEating);

private:
    Cell m_from;
    Cell m_to;
    bool m_isEating;
};

Q_DECLARE_METATYPE(Move)

#endif // MOVE_H
