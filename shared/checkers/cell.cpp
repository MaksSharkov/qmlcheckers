#include "cell.h"

#include "assert.h"
#include "math.h"

Cell::Cell(int row, int col, QObject *parent)
    : QObject(parent)
    , m_row(row)
    , m_col(col)
{
    purgeMan();
    connect(this,SIGNAL(rowChanged(uint)),this,SIGNAL(nameChanged()));
    connect(this,SIGNAL(colChanged(uint)),this,SIGNAL(nameChanged()));
    connect(this,SIGNAL(nameChanged()),this,SIGNAL(isBlackChanged()));
    connect(this,SIGNAL(manChanged(QJsonObject)),this,SIGNAL(isEmptyChanged()));
}

Cell::Cell(const Cell &other)
    : Cell(other.row(),other.col(),other.parent())
{
    setMan(other.man());
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

bool Cell::isBlack() const{
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
    return m_man["rank"] == "none";
}

void Cell::putMan(QString rank, QString whoose)
{
    assert((rank=="none")||(rank=="man")||(rank=="king"));
    assert(whoose==""||whoose=="topPlayer"||(whoose=="bottomPlayer"));
    if(m_man["rank"]!=rank || m_man["whoose"]!= whoose){
        m_man["rank"] = rank;
        m_man["whoose"] = whoose;
        emit manChanged(m_man);
    }
}

void Cell::purgeMan()
{
    if(!isEmpty()){
        m_man["rank"] = "none";
        m_man["whoose"] = "";
        emit manChanged(m_man);
    }
}

bool Cell::belongsTo(QString player) const
{
    if(!isEmpty())
        return m_man["whoose"] == player;
    else
        return false;
}

void Cell::swapMans(Cell &with)
{
    QJsonObject temp = man();
    setMan(with.man());
    with.setMan(temp);
}


Cell& Cell::operator=(const Cell &second)
{
    *this = Cell(second);
    return *this;
}

QJsonObject Cell::toJson() const
{
    QJsonObject result;
    result["row"]=m_row;
    result["col"]=m_col;
    result["man"]=m_man;
    return result;
}

QString Cell::name()const{
    QChar letter='a'+m_row;
    QString result=letter+QString::number(m_col);
    return result;
}

void Cell::setMan(const QJsonObject &man)
{
    putMan(man["rank"].toString(),man["whoose"].toString());
}

bool Cell::operator ==(const Cell &other)const
{
    return (m_col==other.m_col) && (m_row == other.m_row) && (m_man == other.m_man);
}

bool Cell::operator !=(const Cell &other)const
{
    return !operator==(other);
}

bool Cell::isNear(const Cell &other)const
{
    return (abs(other.m_row-m_row)==1)&&(abs(other.m_col-m_col)==1);
}
