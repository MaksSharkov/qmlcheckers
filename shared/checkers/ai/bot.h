#ifndef BOT_H
#define BOT_H

#include <QString>
#include <QPair>

#include <checkers/chessboard.h>
#include <checkers/cell.h>


#include <QVector>
#include "move.h"

class BotUtils{

public:
    static QPair<Cell,Cell> getMove(const QString &player,const ChessBoard &board);
    static QPair<Cell,Cell> getEatMove(const Cell &cell,const ChessBoard &board);
private:
    static QVector<Move> mapToVector(const Cell from,const QMap<Cell,bool> moves,const bool eatMovesOnly=false);

    static QVector<Move> getAllPlayersMoves(const QString &player, const ChessBoard &board);
};

#endif // BOT_H
