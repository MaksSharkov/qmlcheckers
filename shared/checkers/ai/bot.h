#ifndef BOT_H
#define BOT_H

#include <QString>
#include <QPair>

#include <checkers/chessboard.h>
#include <checkers/cell.h>


#include <QVector>
#include "move.h"
#include "situation.h"

class BotUtils{
public:

    static const int analyzeDepth=3;
    static QPair<Cell,Cell> getMove(const QString &player,const CheckersBoard &board);
    static QPair<Cell,Cell> getMove(const Cell &cell,const CheckersBoard &board);

private:
    static QVector<CheckersMove> mapToVector(const Cell from,const QMap<Cell,bool> moves,const bool eatMovesOnly=false);

    static QVector<CheckersMove> getAllPlayersMoves(const QString &player, const CheckersSituation &situation);
    static CheckersMove getMoveFor(const Cell &from,const QString player,const CheckersBoard board);

    static bool needSwitchTurn(const MoveSequence sequence);
};

#endif // BOT_H
