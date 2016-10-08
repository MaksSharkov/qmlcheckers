#include "chessboard.h"
#include "assert.h"

#include <QDebug>

ChessBoard::ChessBoard(int boardSize, QObject *parent)
    : QObject(parent)
    , m_boardSize(boardSize)
{
    int col;
    for(int row=0;row<boardSize;row++)
        for(col=0;col<boardSize;col++){
            m_board.push_back(Cell(row,col,this));
        }
}

bool ChessBoard::isMoveCorrect(Cell &from, Cell &to) const
{
    return true;//TODO: implement
}

Cell* ChessBoard::cellAt(int row, int col)
{
    assert(row<m_boardSize);
    assert(col<m_boardSize);
    return &(m_board[row*m_boardSize+col]);
}

void ChessBoard::addMan(int row,int col,QString rank,QString player)
{
    Cell *cell=cellAt(row,col);
    cell->putMan(rank,player);
}

void ChessBoard::moveMan(Cell &from, Cell &to)
{
    moveMan(from.row(),from.col(),to.row(),to.col());
}

void ChessBoard::moveMan(int rowFrom, int colFrom, int rowTo, int colTo)
{
    Cell *from=cellAt(rowFrom,colFrom);
    Cell *to=cellAt(rowTo,colTo);

    if(isMoveCorrect(*from,*to))
    {
        from->swapMans(*to);
        emit manMoved(*from,*to);
    }
}

void ChessBoard::clear()
{
    for(int index=0;index<m_board.size();index++)
        m_board[index].purgeMan();
}

QJsonArray ChessBoard::toJson()
{
    QJsonArray result;
    foreach(Cell cell,m_board)
        result.append(cell.toJson());

    return result;
}
