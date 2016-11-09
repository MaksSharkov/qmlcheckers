#ifndef BOT_H
#define BOT_H

#include <QString>
#include <QPair>

#include <checkers/chessboard.h>
#include <checkers/cell.h>


class BotUtils{

public:
    static QPair<Cell,Cell> getMove(const QString &player,const ChessBoard &board);
    static QPair<Cell,Cell> getMove(const Cell &cell,const ChessBoard &board);
private:

};

#endif // BOT_H
