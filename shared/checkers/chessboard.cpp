#include "chessboard.h"
#include "assert.h"

#include <QDebug>
#include <QMutexLocker>

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

QVector<Cell> ChessBoard::getPlayersCells(const QString player) const
{
    QVector<Cell> result;
    foreach(const Cell& cell,m_board)
        if(cell.belongsTo(player))
            result.append(cell);

    return result;
}

bool ChessBoard::mustEat(const QString player)const
{
    bool result=false;
    QVector<Cell> cells=getPlayersCells(player);
    foreach(const Cell cell,cells){
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
            }else if(to.containsKing()){
                int dRow= to.row() > from.row() ? 1 : -1;
                int dCol= to.col() > from.col() ? 1 : -1;
                int col=from.col()+dCol;
                for(int row=from.row()+dRow;row!=to.row();row+=dRow){
                    m_board[indexOf(row,col)].purgeMan();
                    col+=dCol;
                }
            }

            //Handle multy-cell moves
            mustEatFurther = getAvailableMoves(to).values().contains(true);
            m_continiousMoveCell = mustEatFurther ? &to : nullptr;
        }

        //Handle man --> king conversion
        if((player=="bottomPlayer" && to.row() == m_boardSize-1)
                ||(player=="topPlayer" && to.row() == 0)){
            QJsonObject man=to.man();
            man["rank"]="king";
            to.setMan(man);
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

Cell const& ChessBoard::getTopLeft(const Cell &from,const Cell &nullValue) const
{
    if(!(isOnTopBorder(from) || isOnLeftBorder(from)))
        return m_board[indexOf(from.row()+1,from.col()-1)];
    else
        return nullValue;
}

Cell const& ChessBoard::getBottomLeft(const Cell &from,const Cell &nullValue) const
{
    if(!(isOnBottomBorder(from) || isOnLeftBorder(from)))
        return m_board[indexOf(from.row()-1,from.col()-1)];
    else
        return nullValue;
}

Cell const& ChessBoard::getTopRight(const Cell &from,const Cell &nullValue) const
{
    if(!(isOnTopBorder(from) || isOnRightBorder(from)))
        return m_board[indexOf(from.row()+1,from.col()+1)];
    else
        return nullValue;
}

Cell const& ChessBoard::getBottomRight(const Cell &from,const Cell &nullValue) const
{
    if(!(isOnBottomBorder(from) || isOnRightBorder(from)))
        return m_board[indexOf(from.row()-1,from.col()+1)];
    else
        return nullValue;
}

QMap<Cell,bool> ChessBoard::getAvailableMoves(const Cell &from)const{
    if(from.containsMan())
        return getAvaibleMovesForMan(from);
    else if(from.containsKing())
        return getAvaibleMovesForKing(from);
    else
        return QMap<Cell,bool>();
}

Cell const & ChessBoard::getDiagonalMove(const Cell &from, const Cell &nullValue, const QString allowedToMovePlayer
                                         ,const std::function<Cell const&(const ChessBoard*, const Cell &,const Cell &)> &diagonalGetter) const
{
    const QString player=from.man()["whoose"].toString();

    const Cell &diagonalCell=diagonalGetter(this,from,nullValue);
    if(diagonalCell != nullValue){
        if(diagonalCell.isEmpty()
                && (allowedToMovePlayer.isEmpty() || (player==allowedToMovePlayer))){
            return diagonalCell;
        }else{
            if(!diagonalCell.belongsTo(player)&& !diagonalCell.isEmpty()){
                const Cell &diagonalCell2=diagonalGetter(this,diagonalCell,nullValue);
                if(diagonalCell2 != nullValue)
                    if(diagonalCell2.isEmpty()){
                        return diagonalCell2;
                    }
            }
        }
    }

    return nullValue;
}

QMap<Cell,bool> ChessBoard::getAvaibleMovesForMan(const Cell &from)const
{
    QMap<Cell,bool> result;
    const Cell& nullValue=m_board.at(1);

    Cell cell;
    if((cell = getDiagonalMove(from,nullValue,"bottomPlayer",&ChessBoard::getTopLeft))
            !=nullValue){
        result.insert(cell,!cell.isNear(from));
    }
    if((cell = getDiagonalMove(from,nullValue,"bottomPlayer",&ChessBoard::getTopRight))
            !=nullValue){
        result.insert(cell,!cell.isNear(from));
    }
    if((cell = getDiagonalMove(from,nullValue,"topPlayer",&ChessBoard::getBottomLeft))
            !=nullValue){
        result.insert(cell,!cell.isNear(from));
    }
    if((cell = getDiagonalMove(from,nullValue,"topPlayer",&ChessBoard::getBottomRight))
            !=nullValue){
        result.insert(cell,!cell.isNear(from));
    }

    return result;
}

void ChessBoard::getDiagonalMovesKing(const Cell &from, QMap<Cell,bool> &result
                                      ,const std::function<Cell const&(const ChessBoard*,const Cell&,const Cell&)>
                                      &diagonalGetter)const
{
    bool isEatMove=false;
    bool isNear = false;
    const Cell& nullValue=m_board.at(1);

    for(Cell current=from,diagonal=nullValue;current != nullValue
        ;current = diagonalGetter(this,current,nullValue)){

        diagonal=getDiagonalMove(current,nullValue,"",diagonalGetter);
        if(diagonal != nullValue){
            isNear = diagonal.isNear(current);
            if(!isEatMove){
                isEatMove=!isNear;
            }
            result.insert(diagonal,isEatMove);
            if(isNear)
                current = diagonalGetter(this,current,nullValue);
        }
    }
}

QMap<Cell,bool>  ChessBoard::getAvaibleMovesForKing(const Cell &from) const
{
    QMap<Cell,bool> result;

    getDiagonalMovesKing(from, result,&ChessBoard::getTopLeft);
    getDiagonalMovesKing(from, result,&ChessBoard::getTopRight);
    getDiagonalMovesKing(from, result,&ChessBoard::getBottomLeft);
    getDiagonalMovesKing(from, result,&ChessBoard::getBottomRight);

    return result;
}

bool ChessBoard::hasMoves(const QString player)
{
    bool result=false;
    QVector<Cell> cells=getPlayersCells(player);
    foreach(const Cell cell,cells){
            QMap<Cell,bool> availableMoves=getAvailableMoves(cell);
            if(!availableMoves.isEmpty()){
                result=true;
                break;
            }
        }

    return result;
}
