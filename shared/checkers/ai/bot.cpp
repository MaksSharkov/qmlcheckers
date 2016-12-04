#include "bot.h"

QVector<Move> BotUtils::getAllPlayersMoves(const QString &player, const ChessBoard &board)
{
    QVector<Move> result;
    bool mustEat = board.mustEat(player);

    QVector<Cell> cells=board.getPlayersCells(player);
    QMap<Cell,bool> moves;
    foreach(Cell const& cell,cells){
        moves = board.getAvailableMoves(cell);
        result += mapToVector(cell,moves,mustEat);
    }

    return result;
}

QPair<Cell,Cell> BotUtils::getMove(const QString &player, const ChessBoard &board)
{
    QVector<Move> result = getAllPlayersMoves(player, board);

    return result.first().toPair();
}

QPair<Cell,Cell> BotUtils::getMove(const Cell &cell,const ChessBoard &board)
{
     QMap<Cell,bool> moves=board.getAvailableMoves(cell);
     QPair<Cell,Cell> result;

     result.first = cell;
     result.second = moves.key(true);
     return result;
}


QVector<Move> BotUtils::mapToVector(const Cell from,const QMap<Cell,bool> moves,const bool eatMovesOnly)
{
    QVector<Move> result;
    foreach(const Cell to,moves.keys()){
        result.append(Move(from,to,moves.value(to)));
    }

    if(eatMovesOnly){
        foreach(const Move move,result){
            if(!move.isEating())
                result.removeOne(move);
        }
    }

    return result;
}
