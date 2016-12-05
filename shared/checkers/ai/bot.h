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
    typedef QVector<Move> MoveVariants;
    typedef QVector<Move> MoveSequence;

    static const int analyzeDepth=3;
    static QPair<Cell,Cell> getMove(const QString &player,const ChessBoard &board);
    static QPair<Cell,Cell> getMove(const Cell &cell,const ChessBoard &board);

    static int getRate(const MoveSequence moves, const ChessBoard board);
private:
    static QVector<Move> mapToVector(const Cell from,const QMap<Cell,bool> moves,const bool eatMovesOnly=false);

    static QVector<Move> getAllPlayersMoves(const QString &player, const ChessBoard &board);
    static Move getMoveFor(const Cell &from,const QString player,const ChessBoard board);

    static bool needSwitchTurn(const MoveSequence sequence);
    static QString getEnemyPlayer(const QString player);

    static int getRate(const ChessBoard board,const ChessBoard oldBoard,const QString player);
    static void sortByRate(QVector<MoveSequence> &sequences, const ChessBoard board);
    static QString toString(const MoveSequence sequence);
};

#endif // BOT_H
