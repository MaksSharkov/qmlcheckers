#include "cell.h"

#include "assert.h"

Cell::Cell(int row, int col, QObject *parent)
    : QObject(parent)
    , m_row(row)
    , m_col(col)
{
    purgeMan();
}

Cell::Cell(const Cell &other)
    : Cell(other.row(),other.col(),other.parent())
{
    man["rank"] = other.man["rank"];
    man["whoose"] = other.man["whoose"];
}

void Cell::setRow(int row)
{
    if(m_row != row){
        m_row = row;
        emit rowChanged(m_row);
    }
}

void Cell::setCol(int col)
{
    if(m_col != col){
        m_col = col;
        emit colChanged(m_col);
    }
}

bool Cell::isBlack(){
    return isBlack(m_row,m_col);
}

bool Cell::isBlack(int row,int col)
{
    if (row % 2 == 0)
        return (col % 2) == 0 ;
    else
        return (col % 2) == 1 ;
}

bool Cell::isEmpty() const
{
    return man["rank"] == "none";
}

void Cell::putMan(QString rank, QString whoose)
{
    assert((rank=="none")||(rank=="man")||(rank=="king"));
    assert(whoose=="topPlayer"||(whoose=="bottomPlayer"));
    man["rank"] = rank;
    man["whoose"] = whoose;
}

void Cell::purgeMan()
{
    man["rank"] = "none";
    man["whoose"] = "";
}

bool Cell::belongsTo(QString player) const
{
    if(!isEmpty())
        return man["whoose"] == player;
    else
        return false;
}

void Cell::swapMans(Cell &with)
{
    qSwap(man,with.man);
}


Cell& Cell::operator=(const Cell &second)
{
    *this = Cell(second);
    return *this;
}

QJsonObject Cell::toJson()
{
    QJsonObject result;
    result["row"]=m_row;
    result["col"]=m_col;
    result["man"]=man;
    return result;
}
