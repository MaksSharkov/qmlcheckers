#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QObject>
#include <QVector>
#include <QJsonArray>

#include "checkers/cell.h"

class ChessBoard : public QObject
{
    Q_OBJECT
public:
    explicit ChessBoard(int boardSize, QObject *parent = 0);

    bool isMoveCorrect(Cell &from, Cell &to)const;
    int boardSize()const{
        return m_boardSize;
    }

    QJsonArray toJson();
signals:
    void manMoved(Cell &from,Cell &to);
public slots:
    void addMan(int row, int col, QString rank, QString player);

    void moveMan(Cell &from, Cell &to);
    void moveMan(int rowFrom, int colFrom, int rowTo, int colTo);

    void clear();
private:
    Cell *cellAt(int row,int col);
private:
    QVector<Cell> m_board;
    int m_boardSize;

};

#endif // CHESSBOARD_H
