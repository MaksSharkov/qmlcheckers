#include "chessboard.h"
#include "assert.h"

#include <QDebug>

ChessBoard::ChessBoard(int boardSize, QObject *parent)
    : QAbstractListModel(parent)
    , m_continiousMoveCell(nullptr)
{
    initializeWithEmpty(boardSize);
}

void ChessBoard::initializeWithEmpty(int boardSize)
{
    initRoles();
    setBoardSize(boardSize);

    clear();
    int col;
    for(int row=0;row<boardSize;row++)
        for(col=0;col<boardSize;col++){
            Cell newCell=Cell(row,col,this);
            append(newCell.toJson());
        }
}

bool ChessBoard::isMoveCorrect(const QString player,const Cell &from,const Cell &to) const
{
    if(!from.belongsTo(player)){
        qDebug()<<"Attempt to move man that doesn't belong to player."<<player<<"!="<<from.man()["whoose"].toString()<<endl;
        return false;
    }else if(!to.isEmpty()){
        qDebug()<<"Attempt to move man to non-empty cell"<<endl;
        return false;
    }else if(from.isEmpty()){
        qDebug()<<"Attempt to move man to empty cell"<<endl;
        return false;
    }else if(from.row() == to.row() && from.col() == to.col()){
        qDebug()<<"Attempt to move man to its current location"<<endl;
        return false;
    }else if(!to.isBlack()){
        qDebug()<<"Attempt to move man to non-black cell"<<endl;
        return false;
    }else if(m_continiousMoveCell != nullptr && m_continiousMoveCell != &from){
        qDebug()<<"Attempt to move separate man while player must eat further";
        return false;
    }else{
        QMap<Cell,bool> available=getAvailableMoves(from);
        if(available.contains(to)){
            if(mustEat(player)){
                if(available.value(to))
                    return true;
                else{
                    qDebug()<<"Attempted to move while player must eat."<<endl;
                    return false;
                }
            }else{
                return true;
            }
        }else{
            qDebug()<<"Attempt to move in not acceptable cell."<<endl;
            return false;
        }
    }
}

bool ChessBoard::mustEat(const QString player)const
{
    bool result=false;
    foreach(const Cell& cell,m_board)
        if(cell.belongsTo(player)){
            QMap<Cell,bool> availableMoves=getAvailableMoves(cell);
            if(availableMoves.values().contains(true)){
                result=true;
                break;
            }
        }
    return result;
}

bool ChessBoard::isMoveCorrect(const QString player,const QJsonObject &from,const QJsonObject &to)const
{
    const Cell& cellFrom=m_board.at(indexOf(from["row"].toInt(),from["col"].toInt()));
    const Cell& cellTo = m_board.at(indexOf(to["row"].toInt(),to["col"].toInt()));
    return isMoveCorrect(player,cellFrom,cellTo);
}

void ChessBoard::addMan(int row,int col,QString rank,QString player)
{
    m_board[indexOf(row,col)].putMan(rank,player);
}

void ChessBoard::moveMan(const QString player,int rowFrom, int colFrom, int rowTo, int colTo)
{
    Cell &from=m_board[indexOf(rowFrom,colFrom)];
    Cell &to=m_board[indexOf(rowTo,colTo)];

    if(isMoveCorrect(player,from,to))
    {
        from.swapMans(to);

        bool mustEatFurther = false;

        if(!from.isNear(to)){

            //Purge man,that has being eaten
            if(to.containsMan()){
                int purgedManRow=(from.row()+to.row())/2;
                int purgedManCol=(from.col()+to.col())/2;
                m_board[indexOf(purgedManRow,purgedManCol)].purgeMan();
            }

            //Handle multy-cell moves
            mustEatFurther = getAvailableMoves(to).values().contains(true);
            m_continiousMoveCell = mustEatFurther ? &to : nullptr;
        }

        qDebug()<<"MustEatFurther="<<mustEatFurther;
        emit manMoved(player,from,to,!mustEatFurther);
    }
}

void ChessBoard::clearFromMans()
{
    for(int index=0;index<m_board.size();index++)
        m_board[index].purgeMan();
}

QJsonArray ChessBoard::toJson() const
{
    QJsonArray result;
    foreach(Cell cell,m_board)
        result.append(cell.toJson());

    return result;
}

int ChessBoard::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_board.size();
}

QVariant ChessBoard::data(const QModelIndex &index, int role) const
{
    int cellIndex = index.row();
    if(cellIndex < 0 || cellIndex >= m_board.size()) {
        return QVariant();
    }

    switch (role) {
    case RoleNames::RowRole:
        return m_board[cellIndex].row();
    case RoleNames::ColRole:
        return m_board[cellIndex].col();
    case RoleNames::ManRole:
        return m_board[cellIndex].man();
    default:
        return QVariant();
    }


}

QHash<int, QByteArray> ChessBoard::roleNames() const
{
    return m_roleNames;
}

void ChessBoard::initRoles()
{
    m_roleNames[RoleNames::RowRole]="row";
    m_roleNames[RoleNames::ColRole]="col";
    m_roleNames[RoleNames::ManRole]="man";
}

int ChessBoard::indexOf(int row, int col)const
{
    assert(row>=0 && row<m_boardSize);
    assert(col>=0 && col<m_boardSize);
    return (row*m_boardSize+col);
}

void ChessBoard:: clear()
{
    if(!m_board.isEmpty()){
        emit beginRemoveRows(QModelIndex(), 0, m_board.size()-1);
        m_board.clear();
        setBoardSize(0);
        emit endRemoveRows();
    }
}

QJsonObject ChessBoard::get(int index)
{
    if(index < 0 || index >= m_board.size()) {
        return Cell().toJson();
    }

    return m_board.at(index).toJson();
}

void ChessBoard::handleManChanged()
{
    QVector<int> roles;
    roles.append(RoleNames::ManRole);

    Cell *changedCell=qobject_cast<Cell*>(sender());
    int intIndex = m_board.indexOf(*changedCell);
    QModelIndex index=createIndex(intIndex,intIndex);

    emit dataChanged(index,index,roles);
}

void ChessBoard::append(const QJsonObject &cell)
{
    int index=m_board.size();

    Cell newCell=Cell(cell["row"].toInt(),cell["col"].toInt(),this);
    newCell.setMan(cell["man"].toObject());

    emit beginInsertRows(QModelIndex(), index, index);
    m_board.push_back(newCell);
    connect(&(m_board.last()),SIGNAL(manChanged(QJsonObject))
            ,this,SLOT(handleManChanged()));
    emit endInsertRows();
}

void ChessBoard::initialize(QJsonArray board,int boardSize)
{
    clear();
    setBoardSize(boardSize);
    foreach (const QJsonValue & cell, board) {
        append(cell.toObject());
    }
}

void ChessBoard::moveMan(const QString player, QJsonObject from, QJsonObject to)
{
    moveMan(player,from["row"].toInt(),from["col"].toInt()
            ,to["row"].toInt(),to["col"].toInt());
}

void ChessBoard::setBoardSize(int boardSize)
{
    if(m_boardSize!= boardSize){
        m_boardSize=boardSize;
        emit boardSizeChanged(m_boardSize);
    }
}

bool ChessBoard::isOnTopBorder(const Cell &cell) const
{
    return cell.row() == (m_boardSize-1);
}

bool ChessBoard::isOnBottomBorder(const Cell &cell) const
{
    return cell.row() == 0;
}

bool ChessBoard::isOnLeftBorder(const Cell &cell) const
{
    return cell.col() == 0;
}

bool ChessBoard::isOnRightBorder(const Cell &cell) const
{
    return cell.col() == (m_boardSize-1);
}

Cell const& ChessBoard::getTopLeft(const Cell &from,const Cell &defaultValue) const
{
    if(!(isOnTopBorder(from) || isOnLeftBorder(from)))
        return m_board[indexOf(from.row()+1,from.col()-1)];
    else
        return defaultValue;
}

Cell const& ChessBoard::getBottomLeft(const Cell &from,const Cell &defaultValue) const
{
    if(!(isOnBottomBorder(from) || isOnLeftBorder(from)))
        return m_board[indexOf(from.row()-1,from.col()-1)];
    else
        return defaultValue;
}

Cell const& ChessBoard::getTopRight(const Cell &from,const Cell &defaultValue) const
{
    if(!(isOnTopBorder(from) || isOnRightBorder(from)))
        return m_board[indexOf(from.row()+1,from.col()+1)];
    else
        return defaultValue;
}

Cell const& ChessBoard::getBottomRight(const Cell &from,const Cell &defaultValue) const
{
    if(!(isOnBottomBorder(from) || isOnRightBorder(from)))
        return m_board[indexOf(from.row()-1,from.col()+1)];
    else
        return defaultValue;
}

QMap<Cell,bool> ChessBoard::getAvailableMoves(const Cell &from)const{
    if(from.containsMan())
        return getAvaibleMovesForMan(from);
    else
        return QMap<Cell,bool>();
}

QMap<Cell,bool> ChessBoard::getAvaibleMovesForMan(const Cell &from)const
{
    assert(!from.isEmpty());

    const QString player=from.man()["whoose"].toString();

    QMap<Cell,bool> result;
    const Cell& defaultValue=m_board.at(1);

    const Cell &topLeft=getTopLeft(from,defaultValue);
    if(topLeft != defaultValue){
        if(topLeft.isEmpty() && (player=="bottomPlayer")){
            result.insert(topLeft,false);
        }else{
            if(!topLeft.belongsTo(player)){
                const Cell &topLeft2=getTopLeft(topLeft,defaultValue);
                if(topLeft2 != defaultValue)
                    if(topLeft2.isEmpty())
                        result.insert(topLeft2,true);
            }
        }
    }

    const Cell &topRight=getTopRight(from,defaultValue);
    if(topRight != defaultValue){
        if(topRight.isEmpty() && (player=="bottomPlayer") ){
            result.insert(topRight,false);
        }else{
            if(!topRight.belongsTo(player)){
                const Cell &topRight2=getTopRight(topRight,defaultValue);
                if(topRight2 != defaultValue)
                    if(topRight2.isEmpty())
                        result.insert(topRight2,true);
            }
        }
    }

    const Cell &bottomLeft=getBottomLeft(from,defaultValue);
    if(bottomLeft != defaultValue){
        if(bottomLeft.isEmpty() && (player=="topPlayer")){
            result.insert(bottomLeft,false);
        }else{
            if(!bottomLeft.belongsTo(player)){
                const Cell &bottomLeft2=getBottomLeft(bottomLeft,defaultValue);
                if(bottomLeft2 != defaultValue)
                    if(bottomLeft2.isEmpty())
                        result.insert(bottomLeft2,true);
            }
        }
    }

    const Cell &bottomRight=getBottomRight(from,defaultValue);
    if(bottomRight != defaultValue){
        if(bottomRight.isEmpty() && (player=="topPlayer")){
            result.insert(bottomRight,false);
        }else{
            if(!bottomRight.belongsTo(player)){
                const Cell &bottomRight2=getBottomRight(bottomRight,defaultValue);
                if(bottomRight2 != defaultValue)
                    if(bottomRight2.isEmpty())
                        result.insert(bottomRight2,true);
            }
        }
    }

    return result;
}
