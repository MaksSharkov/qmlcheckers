#ifndef MOVE_H
#define MOVE_H

#include <QObject>
#include <QJsonObject>
#include "cell.h"

class Move
{
    Q_GADGET
    Q_PROPERTY(Cell from READ from WRITE setFrom NOTIFY fromChanged)
    Q_PROPERTY(Cell to READ to WRITE setTo NOTIFY toChanged)
    Q_PROPERTY(bool isEating READ isEating WRITE setIsEating NOTIFY isEatingChanged)

public:
    Move(const Cell &from=Cell(),const Cell &to=Cell(),bool isEatingMove=false);
    Move(const Move &other);

    Cell from()const{
        return m_from;
    }

    Cell to()const{
        return m_to;
    }

    bool isEating()const{
        return m_isEating;
    }

    Q_INVOKABLE QJsonObject toJson(bool switchTurn)const;
signals:
    void fromChanged(Cell);
    void toChanged(Cell);
    void isEatingChanged(bool);
public slots:
    void setFrom(const Cell &from);
    void setTo(const Cell &to);
    void setIsEating(const bool isEating);

private:
    Cell m_from,m_to;
    bool m_isEating;

};

Q_DECLARE_METATYPE(Move)

#endif // MOVE_H
